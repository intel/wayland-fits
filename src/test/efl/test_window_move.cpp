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

#include "window.h"
#include "elmtestharness.h"

namespace wfits {
namespace test {
namespace efl {

class WindowMoveTest : public ElmTestHarness
{
public:
	WindowMoveTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowMoveTest", "Window Move Test")
		, moveDone_(false)
	{
		return;
	}

	void setup()
	{
		evas_object_event_callback_add(window_, EVAS_CALLBACK_MOVE, &onMove, this);

		window_.show();
	}

	void test()
	{
		for (int x(10), y(20); x < 20 and y < 30; x += 5, y += 5)
		{
			TEST_LOG("moving window to " << x << "," << y);
			moveDone_ = false;
			window_.setPosition(x, y);

			TEST_LOG("waiting for move event");
			YIELD_UNTIL(moveDone_);

			TEST_LOG("checking window position == " << x << "," << y);
			window_.checkPosition(x, y);

			TEST_LOG("checking server position == client position");
			YIELD_UNTIL(serverPositionIsEqual());
		}
	}

	static void onMove(void *data, Evas*, Evas_Object*, void*)
	{
		WindowMoveTest *test = static_cast<WindowMoveTest*>(data);
		test->moveDone_ = true;
		TEST_LOG("got move event");
	}

	bool serverPositionIsEqual()
	{
		Geometry geometry(getSurfaceGeometry(window_.get_wl_surface()));
		return window_.getX() == geometry.x
			and window_.getY() == geometry.y;
	}

private:
	Window		window_;
	bool		moveDone_;
};

// NOTE: This test is DISABLED for now since the wayland protocol
// does not currently support programatic window placement.
//WFITS_EFL_HARNESS_TEST_CASE(WindowMoveTest)

} // namespace efl
} // namespace test
} // namespace wfits
