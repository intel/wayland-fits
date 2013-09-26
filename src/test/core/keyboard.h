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

#ifndef __WFITS_CORE_KEYBOARD_H__
#define __WFITS_CORE_KEYBOARD_H__

#include <xkbcommon/xkbcommon.h>
#include <vector>
#include "seat.h"

namespace wfits {
namespace test {
namespace core {

class Keyboard
{
	typedef std::vector<uint32_t> Keys;

public:
	Keyboard(const Seat&);

	virtual ~Keyboard();

	operator wl_keyboard*() const { return wl_keyboard_; }
	const Seat& seat() const { return seat_; }
	wl_surface* focus() const { return focus_; }
	xkb_keymap* keymap() const { return keymap_; }
	const uint32_t focusSerial() const { return focusSerial_; }

	bool hasFocus(wl_surface*) const;

	const uint32_t modsDepressed() const { return modsDepressed_; }
	const uint32_t modsLatched() const { return modsLatched_; }
	const uint32_t modsLocked() const { return modsLocked_; }
	const uint32_t group() const { return group_; }
	bool keyPressed(uint32_t key) const;

private:
	static void keymap(void *data,
			   struct wl_keyboard *wl_keyboard,
			   uint32_t format,
			   int32_t fd,
			   uint32_t size);
	static void enter(void *data,
			  struct wl_keyboard *wl_keyboard,
			  uint32_t serial,
			  struct wl_surface *surface,
			  struct wl_array *keys);
	static void leave(void *data,
			  struct wl_keyboard *wl_keyboard,
			  uint32_t serial,
			  struct wl_surface *surface);
	static void key(void *data,
			struct wl_keyboard *wl_keyboard,
			uint32_t serial,
			uint32_t time,
			uint32_t key,
			uint32_t state);
	static void modifiers(void *data,
			      struct wl_keyboard *wl_keyboard,
			      uint32_t serial,
			      uint32_t modsDepressed,
			      uint32_t modsLatched,
			      uint32_t modsLocked,
			      uint32_t group);

	void pressKey(uint32_t key);
	void releaseKey(uint32_t key);

	const Seat&	seat_;
	wl_surface*	focus_;
	uint32_t	focusSerial_;
	wl_keyboard*	wl_keyboard_;
	Keys		keys_;
	uint32_t	modsDepressed_;
	uint32_t	modsLatched_;
	uint32_t	modsLocked_;
	uint32_t	group_;
	struct xkb_keymap* keymap_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
