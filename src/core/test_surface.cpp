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

#include "display.h"
#include "compositor.h"
#include "pointer.h"
#include "seat.h"
#include "surface.h"
#include "shell.h"
#include "shell_surface.h"
#include "shm.h"
#include "input.h"
#include "query.h"

class Client
{
public:
	Client(uint32_t width, uint32_t height)
		: display()
		, compositor(display)
		, shell(display)
		, seat(display)
		, pointer(seat)
		, surface(compositor)
		, shellSurface(shell, surface)
		, shm(display)
		, buffer(shm, width, height)
		, query(display)
		, input(query)
	{
		wl_surface_attach(surface, buffer, 0, 0);
		wl_surface_damage(surface, 0, 0, buffer.width(),
				  buffer.height());
		surface.commit();
	}

	void movePointer(int32_t x, int32_t y)
	{
		Query::Geometry geometry = query.getSurfaceGeometry(surface);
		input.moveGlobalPointer(geometry.x + x, geometry.y + y);
	}

	void checkFocus(bool focus)
	{
		display.yield();
		for(unsigned i(0); i < 20 && (pointer.hasFocus(&surface) != focus); ++i) {
			display.yield(i*0.001*1e6);
		}
		FAIL_UNLESS_EQUAL(pointer.hasFocus(&surface), focus);
	}

	void checkPointer(uint32_t x, uint32_t y)
	{
		display.yield();
		for(unsigned i(0); i < 20 && (pointer.x() != x || pointer.y() != y); ++i) {
			display.yield(i*0.001*1e6);
		}
		FAIL_UNLESS_EQUAL(pointer.x(), x);
		FAIL_UNLESS_EQUAL(pointer.y(), y);
	}

	void checkButton(uint32_t button, uint32_t state)
	{
		display.yield();
		for (unsigned i(0);
		     i < 20 && (pointer.button() != button
		     || pointer.buttonState() != state); ++i) {
			display.yield(i*0.001*1e6);
		}
		FAIL_UNLESS_EQUAL(pointer.button(), button);
		FAIL_UNLESS_EQUAL(pointer.buttonState(), state);
	}

	Display display;
	Compositor compositor;
	Shell shell;
	Seat seat;
	Pointer pointer;
	Surface surface;
	ShellSurface shellSurface;
	SharedMemory shm;
	SharedMemoryBuffer buffer;
	Query query;
	Input input;
};

#include <linux/input.h>

TEST(SurfacePointer, "Core/Input")
{
	Client client(120, 75);

	for (unsigned x(0); x < 120; x+=10) {
		for (unsigned y(0); y < 75; y+=10) {
			client.movePointer(x, y);
			client.checkFocus(true);
			client.checkPointer(x, y);
		}
	}
	client.movePointer(-1, -1);
	client.checkFocus(false);

	client.movePointer(5, 5);
	client.checkFocus(true);
	
	wfits_input_key_send(client.input, BTN_LEFT, 1);
	client.checkButton(BTN_LEFT, 1);

	wfits_input_key_send(client.input, BTN_LEFT, 0);
	client.checkButton(BTN_LEFT, 0);

	wfits_input_key_send(client.input, BTN_RIGHT, 1);
	client.checkButton(BTN_RIGHT, 1);

	wfits_input_key_send(client.input, BTN_RIGHT, 0);
	client.checkButton(BTN_RIGHT, 0);

	wfits_input_key_send(client.input, BTN_MIDDLE, 1);
	client.checkButton(BTN_MIDDLE, 1);

	wfits_input_key_send(client.input, BTN_MIDDLE, 0);
	client.checkButton(BTN_MIDDLE, 0);
}
