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

#include <cassert>
#include <linux/input.h>

#include "common/util.h"
#include "weston-wfits.h"
#include "input-emulator-notify.h"

namespace wfits {
namespace weston {

/*static*/ bool InputEmulatorNotify::registered_ = InputEmulatorFactory::registerEmulator("notify", Create<InputEmulatorNotify>());

InputEmulatorNotify::InputEmulatorNotify()
	: InputEmulator::InputEmulator()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);
}

/*virtual*/
InputEmulatorNotify::~InputEmulatorNotify()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);
}

/*virtual*/
void InputEmulatorNotify::movePointer(const int32_t x, const int32_t y) const
{
	struct weston_seat *seat(Globals::seat());
	wl_fixed_t cx, cy;

#if WESTON_SDK_AT_LEAST(1, 3, 90)
	// Weston >= 1.4 No longer tracks focus
#else
	Globals::compositor()->focus = 1;
#endif

	Globals::pointerXY(&cx, &cy);

#if WESTON_SDK_AT_LEAST(1, 9, 91)
	struct weston_pointer_motion_event motion_event = {0};

	motion_event.mask = WESTON_POINTER_MOTION_REL;
	motion_event.dx = wl_fixed_to_double(wl_fixed_from_int(x) - cx);
	motion_event.dy = wl_fixed_to_double(wl_fixed_from_int(y) - cy);

	notify_motion(seat, weston_compositor_get_time(), &motion_event);
#else
	notify_motion(
		seat, weston_compositor_get_time(),
		wl_fixed_from_int(x) - cx,
		wl_fixed_from_int(y) - cy
	);
#endif
}

/*virtual*/
void InputEmulatorNotify::keySend(const uint32_t key, const uint32_t state) const
{
	struct weston_seat *seat(Globals::seat());

#if WESTON_SDK_AT_LEAST(1, 3, 90)
	// Weston >= 1.4 No longer tracks focus
#else
	Globals::compositor()->focus = 1;
#endif

	if (key == BTN_LEFT or key == BTN_MIDDLE or key == BTN_RIGHT) {
		notify_button(
			seat, weston_compositor_get_time(), key,
			static_cast<wl_pointer_button_state>(state)
		);
	} else {
		assert(key >= 0 and key < 255);
		notify_key(
			seat, weston_compositor_get_time(), key,
			static_cast<wl_keyboard_key_state>(state),
			STATE_UPDATE_AUTOMATIC
		);
	}
}

} // namespace weston
} // namespace wfits
