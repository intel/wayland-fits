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
		, positions_()
		, moveDone_(false)
	{
		return;
	}

	void setup()
	{
		window_.show();

		positions_.push_back(Position(10, 20));
		positions_.push_back(Position(15, 25));

		evas_object_event_callback_add(window_, EVAS_CALLBACK_MOVE, &onMove, this);

		nextPosition();
	}

	static void onMove(void *data, Evas*, Evas_Object*, void*)
	{
		WindowMoveTest *test = static_cast<WindowMoveTest*>(data);
		test->moveDone_ = true;
	}

	void nextPosition() {
		moveDone_ = false;
		if (not positions_.empty()) {
			Position position(positions_.front());
			positions_.pop_front();
			queueStep(boost::bind(&Window::setPosition, boost::ref(window_), position.first, position.second));
			queueStep(boost::bind(&WindowMoveTest::checkPosition, boost::ref(*this), position.first, position.second, 20));
		}
	}

	void checkPosition(int x, int y, unsigned tries)
	{
		if (not moveDone_) {
			ASSERT_MSG(tries != 0,
				"failed to get EVAS_CALLBACK_MOVE event ("
				<< x << ","
				<< y << ")"
			);
			queueStep(boost::bind(&WindowMoveTest::checkPosition, boost::ref(*this), x, y, --tries));
		} else {
			window_.checkPosition(x, y);
			checkServerPosition(2);
		}
	}

	void checkServerPosition(unsigned tries) {
		Geometry geometry(getSurfaceGeometry(window_.get_wl_surface()));
		bool positionMatch(
			window_.getX() == geometry.x
			and window_.getY() == geometry.y);

		if (not positionMatch) {
			ASSERT_MSG(tries != 0, ""
				<< "client position ("
				<< window_.getX() << ","
				<< window_.getY() << ") != "
				<< "server position ("
				<< geometry.x << ","
				<< geometry.y << ")"
			);
			queueStep(boost::bind(&WindowMoveTest::checkServerPosition, boost::ref(*this), --tries));
		} else {
			FAIL_UNLESS(positionMatch);
			nextPosition();
		}
	}

private:
	typedef std::pair<int, int> Position;
	typedef std::deque<Position> Positions;

	Window		window_;
	Positions	positions_;
	bool		moveDone_;
};

// NOTE: This test is DISABLED for now since the wayland protocol
// does not currently support programatic window placement.
//WFITS_EFL_HARNESS_TEST_CASE(WindowMoveTest)

} // namespace efl
} // namespace test
} // namespace wfits
