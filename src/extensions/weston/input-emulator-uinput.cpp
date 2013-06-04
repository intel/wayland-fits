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

#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

#include "input-emulator-uinput.h"
#include "common/util.h"
#include "weston-wfits.h"

/**
 * This struct is a copy from Weston's x11-compositor.c.
 * When Weston is using the x11-backend, we can safely
 * cast a "struct weston_compositor" to one of these.
 **/
struct x11_compositor {
	struct weston_compositor base;
	Display *dpy;
	xcb_connection_t *conn;
	xcb_screen_t *screen;
};

/**
 * This struct is a copy from Weston's x11-compositor.c.
 * When Weston is using the x11-backend, we can safely
 * cast a "struct weston_output" to one of these.
 **/
struct x11_output {
	struct weston_output base;
	xcb_window_t window;
};

namespace wfits {
namespace weston {

/*static*/ bool InputEmulatorUInput::registered_ = InputEmulatorFactory::registerEmulator("uinput", Create<InputEmulatorUInput>());

/**
 * Get the width and height (size) of the current display output.  If Weston is
 * using the x11 backend then the result is the size of the X screen.
 * Otherwise, the size of the Weston compositor output is the result.
 **/
static void screenSize(int32_t *width, int32_t *height)
{
	struct weston_output *output(Globals::output());
	struct x11_compositor *x11_compositor = NULL;

	if (std::string(output->make) == "xwayland") {
		x11_compositor = (struct x11_compositor*) Globals::compositor();
		*width = x11_compositor->screen->width_in_pixels;
		*height = x11_compositor->screen->height_in_pixels;
	} else {
		*width = output->width;
		*height = output->height;
	}
}

/**
 * Converts a Weston compositor x,y coordinate into a global coordinate.  When
 * Weston is using the x11 backend, the x,y coordinate is mapped to the X
 * display output (needed to correctly map a uinput pointer to weston).
 **/
static void toScreen(int32_t *x, int32_t *y)
{
	struct weston_output *output(Globals::output());
	struct x11_compositor *x11_compositor = NULL;
	struct x11_output *x11_output = NULL;
	xcb_get_geometry_reply_t *geom;
	xcb_translate_coordinates_reply_t *trans;

	if (std::string(output->make) == "xwayland") {
		x11_compositor = (struct x11_compositor*)Globals::compositor();
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

		delete trans;
		delete geom;
	}
}

InputEmulatorUInput::InputEmulatorUInput()
	: InputEmulator::InputEmulator()
	, fd_(-1)
{
	struct uinput_user_dev device;

	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);

	assert(not Globals::isHeadless());

	fd_ = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (fd_ < 0) {
		weston_log("weston-wfits: failed to create uinput device\n");
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd_, UI_SET_EVBIT, EV_KEY) < 0) {
		exit(EXIT_FAILURE);
	}

	for (int32_t i(0); i < 255; ++i) {
		if (ioctl(fd_, UI_SET_KEYBIT, i) < 0) {
			exit(EXIT_FAILURE);
		}
	}

	if (ioctl(fd_, UI_SET_KEYBIT, BTN_LEFT) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd_, UI_SET_KEYBIT, BTN_RIGHT) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd_, UI_SET_KEYBIT, BTN_MIDDLE) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd_, UI_SET_EVBIT, EV_ABS) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd_, UI_SET_ABSBIT, ABS_X) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd_, UI_SET_ABSBIT, ABS_Y) < 0) {
		exit(EXIT_FAILURE);
	}

	memset(&device, 0, sizeof(device));
	snprintf(device.name, UINPUT_MAX_NAME_SIZE, "WFITS UINPUT");
	device.id.bustype = BUS_USB;
	device.id.vendor  = 0x1;
	device.id.product = 0x1;
	device.id.version = 1;

	/*
	* TODO: Need to handle multidisplay, eventually.
	*/
	int32_t width, height;
	screenSize(&width, &height);
	device.absmin[ABS_X] = 0;
	device.absmax[ABS_X] = width - 1;
	device.absmin[ABS_Y] = 0;
	device.absmax[ABS_Y] = height - 1;

	if (write(fd_, &device, sizeof(device)) < 0) {
		exit(EXIT_FAILURE);
	}

	if (ioctl(fd_, UI_DEV_CREATE) < 0) {
		exit(EXIT_FAILURE);
	}

	/* sync our input pointer device with weston */
	wl_fixed_t cx, cy;
	Globals::pointerXY(&cx, &cy);
	movePointer(wl_fixed_to_int(cx), wl_fixed_to_int(cy));
}

/*virtual*/
InputEmulatorUInput::~InputEmulatorUInput()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);

	if (ioctl(fd_, UI_DEV_DESTROY) < 0) {
		weston_log("weston-wfits: failed to destroy uinput device\n");
	}
}

/*virtual*/
void InputEmulatorUInput::movePointer(const int32_t x, const int32_t y) const
{
	struct input_event event;
	int32_t gx(x), gy(y);

	toScreen(&gx, &gy);

	memset(&event, 0, sizeof(event));

	event.type = EV_ABS;
	event.code = ABS_X;
	event.value = gx;
	writeEvent(&event);

	event.type = EV_ABS;
	event.code = ABS_Y;
	event.value = gy;
	writeEvent(&event);

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	writeEvent(&event);
}

/*virtual*/
void InputEmulatorUInput::keySend(const uint32_t key, const uint32_t state) const
{
	struct input_event event;

	memset(&event, 0, sizeof(event));

	event.type = EV_KEY;
	event.code = key;
	event.value = state;
	writeEvent(&event);

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	writeEvent(&event);
}

void InputEmulatorUInput::writeEvent(struct input_event *event) const
{
	if (write(fd_, event, sizeof(*event)) == -1) {
		perror("write");
	}
}

} // namespace weston
} // namespace wfits
