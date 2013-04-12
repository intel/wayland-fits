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
#include <unistd.h>

#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

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

#include <set>

#include "config.h"
#include "extensions/protocol/wayland-fits-server-protocol.h"
#include "common/util.h"

/** TODO: Convert this code to C++-style classes, methods, etc... **/

struct x11_compositor {
	struct weston_compositor base;
	Display *dpy;
	xcb_connection_t *conn;
	xcb_screen_t *screen;
};

struct x11_output {
	struct weston_output base;
	xcb_window_t window;
};

struct wfits {
	struct weston_compositor *compositor;
	struct wl_listener compositor_destroy_listener;
	int input_fd;
};

struct wfits_input_client {
	struct wfits *wfits;
	std::set<uint32_t> *active_keys;
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
global_size(struct wfits *wfits, int32_t *width, int32_t *height)
{
	struct weston_output *output;
	struct x11_compositor *x11_compositor = NULL;

	output = get_output(wfits);
	if (strcmp(output->make, "xwayland") == 0) {
		x11_compositor = (struct x11_compositor*) wfits->compositor;
		*width = x11_compositor->screen->width_in_pixels;
		*height = x11_compositor->screen->height_in_pixels;
	} else {
		*width = output->width;
		*height = output->height;
	}
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

		*x += (int16_t)trans->dst_x;
		*y += (int16_t)trans->dst_y;

		free(trans);
		free(geom);
	}
}

void
move_pointer(struct wfits *wfits, int32_t x, int32_t y)
{
	struct input_event event;

	compositor_to_global(wfits, &x, &y);

	memset(&event, 0, sizeof(event));

	event.type = EV_ABS;
	event.code = ABS_X;
	event.value = x;
	write(wfits->input_fd, &event, sizeof(event));

	event.type = EV_ABS;
	event.code = ABS_Y;
	event.value = y;
	write(wfits->input_fd, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(wfits->input_fd, &event, sizeof(event));
}

static void
input_move_pointer(struct wl_client *client, struct wl_resource *resource,
		   int32_t x, int32_t y)
{
	struct wfits_input_client *wfits_input_client =
		static_cast<struct wfits_input_client*>(resource->data);
	move_pointer(wfits_input_client->wfits, x, y);
}

static void
key_send(struct wfits *wfits, uint32_t key, uint32_t state)
{
	struct input_event event;

	memset(&event, 0, sizeof(event));

	event.type = EV_KEY;
	event.code = key;
	event.value = state;
	write(wfits->input_fd, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(wfits->input_fd, &event, sizeof(event));
}

static void
input_key_send(struct wl_client *client, struct wl_resource *resource,
		  uint32_t key, uint32_t state)
{
	struct wfits_input_client *wfits_input_client =
		static_cast<struct wfits_input_client*>(resource->data);

	key_send(wfits_input_client->wfits, key, state);

	if (state) {
		wfits_input_client->active_keys->insert(key);
	} else {
		wfits_input_client->active_keys->erase(key);
	}
}

static const struct wfits_input_interface wfits_input_implementation = {
	input_move_pointer,
	input_key_send,
};

static void
unbind_input_client(struct wl_resource *resource)
{
	struct wfits_input_client *wfits_input_client =
		static_cast<struct wfits_input_client*>(resource->data);

	foreach (uint32_t key, *wfits_input_client->active_keys)
	{
		key_send(wfits_input_client->wfits, key, 0);
	}

	delete wfits_input_client->active_keys;
	delete wfits_input_client;
	free(resource);
}

static void
bind_input(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct wl_resource *resource;
	struct wfits_input_client *wfits_input_client(
		new struct wfits_input_client);

	wfits_input_client->wfits = static_cast<struct wfits*>(data);
	wfits_input_client->active_keys = new std::set<uint32_t>;

	resource = wl_client_add_object(client, &wfits_input_interface,
					&wfits_input_implementation, id,
					wfits_input_client);
	resource->destroy = unbind_input_client;
}

static void
create_input(struct wfits* wfits)
{
	struct uinput_user_dev device;
	struct weston_output *output = get_output(wfits);
	int32_t width;
	int32_t height;
	int32_t i;
	
	weston_log("weston-wfits: creating uinput device\n");

	wfits->input_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (wfits->input_fd < 0) {
		weston_log("weston-wfits: failed to create uinput device\n");
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->input_fd, UI_SET_EVBIT, EV_KEY) < 0) {
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < 255; i++){
		if (ioctl(wfits->input_fd, UI_SET_KEYBIT, i) < 0) {
			exit(EXIT_FAILURE);
		}
	}
	
	if (ioctl(wfits->input_fd, UI_SET_KEYBIT, BTN_LEFT) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->input_fd, UI_SET_KEYBIT, BTN_RIGHT) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->input_fd, UI_SET_KEYBIT, BTN_MIDDLE) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->input_fd, UI_SET_EVBIT, EV_ABS) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->input_fd, UI_SET_ABSBIT, ABS_X) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->input_fd, UI_SET_ABSBIT, ABS_Y) < 0) {
		exit(EXIT_FAILURE);
	}

	memset(&device, 0, sizeof(device));
	snprintf(device.name, UINPUT_MAX_NAME_SIZE, "WFITS INPUT");
	device.id.bustype = BUS_USB;
	device.id.vendor  = 0x1;
	device.id.product = 0x1;
	device.id.version = 1;

	/* FIXME: What about multidisplay? What about x11-backend?
	 * On x11-backend the device should be bound to the compositor
	 * X-client window viewport.
	 */
	global_size(wfits, &width, &height);
	device.absmin[ABS_X] = 0;
	device.absmax[ABS_X] = width;
	device.absmin[ABS_Y] = 0;
	device.absmax[ABS_Y] = height;

	if (write(wfits->input_fd, &device, sizeof(device)) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(wfits->input_fd, UI_DEV_CREATE) < 0) {
		exit(EXIT_FAILURE);
	}
}

static void
query_surface_geometry(struct wl_client *client, struct wl_resource *resource,
		       struct wl_resource *surface_resource, uint32_t result_id)
{
	struct wl_resource result;
	struct wfits *wfits = static_cast<struct wfits*>(resource->data);
	struct weston_surface *surface =
		static_cast<struct weston_surface*>(surface_resource->data);

	memset(&result, 0, sizeof(result));
	result.object.interface = &wfits_query_result_interface;
	result.object.id = result_id;
	result.destroy = NULL;
	result.client = client;
	result.data = NULL;

	wl_client_add_resource(client, &result);

	wfits_query_result_send_surface_geometry(
		&result,
		wl_fixed_from_double(surface->geometry.x),
		wl_fixed_from_double(surface->geometry.y),
		surface->geometry.width,
		surface->geometry.height);
	wl_resource_destroy(&result);
}

static void
query_global_pointer_position(struct wl_client *client,
			      struct wl_resource *resource,
			      uint32_t result_id)
{
	struct wl_resource result;
	struct wfits *wfits = static_cast<struct wfits*>(resource->data);
	struct weston_seat *seat = get_seat(wfits);

	memset(&result, 0, sizeof(result));
	result.object.interface = &wfits_query_result_interface;
	result.object.id = result_id;
	result.destroy = NULL;
	result.client = client;
	result.data = NULL;

	wl_client_add_resource(client, &result);
	wfits_query_result_send_global_pointer_position(&result,
							seat->pointer.x,
						        seat->pointer.y);
	wl_resource_destroy(&result);
}

static const struct wfits_query_interface wfits_query_implementation = {
	query_surface_geometry,
	query_global_pointer_position,
};

static void
bind_query(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct wfits *wfits = static_cast<struct wfits*>(data);
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

	if (ioctl(wfits->input_fd, UI_DEV_DESTROY) < 0) {
		weston_log("weston-wfits: failed to destroy uinput device\n");
	}
}

#ifdef HAVE_WESTON_SDK2
WL_EXPORT int
module_init(struct weston_compositor *compositor,
	int *argc, char *argv[], const char *config_file)
#else
WL_EXPORT int
module_init(struct weston_compositor *compositor)
#endif
{
	struct wfits *wfits;
	struct wl_event_loop *loop;
	struct weston_seat *seat;

	wfits = static_cast<struct wfits*>(malloc(sizeof *wfits));
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
	
	create_input(wfits);

	/* sync our input pointer device with weston */
	seat = get_seat(wfits);
	move_pointer(wfits, wl_fixed_to_int(seat->pointer.x),
		     wl_fixed_to_int(seat->pointer.y));

	wfits->compositor_destroy_listener.notify = compositor_destroy;
	wl_signal_add(&compositor->destroy_signal,
		      &wfits->compositor_destroy_listener);

	return 0;
}
