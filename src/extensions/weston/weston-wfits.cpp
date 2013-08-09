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

#include "config.h"
#include "common/util.h"
#include "weston-wfits.h"

namespace wfits {
namespace weston {

/*static*/ struct weston_compositor *Globals::compositor_(NULL);
/*static*/ bool Globals::initialized_(false);

/*static*/
void Globals::init(struct weston_compositor *compositor)
{
	if (initialized_) return;

	compositor_ = compositor;
	initialized_ = true;

	struct weston_seat *seat(Globals::seat());

	if (isHeadless()) {
		weston_seat_init_pointer(seat);
		weston_seat_init_keyboard(seat, NULL);
	}
}

/*static*/
void Globals::destroy()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);
}

/*static*/
struct weston_compositor* Globals::compositor()
{
	assert(initialized_);

	return compositor_;
}

/*static*/
struct wl_display* Globals::display()
{
	return compositor()->wl_display;
}

/*static*/
struct wl_event_loop* Globals::eventLoop()
{
	return wl_display_get_event_loop(display());
}

/**
 * Get the weston_seat associated with the Weston compositor.
 * Currently, only one (1) seat is supported by this plugin.
 **/
struct weston_seat * Globals::seat()
{
	struct wl_list *seat_list;
	struct weston_seat *seat;

	seat_list = &compositor()->seat_list;
	assert(wl_list_length(seat_list) == 1);
	seat = container_of(seat_list->next, struct weston_seat, link);

	return seat;
}

/**
 * Get the current x,y coordinate of the compositor pointer.
 **/
void Globals::pointerXY(wl_fixed_t *x, wl_fixed_t *y)
{
	struct weston_seat *seat(Globals::seat());

	*x = seat->pointer->x;
	*y = seat->pointer->y;
}

bool Globals::isHeadless()
{
	struct weston_output *output(Globals::output());
	return "headless" == std::string(output->model);
}

/**
 * Get the weston_output associated with the Weston compositor.
 * Currently, only one (1) output is supported by this plugin.
 **/
struct weston_output * Globals::output()
{
	struct wl_list *output_list;

	output_list = &compositor()->output_list;
	if (wl_list_length(output_list) != 1) {
		weston_log("weston-wfits: ERROR: single output support only!\n");
		assert(wl_list_length(output_list) == 1);
	}

	return container_of(output_list->next, struct weston_output, link);
}

} // namespace weston
} // namespace wfits
