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

#include "harness.h"
#include "compositor.h"
#include "pointer.h"
#include "keyboard.h"
#include "seat.h"
#include "surface.h"
#include "shell.h"
#include "shell_surface.h"
#include "shm.h"

namespace wfits {
namespace test {
namespace core {
namespace input {

class DummyClient : public Harness
{
public:
	DummyClient()
		: Harness::Harness()
		, compositor_(display())
		, shell_(display())
		, seat_(display())
		, surface_(compositor_)
		, shellSurface_(shell_, surface_)
		, shm_(display())
		, buffer_(shm_, 128, 128)
	{
		wl_surface_attach(surface_, buffer_, 0, 0);
		wl_surface_damage(surface_, 0, 0,
				  buffer_.width(), buffer_.height());
		surface_.commit();

		setSurfacePosition(surface_, 300, 100);
	}

private:
	Compositor compositor_;
	Shell shell_;
	Seat seat_;
	Surface surface_;
	ShellSurface shellSurface_;
	SharedMemory shm_;
	SharedMemoryBuffer buffer_;
};

class MultiResourceTest : public Harness
{
public:
	MultiResourceTest()
		: Harness::Harness()
		, compositor_(display())
		, shell_(display())
		, seat_(display())
		, surface_(compositor_)
		, shellSurface_(shell_, surface_)
		, shm_(display())
		, buffer_(shm_, 128, 128)
	{
	}

	void setup();
	void testPointer();
	void testKeyboard();

	void movePointer(const int32_t x, const int32_t y)
	{
		Geometry geometry(getSurfaceGeometry(surface_));
		setGlobalPointerPosition(geometry.x + x, geometry.y + y);
	}

	void checkPointer(Pointer *pointer, const int32_t x, const int32_t y)
	{
		YIELD_UNTIL(pointer->x() == x and pointer->y() == y);
	}

private:
	Compositor compositor_;
	Shell shell_;
	Seat seat_;
	Surface surface_;
	ShellSurface shellSurface_;
	SharedMemory shm_;
	SharedMemoryBuffer buffer_;
};

void MultiResourceTest::setup()
{
	wl_surface_attach(surface_, buffer_, 0, 0);
	wl_surface_damage(surface_, 0, 0, buffer_.width(), buffer_.height());
	surface_.commit();

	setSurfacePosition(surface_, 100, 100);

	queueStep(boost::bind(&MultiResourceTest::testPointer, boost::ref(*this)));
	queueStep(boost::bind(&MultiResourceTest::testKeyboard, boost::ref(*this)));
}

void MultiResourceTest::testPointer()
{
	Pointer *pointer_a = new Pointer(seat_);
	Pointer *pointer_b = new Pointer(seat_);

	movePointer(42, 120);

	/* Verify that both pointers received the new position */
	YIELD_UNTIL(pointer_a->hasFocus(surface_));
	YIELD_UNTIL(pointer_b->hasFocus(surface_));
	checkPointer(pointer_a, 42, 120);
	checkPointer(pointer_b, 42, 120);

	/* Move the pointer outside of the surface */
	movePointer(130, 130);
	YIELD_UNTIL(!pointer_a->hasFocus(surface_));
	YIELD_UNTIL(!pointer_b->hasFocus(surface_));

	/* Move the pointer back inside the surface while we only have
	 * one pointer */
	delete pointer_b;
	yield();
	movePointer(42, 120);
	YIELD_UNTIL(pointer_a->hasFocus(surface_));
	checkPointer(pointer_a, 42, 120);

	/* Verify that the second pointer gets the updated position
	 * even if it is created later */
	pointer_b = new Pointer(seat_);
	YIELD_UNTIL(pointer_b->hasFocus(surface_));
	checkPointer(pointer_b, 42, 120);

	/* Both pointers should have the same focus serial or one of
	 * them would fail to set the cursor */
	ASSERT(pointer_a->focusSerial() == pointer_b->focusSerial());

	delete pointer_a;
	delete pointer_b;
}

void MultiResourceTest::testKeyboard()
{
	Keyboard *keyboard_a = new Keyboard(seat_);
	Keyboard *keyboard_b = new Keyboard(seat_);

	/* Both keyboards should initially have focus */
	YIELD_UNTIL(keyboard_a->hasFocus(surface_));
	YIELD_UNTIL(keyboard_b->hasFocus(surface_));

	xkb_mod_index_t shiftMod =
		xkb_keymap_mod_get_index(keyboard_a->keymap(),
					 XKB_MOD_NAME_SHIFT);
	ASSERT(shiftMod != XKB_MOD_INVALID);

	inputKeySend(KEY_LEFTSHIFT, 1);
	inputKeySend(KEY_A, 1);

	/* Both keyboards should get key events */
	YIELD_UNTIL(keyboard_a->keyPressed(KEY_LEFTSHIFT) and
		    keyboard_b->keyPressed(KEY_LEFTSHIFT) and
		    keyboard_a->keyPressed(KEY_A) and
		    keyboard_b->keyPressed(KEY_A) and
		    (keyboard_a->modsDepressed() & (1 << shiftMod)) != 0 and
		    (keyboard_b->modsDepressed() & (1 << shiftMod)) != 0);

	inputKeySend(KEY_LEFTSHIFT, 0);
	inputKeySend(KEY_A, 0);

	YIELD_UNTIL(!keyboard_a->keyPressed(KEY_LEFTSHIFT) and
		    !keyboard_b->keyPressed(KEY_LEFTSHIFT) and
		    !keyboard_a->keyPressed(KEY_A) and
		    !keyboard_b->keyPressed(KEY_A) and
		    (keyboard_a->modsDepressed() & (1 << shiftMod)) == 0 and
		    (keyboard_b->modsDepressed() & (1 << shiftMod)) == 0);

	/* Create another surface with a dummy display so that we can
	 * be sure this client doesn't have focus */
	DummyClient dummyClient;

	/* Give the old surface pointer focus but not keyboard focus */
	movePointer(5, 5);
	{
		Pointer pointer(seat_);
		YIELD_UNTIL(!keyboard_a->hasFocus(surface_) and
			    pointer.hasFocus(surface_));
	}

	/* We should still be able to see the modifiers even though we
	 * don't have a pointer resource and the surface doesn't have
	 * keyboard focus */
	inputKeySend(KEY_LEFTSHIFT, 1);
	YIELD_UNTIL((keyboard_a->modsDepressed() & (1 << shiftMod)) and
		    (keyboard_b->modsDepressed() & (1 << shiftMod)));
	inputKeySend(KEY_LEFTSHIFT, 0);
	YIELD_UNTIL((keyboard_a->modsDepressed() & (1 << shiftMod)) == 0 and
		    (keyboard_b->modsDepressed() & (1 << shiftMod)) == 0);

	delete keyboard_b;

	inputKeySend(KEY_A, 1);
	yield();

	/* This client doesn't have focus so we shouldn't be able to
	 * see the key press */
	YIELD_UNTIL(!keyboard_a->keyPressed(KEY_A));

	/* Give the surface focus by clicking on it */
	inputKeySend(BTN_LEFT, 1);
	yield();
	inputKeySend(BTN_LEFT, 0);
	yield();

	/* The surface should now be told about the keypresses in the
	 * enter event */
	YIELD_UNTIL(keyboard_a->hasFocus(surface_) and
		    keyboard_a->keyPressed(KEY_A));

	inputKeySend(KEY_LEFTSHIFT, 1);
	YIELD_UNTIL(keyboard_a->keyPressed(KEY_LEFTSHIFT) and
		    (keyboard_a->modsDepressed() & (1 << shiftMod)));

	/* If we later create another keyboard resource it should be
	 * told about the modifiers and key presses on creation */
	keyboard_b = new Keyboard(seat_);
	YIELD_UNTIL(keyboard_b->hasFocus(surface_) and
		    keyboard_b->keyPressed(KEY_A) and
		    keyboard_b->keyPressed(KEY_LEFTSHIFT) and
		    (keyboard_b->modsDepressed() & (1 << shiftMod)));

	inputKeySend(KEY_A, 0);
	inputKeySend(KEY_LEFTSHIFT, 0);

	delete keyboard_a;
	delete keyboard_b;
}

WFITS_CORE_HARNESS_TEST_CASE(MultiResourceTest);

} // namespace input
} // namespace core
} // namespace test
} // namespace wfits
