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
#include "seat.h"
#include "surface.h"
#include "shell.h"
#include "shell_surface.h"
#include "shm.h"

namespace wfits {
namespace test {
namespace core {
namespace input {

class SurfacePointerTest : public Harness
{
public:
	SurfacePointerTest()
		: Harness::Harness()
		, compositor_(display())
		, shell_(display())
		, seat_(display())
		, pointer_(seat_)
		, surface_(compositor_)
		, shellSurface_(shell_, surface_)
		, shm_(display())
		, buffer_(shm_, 120, 75)
	{
		return;
	}

	void setup()
	{
		wl_surface_attach(surface_, buffer_, 0, 0);
		wl_surface_damage(surface_, 0, 0, buffer_.width(), buffer_.height());
		surface_.commit();

		setSurfacePosition(surface_, 100, 100);

		queueStep(boost::bind(&SurfacePointerTest::test, boost::ref(*this)));
	}

	void test()
	{
		for (int x(0); x < buffer_.width(); x+=10) {
			for (int y(0); y < buffer_.height(); y+=10) {
				movePointer(x, y);
				checkFocus(true);
				checkPointer(x, y);
			}
		}

		movePointer(-1, -1);
		checkFocus(false);

		movePointer(5, 5);
		checkFocus(true);

		inputKeySend(BTN_LEFT, 1);
		checkButton(BTN_LEFT, 1);

		inputKeySend(BTN_LEFT, 0);
		checkButton(BTN_LEFT, 0);

		inputKeySend(BTN_RIGHT, 1);
		checkButton(BTN_RIGHT, 1);

		inputKeySend(BTN_RIGHT, 0);
		checkButton(BTN_RIGHT, 0);

		inputKeySend(BTN_MIDDLE, 1);
		checkButton(BTN_MIDDLE, 1);

		inputKeySend(BTN_MIDDLE, 0);
		checkButton(BTN_MIDDLE, 0);
	}

	void movePointer(const int32_t x, const int32_t y)
	{
		Geometry geometry(getSurfaceGeometry(surface_));
		setGlobalPointerPosition(geometry.x + x, geometry.y + y);
	}

	void checkFocus(const bool focus)
	{
		YIELD_UNTIL(pointer_.hasFocus(surface_) == focus);
	}

	void checkPointer(const int32_t x, const int32_t y)
	{
		YIELD_UNTIL(pointer_.x() == x and pointer_.y() == y);
	}

	void checkButton(const uint32_t button, const uint32_t state)
	{
		YIELD_UNTIL(
			pointer_.button() == button
			and pointer_.buttonState() == state);
	}

	Compositor		compositor_;
	Shell			shell_;
	Seat			seat_;
	Pointer			pointer_;
	Surface			surface_;
	ShellSurface		shellSurface_;
	SharedMemory		shm_;
	SharedMemoryBuffer	buffer_;
};

WFITS_CORE_HARNESS_TEST_CASE(SurfacePointerTest);

} // namespace input
} // namespace core
} // namespace test
} // namespace wfits
