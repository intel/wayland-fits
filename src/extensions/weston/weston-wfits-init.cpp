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
#include "weston-wfits.h"
#include "weston-wfits-input.h"
#include "weston-wfits-query.h"
#include "weston-wfits-manip.h"

static void
compositor_destroy(struct wl_listener *listener, void *data)
{
	delete listener;
	wfits::weston::QueryInterface::destroy();
	wfits::weston::InputInterface::destroy();
	wfits::weston::ManipInterface::destroy();
	wfits::weston::Globals::destroy();
}

extern "C" {

WL_EXPORT int
module_init(struct weston_compositor *compositor,
	    int *argc, char *argv[])
{
	struct wl_listener *listener(new struct wl_listener);

	wfits::weston::Globals::init(compositor);
	wfits::weston::InputInterface::init();
	wfits::weston::QueryInterface::init();
	wfits::weston::ManipInterface::init();

	listener->notify = compositor_destroy;
	wl_signal_add(&compositor->destroy_signal, listener);

	return 0;
}

} // extern "C"
