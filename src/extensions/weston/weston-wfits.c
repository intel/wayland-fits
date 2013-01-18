/*
 * Copyright © 2012 Intel Corporation
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

#include <stdlib.h>
#include <string.h>
#include <wayland-server.h>
#include "weston-source/src/compositor.h"
#include "wayland-fits-server-protocol.h"

struct wfits {
	struct weston_compositor *compositor;
};

static void
move_pointer(struct wl_client *client, struct wl_resource *resource,
	     int32_t x, int32_t y)
{
	struct wfits *wfits = resource->data;
}

static const struct wfits_uinput_interface wfits_uinput_implementation = {
	move_pointer,
};

static void
bind_uinput(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct wfits *wfits = data;
	struct wl_resource *resource;

	resource = wl_client_add_object(client, &wfits_uinput_interface,
					&wfits_uinput_implementation, id, wfits);
}

WL_EXPORT int
module_init(struct weston_compositor *compositor)
{
	struct wfits *wfits;

	weston_log("in weston-wfits::module_init\n");

	wfits = malloc(sizeof *wfits);
	if (wfits == NULL)
		return -1;

	memset(wfits, 0, sizeof *wfits);
	wfits->compositor = compositor;

	if (wl_display_add_global(compositor->wl_display, &wfits_uinput_interface,
				  wfits, bind_uinput) == NULL)
		return -1;
	return 0;
}
