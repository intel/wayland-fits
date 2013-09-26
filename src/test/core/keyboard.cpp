/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/mman.h>

#include "keyboard.h"

namespace wfits {
namespace test {
namespace core {

Keyboard::Keyboard(const Seat& seat)
	: seat_(seat)
	, focus_(NULL)
	, focusSerial_(0)
	, wl_keyboard_(NULL)
	, keys_()
	, modsDepressed_(0)
	, modsLatched_(0)
	, modsLocked_(0)
	, group_(0)
	, keymap_(NULL)
{
	ASSERT(seat.capabilities() & WL_SEAT_CAPABILITY_KEYBOARD);

	wl_keyboard_ = wl_seat_get_keyboard(seat);

	ASSERT(wl_keyboard_ != NULL);

	wl_keyboard_set_user_data(*this, this);

	static const wl_keyboard_listener listener = {
		keymap, enter, leave, key, modifiers
	};

	wl_keyboard_add_listener(*this, &listener, this);
}

/*virtual*/ Keyboard::~Keyboard()
{
	if (keymap_)
	{
		xkb_map_unref(keymap_);
	}

	wl_keyboard_destroy(*this);
}

bool Keyboard::hasFocus(wl_surface* surface) const
{
	return focus() == surface;
}

bool Keyboard::keyPressed(uint32_t key) const
{
	return std::find(keys_.begin(), keys_.end(), key) != keys_.end();
}

void Keyboard::pressKey(uint32_t key)
{
	if (not keyPressed(key))
	{
		keys_.push_back(key);
	}
}

void Keyboard::releaseKey(uint32_t key)
{
	const Keys::iterator it = std::find(keys_.begin(), keys_.end(), key);

	if (it != keys_.end())
	{
		keys_.erase(it);
	}
}

/* static */ void Keyboard::keymap(void *data,
				   struct wl_keyboard *wl_keyboard,
				   uint32_t format,
				   int32_t fd,
				   uint32_t size)
{
	Keyboard* keyboard = static_cast<Keyboard*>(data);
	struct xkb_keymap *keymap;
	char *map_str;

	ASSERT(wl_keyboard == *keyboard);
	ASSERT(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);

	map_str = (char *) mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	ASSERT(map_str != MAP_FAILED);

	keymap = xkb_map_new_from_string(keyboard->seat_.display().xkbContext(),
					 map_str,
					 XKB_KEYMAP_FORMAT_TEXT_V1,
					 (enum xkb_keymap_compile_flags) 0);
	munmap(map_str, size);
	close(fd);

	ASSERT(keymap != NULL);

	if (keyboard->keymap_ != NULL)
	{
		xkb_map_unref(keyboard->keymap_);
	}

	keyboard->keymap_ = keymap;
}

/* static */ void Keyboard::enter(void *data,
				  struct wl_keyboard *wl_keyboard,
				  uint32_t serial,
				  struct wl_surface *surface,
				  struct wl_array *keys)
{
	Keyboard* keyboard = static_cast<Keyboard*>(data);
	ASSERT(wl_keyboard == *keyboard);

	keyboard->focus_ = surface;
	keyboard->focusSerial_ = serial;

	keyboard->keys_.resize(keys->size / sizeof(uint32_t));
	memcpy(&keyboard->keys_[0], keys->data, keys->size);
}

/* static */ void Keyboard::leave(void *data,
				  struct wl_keyboard *wl_keyboard,
				  uint32_t serial,
				  struct wl_surface *surface)
{
	Keyboard* keyboard = static_cast<Keyboard*>(data);
	ASSERT(wl_keyboard == *keyboard);

	keyboard->focus_ = NULL;
	keyboard->keys_.resize(0);

	/* We don't want to reset the modifiers here because the
	 * compositor may still send updated modifier state if the
	 * surface has pointer focus */
}

/* static */ void Keyboard::key(void *data,
				struct wl_keyboard *wl_keyboard,
				uint32_t serial,
				uint32_t time,
				uint32_t key,
				uint32_t state)
{
	Keyboard* keyboard = static_cast<Keyboard*>(data);
	ASSERT(wl_keyboard == *keyboard);

	if (state == 0)
	{
		keyboard->releaseKey(key);
	}
	else
	{
		keyboard->pressKey(key);
	}
}

/* static */ void Keyboard::modifiers(void *data,
				      struct wl_keyboard *wl_keyboard,
				      uint32_t serial,
				      uint32_t modsDepressed,
				      uint32_t modsLatched,
				      uint32_t modsLocked,
				      uint32_t group)
{
	Keyboard* keyboard = static_cast<Keyboard*>(data);
	ASSERT(wl_keyboard == *keyboard);

	keyboard->modsDepressed_ = modsDepressed;
	keyboard->modsLatched_ = modsLatched;
	keyboard->modsLocked_ = modsLocked;
	keyboard->group_ = group;
}

namespace wrapper {

	TEST(Keyboard)
	{
		Display display;
		Seat seat(display);
		Keyboard keyboard(seat);

		FAIL_UNLESS_EQUAL(&keyboard.seat(), &seat);
		FAIL_IF((wl_keyboard*)keyboard == NULL);
		FAIL_UNLESS_EQUAL(wl_keyboard_get_user_data(keyboard), &keyboard);
		FAIL_UNLESS(keyboard.hasFocus(NULL));
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
