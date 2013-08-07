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

#ifndef __WESTON_WFITS_H__
#define __WESTON_WFITS_H__

#include <wayland-server.h>

/**
 * Weston SDK 1.0.x workaround for
 * https://bugs.freedesktop.org/show_bug.cgi?id=63485
 **/
extern "C" {
#define private configure_private
#include <weston/compositor.h>
#undef private
}

#include "extensions/protocol/wayland-fits-server-protocol.h"

namespace wfits {
namespace weston {

class Globals
{
public:
	static void init(struct weston_compositor *);
	static void destroy();

	static struct weston_compositor *compositor();
	static struct wl_display *display();
	static struct wl_event_loop *eventLoop();
	static struct weston_seat *seat();
	static struct weston_output *output();
	static void pointerXY(wl_fixed_t *x, wl_fixed_t *y);
	static bool isHeadless();

private:
	static struct weston_compositor *compositor_;
	static bool initialized_;
};

} // namespace weston
} // namespace wfits

#endif
