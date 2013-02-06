#include <Elementary.h>
#include <boost/bind.hpp>

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
		, sizes_()
		, resizeDone_(false)
	{
		return;
	}

	void setup()
	{
		window_.show();

		sizes_.push_back(Size(-10, -10));
		sizes_.push_back(Size(-1, 10));
		sizes_.push_back(Size(10, -1));
		sizes_.push_back(Size(0, 0));

		for (int w(1); w <= 400; w += 89)
			for (int h(3); h <= 400; h += 91)
				sizes_.push_back(Size(w, h));

		sizes_.push_back(Size(3000, 3000));

		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);

		nextResize();
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowResizeTest *test = static_cast<WindowResizeTest*>(data);
		test->resizeDone_ = true;
	}

	void nextResize() {
		resizeDone_ = false;
		if (not sizes_.empty()) {
			Size size(sizes_.front());
			sizes_.pop_front();
			queueStep(boost::bind(&Window::setSize, boost::ref(window_), size.first, size.second));
			queueStep(boost::bind(&WindowResizeTest::checkResize, boost::ref(*this), size.first, size.second, 20));
		}
	}

	void checkResize(int w, int h, unsigned tries)
	{
		if (not resizeDone_) {
			ASSERT(tries != 0);
			queueStep(boost::bind(&WindowResizeTest::checkResize, boost::ref(*this), w, h, --tries));
		} else {
			window_.checkSize(std::max(1, w), std::max(1, h));
			checkServerSize(Geometry(), 20);
		}
	}

	void checkServerSize(Geometry geometry, unsigned tries) {
		bool sizeMatch(
			window_.getWidth() == geometry.width
			and window_.getHeight() == geometry.height);

		if (not sizeMatch) {
			ASSERT(tries != 0);
			GeometryCallback cb = boost::bind(&WindowResizeTest::checkServerSize, boost::ref(*this), _1, --tries);
			getSurfaceGeometry(elm_win_wl_window_get(window_)->surface, cb);
		} else {
			FAIL_UNLESS(sizeMatch);
			nextResize();
		}
	}

private:
	typedef std::pair<int, int> Size;
	typedef std::deque<Size> Sizes;

	Window	window_;
	Sizes	sizes_;
	bool	resizeDone_;
};

class WindowMoveTest : public ElmTestHarness
{
public:
	WindowMoveTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowMoveTest", "Window Move Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::setPosition, boost::ref(window_), 10, 20));
		queueStep(boost::bind(&Window::checkPosition, boost::ref(window_), 10, 20));
		queueStep(boost::bind(&Window::setPosition, boost::ref(window_), 15, 25));
		queueStep(boost::bind(&Window::checkPosition, boost::ref(window_), 15, 25));
	}

private:
	Window	window_;
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
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::maximize, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkMaximized, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::maximize, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkMaximized, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
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
