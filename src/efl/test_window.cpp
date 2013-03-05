#include <Elementary.h>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

boost::lambda::placeholder1_type _1_;
boost::lambda::placeholder2_type _2_;
boost::lambda::placeholder3_type _3_;

#include <vector>

#include "window.h"
#include "elmtestharness.h"

using std::vector;

class WindowResizeTest : public ElmTestHarness
{
public:
	WindowResizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowResizeTest", "Window Resize Test")
		, resizeDone_(false)
	{
		return;
	}

	void setup()
	{
		window_.show();

		// This will flush out the window titlebar animation.
		// However, this is highly dependent on the default animation
		// and if that changes, then this may not work.  But currently
		// 20 yields seems to do the trick.
		for (unsigned i(0); i < 20; ++i)
			queueStep(boost::bind(&Application::yield, 0.001*1e6));

		addResizeTest(-10, -10);
		addResizeTest(-1, 10);
		addResizeTest(10, -1);
		addResizeTest(0, 0);

		for (int w(1); w <= 400; w += 89)
			for (int h(3); h <= 400; h += 91)
				addResizeTest(w, h);

		addResizeTest(3000, 3000);

		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);
	}

	void addResizeTest(int width, int height)
	{
		queueStep(
			boost::lambda::bind(
				&WindowResizeTest::resizeDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::bind(
				&Window::setSize,
				boost::ref(window_),
				width,
				height
			),
			boost::str(
				boost::format("resizing to %1% x %2%") % width % height
			)
		);
		queueStep(
			boost::bind(
				&WindowResizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowResizeTest::resizeDone_, boost::ref(*this))
			),
			"checking resize event"
		);
		queueStep(
			boost::bind(
				&WindowResizeTest::assertCondition,
				boost::ref(*this),
				boost::lambda::bind(&Window::getWidth, boost::ref(window_)) == std::max(1, width)
				and boost::lambda::bind(&Window::getHeight, boost::ref(window_)) == std::max(1, height)
			),
			"checking client size"
		);
		queueStep(
			boost::bind(
				&WindowResizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowResizeTest::serverSizeIsEqual, boost::ref(*this))
			),
			"checking server size"
		);
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowResizeTest *test = static_cast<WindowResizeTest*>(data);
		test->resizeDone_ = true;
	}

	bool serverSizeIsEqual()
	{
		Geometry geometry(getSurfaceGeometry(window_.get_wl_surface()));
		
		return window_.getWidth() == geometry.width
			and window_.getHeight() == geometry.height;
	}

private:
	Window	window_;
	bool	resizeDone_;
};

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

class WindowIconifyTest : public ElmTestHarness
{
public:
	WindowIconifyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIconifyTest", "Window Iconify/Minimize Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::iconify, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkIconified, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::iconify, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkIconified, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
};

class WindowMaximizeTest : public ElmTestHarness
{
public:
	WindowMaximizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowMaximizeTest", "Window Maximize Test")
		, geometry_()
		, isMaximized_(false)
		, configureDone_(false)
		, resizeDone_(false)
	{
		return;
	}

	void setup()
	{
		window_.show();
		Application::yield(0.01*1e6);
		geometry_ = getSurfaceGeometry(window_.get_wl_surface());
		std::cout << "...initial server geometry is: "
			<< geometry_.x << " "
			<< geometry_.y << " "
			<< geometry_.width << " "
			<< geometry_.height << std::endl;

		evas_object_smart_callback_add(window_, "maximized", onMaximize, this);
		evas_object_smart_callback_add(window_, "unmaximized", onUnMaximize, this);
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);
		ecore_event_handler_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, onConfigure, this);

		for(unsigned i(0); i < 5; ++i)
			addMaximizeTest();
	}

	void addMaximizeTest()
	{
		// Maximize
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::isMaximized_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::configureDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::resizeDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::bind(
				&Window::maximize, boost::ref(window_),
				EINA_TRUE
			),
			"maximizing window"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::isMaximized_, boost::ref(*this))
			),
			"checking maximized event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::configureDone_, boost::ref(*this))
			),
			"checking configure event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::resizeDone_, boost::ref(*this))
			),
			"checking resize event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&Window::getWidth, boost::ref(window_)) > geometry_.width
				and boost::lambda::bind(&Window::getHeight, boost::ref(window_)) > geometry_.height
			),
			"checking client size > initial server size"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::serverSizeIsEqual, boost::ref(*this))
			),
			"checking client size == server size"
		);

		// UnMaximize
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::configureDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::resizeDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::bind(
				&Window::maximize, boost::ref(window_),
				EINA_FALSE
			),
			"unmaximizing window"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::configureDone_, boost::ref(*this))
			),
			"checking configure event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				not boost::lambda::bind(&WindowMaximizeTest::isMaximized_, boost::ref(*this))
			),
			"checking unmaximized event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::resizeDone_, boost::ref(*this))
			),
			"checking resize event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&Window::getWidth, boost::ref(window_)) == geometry_.width
				and boost::lambda::bind(&Window::getHeight, boost::ref(window_)) == geometry_.height
			),
			"checking client size == initial server size"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::serverSizeIsEqual, boost::ref(*this))
			),
			"checking client size == server size"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::serverGeometryIsInitial,
				boost::ref(*this)
			),
			"checking server geometry == initial server geometry"
		);
	}

	bool serverSizeIsEqual()
	{
		Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return window_.getWidth() == g.width
			and window_.getHeight() == g.height;
	}

	bool serverGeometryIsInitial()
	{
		Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return geometry_.x == g.x
			and geometry_.y == g.y
			and geometry_.width == g.width
			and geometry_.height == g.height;
	}

	static Eina_Bool onConfigure(void *data, int type, void *event)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->configureDone_ = true;
		Ecore_Wl_Event_Window_Configure *ev = static_cast<Ecore_Wl_Event_Window_Configure *>(event);

		std::cout << "...got configure event: "
			<< ev->x << " "
			<< ev->y << " "
			<< ev->w << " "
			<< ev->h << std::endl;
		return ECORE_CALLBACK_PASS_ON;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->resizeDone_ = true;
	}

	static void onMaximize(void* data, Evas_Object *obj, void* event_info)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = EINA_TRUE;
	}

	static void onUnMaximize(void *data, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = EINA_FALSE;
	}

private:
	Window		window_;
	Geometry	geometry_;
	bool		isMaximized_;
	bool		configureDone_;
	bool		resizeDone_;
};

class WindowFullscreenTest : public ElmTestHarness
{
public:
	WindowFullscreenTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowFullscreenTest", "Window Fullscreen Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::fullscreen, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkFullscreen, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::fullscreen, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkFullscreen, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
};

class WindowStickyTest : public ElmTestHarness
{
public:
	WindowStickyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowStickyTest", "Window Sticky Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::sticky, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkSticky, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::sticky, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkSticky, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
};

class WindowWithdrawnTest : public ElmTestHarness
{
public:
	WindowWithdrawnTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowWithdrawnTest", "Window Withdrawn Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::withdrawn, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkWithdrawn, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::withdrawn, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkWithdrawn, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
};

class WindowRotationTest : public ElmTestHarness
{
public:
	WindowRotationTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowRotationTest", "Window Rotation Test")
	{
		degrees_.push_back(90);
		degrees_.push_back(180);
		degrees_.push_back(270);
		degrees_.push_back(360);
		degrees_.push_back(0);
		degrees_.push_back(90);
		degrees_.push_back(0);
		degrees_.push_back(180);
		degrees_.push_back(0);
		degrees_.push_back(270);
		degrees_.push_back(0);
		degrees_.push_back(360);
	}

	void setup()
	{
		window_.show();

		foreach (const int degree, degrees_) {
			queueStep(boost::bind(&Window::rotate, boost::ref(window_), degree));
			queueStep(boost::bind(&Window::checkRotation, boost::ref(window_), degree));
		}
	}

private:
	Window		window_;
	vector<int>	degrees_;
};

class WindowAlphaTest : public ElmTestHarness
{
public:
	WindowAlphaTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowAlphaTest", "Window Alpha Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(elm_win_alpha_set, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&WindowAlphaTest::checkAlpha, boost::ref(*this), EINA_TRUE));
		queueStep(boost::bind(elm_win_alpha_set, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&WindowAlphaTest::checkAlpha, boost::ref(*this), EINA_FALSE));
		queueStep(boost::bind(elm_win_alpha_set, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&WindowAlphaTest::checkAlpha, boost::ref(*this), EINA_TRUE));
	}

	void checkAlpha(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_win_alpha_get(window_), expected);
	}

private:
	Window	window_;
};

class WindowIndicatorTest : public ElmTestHarness
{
public:
	WindowIndicatorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIndicatorTest", "Window Indicator Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(elm_win_indicator_mode_set, boost::ref(window_), ELM_WIN_INDICATOR_SHOW));
		queueStep(boost::bind(&WindowIndicatorTest::checkIndicator, boost::ref(*this), ELM_WIN_INDICATOR_SHOW));
		queueStep(boost::bind(elm_win_indicator_mode_set, boost::ref(window_), ELM_WIN_INDICATOR_HIDE));
		queueStep(boost::bind(&WindowIndicatorTest::checkIndicator, boost::ref(*this), ELM_WIN_INDICATOR_HIDE));
		queueStep(boost::bind(elm_win_indicator_mode_set, boost::ref(window_), ELM_WIN_INDICATOR_SHOW));
		queueStep(boost::bind(&WindowIndicatorTest::checkIndicator, boost::ref(*this), ELM_WIN_INDICATOR_SHOW));
	}

	void checkIndicator(const Elm_Win_Indicator_Mode expected)
	{
		FAIL_UNLESS_EQUAL(elm_win_indicator_mode_get(window_), expected);
	}

private:
	Window	window_;
};

class WindowIndicatorOpacityTest : public ElmTestHarness
{
public:
	WindowIndicatorOpacityTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIndicatorOpacityTest", "Window Indicator Opacity Test")
	{
		modes_.push_back(ELM_WIN_INDICATOR_OPAQUE);
		modes_.push_back(ELM_WIN_INDICATOR_TRANSLUCENT);
		modes_.push_back(ELM_WIN_INDICATOR_TRANSPARENT);
	}

	void setup()
	{
		window_.show();

		typedef vector<Elm_Win_Indicator_Opacity_Mode>::const_iterator CIterator;
		const CIterator endIt(modes_.end());
		for (CIterator it(modes_.begin()); it != endIt; ++it) {
			queueStep(boost::bind(elm_win_indicator_opacity_set, boost::ref(window_), *it));
			queueStep(boost::bind(&WindowIndicatorOpacityTest::checkOpacity, boost::ref(*this), *it));
			queueStep(boost::bind(elm_win_indicator_opacity_set, boost::ref(window_), ELM_WIN_INDICATOR_OPAQUE));
			queueStep(boost::bind(&WindowIndicatorOpacityTest::checkOpacity, boost::ref(*this), ELM_WIN_INDICATOR_OPAQUE));
		}
	}

	void checkOpacity(const Elm_Win_Indicator_Opacity_Mode expected)
	{
		FAIL_UNLESS_EQUAL(elm_win_indicator_opacity_get(window_), expected);
	}

private:
	Window					window_;
	vector<Elm_Win_Indicator_Opacity_Mode>	modes_;
};

class EcoreWlWindowTest : public ElmTestHarness
{
public:
	EcoreWlWindowTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EcoreWlWindowTest", "EcoreWlWindowTest")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&EcoreWlWindowTest::check, boost::ref(*this)));
	}

	void check()
	{
		ASSERT(elm_win_wl_window_get(window_) != NULL);
	}

private:
	Window	window_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(WindowResizeTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowMoveTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowIconifyTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowMaximizeTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowFullscreenTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowStickyTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowWithdrawnTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowRotationTest, "Window")
//TODO: These three below need work still
WAYLAND_ELM_HARNESS_TEST_CASE(WindowAlphaTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowIndicatorTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowIndicatorOpacityTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(EcoreWlWindowTest, "Window")
