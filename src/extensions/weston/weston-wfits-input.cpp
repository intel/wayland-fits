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

#include "common/util.h"
#include "input-emulator.h"
#include "input-emulator-uinput.h"
#include "input-emulator-server.h"

#include "weston-wfits-input.h"

namespace wfits {
namespace weston {

/*static*/ const struct wfits_input_interface InputInterface::implementation = {
	InputInterface::movePointer,
	InputInterface::keySend,
};

/*static*/ InputEmulator *InputInterface::emulator_(NULL);
/*static*/ bool InputInterface::initialized_(false);

/*static*/
void InputInterface::init()
{
	if (initialized_) return;

	assert(
		wl_display_add_global(
			Globals::display(),
			&wfits_input_interface,
			NULL,
			InputInterface::bind
		)
	);

	wl_event_loop_add_idle(Globals::eventLoop(), InputInterface::initEmulator, NULL);

	initialized_ = true;
}

/*static*/
void InputInterface::destroy()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);

	if (emulator_ not_eq NULL) {
		delete emulator_;
	}
}

/*static*/
void InputInterface::bind(struct wl_client *wl_client, void *data, uint32_t version, uint32_t id)
{
	ActiveKeys *keys(new ActiveKeys);

	struct wl_resource *resource = wl_client_add_object(
		wl_client, &wfits_input_interface,
		&InputInterface::implementation, id, keys
	);

	resource->destroy = InputInterface::unbind;
}

/*static*/
void InputInterface::unbind(struct wl_resource *resource)
{
	ActiveKeys *keys(static_cast<ActiveKeys*>(resource->data));

	foreach (uint32_t key, *keys) {
		InputInterface::keySend(key, 0);
	}

	delete keys;
	delete resource;
}

/*static*/
void InputInterface::initEmulator(void *data)
{
	if (not Globals::isHeadless()) {
		emulator_ = new InputEmulatorUInput();

		/* sync our input pointer device with weston */
		wl_fixed_t cx, cy;
		Globals::pointerXY(&cx, &cy);
		InputInterface::movePointer(wl_fixed_to_int(cx), wl_fixed_to_int(cy));
	} else {
		emulator_ = new InputEmulatorServer();

		struct weston_seat *seat(Globals::seat());
		weston_seat_init_pointer(seat);
		weston_seat_init_keyboard(seat, NULL);
	}
}

void InputInterface::keySend(const uint32_t key, const uint32_t state)
{
	assert(initialized_);

	emulator_->keySend(key, state);
}

void InputInterface::movePointer(const int32_t x, const int32_t y)
{
	assert(initialized_);

	emulator_->movePointer(x, y);
}

/*static*/
void InputInterface::movePointer(struct wl_client *wl_client,
	struct wl_resource *resource, int32_t x, int32_t y)
{
	InputInterface::movePointer(x, y);
}

/*static*/
void InputInterface::keySend(struct wl_client *wl_client,
	struct wl_resource *resource, uint32_t key, uint32_t state)
{
	ActiveKeys *keys(static_cast<ActiveKeys*>(resource->data));

	InputInterface::keySend(key, state);

	if (state) {
		keys->insert(key);
	} else {
		keys->erase(key);
	}
}

} // namespace weston
} // namespace wfits
