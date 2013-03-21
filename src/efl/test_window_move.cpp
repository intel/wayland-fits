#include "window.h"
#include "elmtestharness.h"

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
		Geometry geometry(getSurfaceGeometry(elm_win_wl_window_get(window_)->surface));
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

WAYLAND_ELM_HARNESS_TEST_CASE(WindowMoveTest, "Window")
