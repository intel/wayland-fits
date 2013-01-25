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

#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <assert.h>

#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

#include <wayland-server.h>
#include <weston/src/compositor.h>

#include "extensions/protocol/wayland-fits-server-protocol.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

struct x11_compositor {
	struct weston_compositor base;
	Display *dpy;
	xcb_connection_t *conn;
};

struct x11_output {
	struct weston_output base;
	xcb_window_t window;
};

struct wfits {
	struct weston_compositor *compositor;
	struct wl_listener compositor_destroy_listener;
	int pointer_fd;
};

static struct weston_seat *
get_seat(struct wfits *wfits)
{
	struct wl_list *seat_list;
	struct weston_seat *seat;

	seat_list = &wfits->compositor->seat_list;
	assert(wl_list_length(seat_list) == 1);
	seat = container_of(seat_list->next, struct weston_seat, link);

	return seat;
}

static struct weston_output *
get_output(struct wfits *wfits)
{
	struct wl_list *output_list;

	output_list = &wfits->compositor->output_list;
	if (wl_list_length(output_list) != 1) {
		weston_log("weston-wfits: ERROR: single output support only!\n");
		assert(wl_list_length(output_list) == 1);
	}

	return container_of(output_list->next, struct weston_output, link);
}

static void
compositor_to_global(struct wfits* wfits, int32_t *x, int32_t *y)
{
	struct weston_output *output;
	struct x11_compositor *x11_compositor = NULL;
	struct x11_output *x11_output = NULL;
	xcb_get_geometry_reply_t *geom;
	xcb_translate_coordinates_reply_t *trans;

	output = get_output(wfits);
	if (strcmp(output->make, "xwayland") == 0) {
		x11_compositor = (struct x11_compositor*) wfits->compositor;
		x11_output = (struct x11_output*) output;

		geom = xcb_get_geometry_reply(
			x11_compositor->conn,
			xcb_get_geometry(
				x11_compositor->conn,
				x11_output->window
			),
			NULL
		);

		trans = xcb_translate_coordinates_reply(
			x11_compositor->conn,
			xcb_translate_coordinates(
				x11_compositor->conn,
				x11_output->window,
				geom->root,
				-(geom->border_width),
				-(geom->border_width)
			),
			NULL
		);

		weston_log("%d %d :: %d %d\n", *x, *y, trans->dst_x, trans->dst_y);
		*x += (int16_t)trans->dst_x;
		*y += (int16_t)trans->dst_y;

		free(trans);
		free(geom);
	}
}

static void
input_move_pointer(struct wl_client *client, struct wl_resource *resource,
	     int32_t x, int32_t y)
{
	struct wfits *wfits = resource->data;
	struct input_event event;

	compositor_to_global(wfits, &x, &y);

	weston_log("weston-wfits: moving pointer to %d %d\n", x, y);

	memset(&event, 0, sizeof(event));

	event.type = EV_ABS;
	event.code = ABS_X;
	event.value = x;
	write(wfits->pointer_fd, &event, sizeof(event));

	event.type = EV_ABS;
	event.code = ABS_Y;
	event.value = y;
	write(wfits->pointer_fd, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(wfits->pointer_fd, &event, sizeof(event));
}

static const struct wfits_input_interface wfits_input_implementation = {
	input_move_pointer,
};

static void
bind_input(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct wfits *wfits = data;
	struct wl_resource *resource;

	resource = wl_client_add_object(client, &wfits_input_interface,
					&wfits_input_implementation, id, wfits);
}

static void
create_pointer(struct wfits* wfits)
{
	struct uinput_user_dev device;
	struct weston_output *output = get_output(wfits);

	weston_log("weston-wfits: creating uinput device\n");

	wfits->pointer_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (wfits->pointer_fd < 0) {
		weston_log("weston-wfits: failed to create uinput device\n");
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->pointer_fd, UI_SET_EVBIT, EV_KEY) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->pointer_fd, UI_SET_KEYBIT, BTN_LEFT) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->pointer_fd, UI_SET_KEYBIT, BTN_RIGHT) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->pointer_fd, UI_SET_EVBIT, EV_ABS) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->pointer_fd, UI_SET_ABSBIT, ABS_X) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->pointer_fd, UI_SET_ABSBIT, ABS_Y) < 0) {
		exit(EXIT_FAILURE);
	}

	memset(&device, 0, sizeof(device));
	snprintf(device.name, UINPUT_MAX_NAME_SIZE, "WFITS ABS POINTER");
	device.id.bustype = BUS_USB;
	device.id.vendor  = 0x1;
	device.id.product = 0x1;
	device.id.version = 1;

	/* FIXME: What about multidisplay? What about x11-backend?
	 * On x11-backend the device should be bound to the compositor
	 * X-client window viewport.
	 */
	device.absmin[ABS_X] = output->x;
	device.absmax[ABS_X] = output->width;
	device.absmin[ABS_Y] = output->y;
	device.absmax[ABS_Y] = output->height;

	if (write(wfits->pointer_fd, &device, sizeof(device)) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->pointer_fd, UI_DEV_CREATE) < 0) {
		exit(EXIT_FAILURE);
	}
}

static void
query_surface_geometry(struct wl_client *client, struct wl_resource *resource,
	     struct wl_resource *surface_resource)
{
	struct wfits *wfits = resource->data;
	struct weston_surface *surface = surface_resource->data;

	wfits_query_send_surface_geometry(resource,
					  wl_fixed_from_double(surface->geometry.x),
					  wl_fixed_from_double(surface->geometry.y),
					  surface->geometry.width,
					  surface->geometry.height);
}

static void
query_pointer_position(struct wl_client *client, struct wl_resource *resource)
{
	struct wfits *wfits = resource->data;
	struct weston_seat *seat = get_seat(wfits);

	wfits_query_send_pointer_position(resource, seat->pointer.x,
					  seat->pointer.y);
}

static const struct wfits_query_interface wfits_query_implementation = {
	query_surface_geometry,
	query_pointer_position,
};

static void
bind_query(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct wfits *wfits = data;
	struct wl_resource *resource;

	resource = wl_client_add_object(client, &wfits_query_interface,
					&wfits_query_implementation, id, wfits);
}

static void
compositor_destroy(struct wl_listener *listener, void *data)
{
	struct wfits *wfits = container_of(listener, struct wfits,
					   compositor_destroy_listener);

	weston_log("weston-wfits: destroying uinput device\n");

	if (ioctl(wfits->pointer_fd, UI_DEV_DESTROY) < 0) {
		weston_log("weston-wfits: failed to destroy uinput device\n");
	}
}

WL_EXPORT int
module_init(struct weston_compositor *compositor)
{
	struct wfits *wfits;
	struct wl_event_loop *loop;

	wfits = malloc(sizeof *wfits);
	if (wfits == NULL)
		return -1;

	memset(wfits, 0, sizeof *wfits);
	wfits->compositor = compositor;

	if (wl_display_add_global(compositor->wl_display,
				  &wfits_input_interface,
				  wfits, bind_input) == NULL)
		return -1;

	if (wl_display_add_global(compositor->wl_display,
				  &wfits_query_interface,
				  wfits, bind_query) == NULL)
		return -1;
	
	create_pointer(wfits);

	wfits->compositor_destroy_listener.notify = compositor_destroy;
	wl_signal_add(&compositor->destroy_signal,
		      &wfits->compositor_destroy_listener);

	return 0;
}
