#include <vector>

#include "window.h"
#include "elmtestharness.h"

using std::vector;

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

WAYLAND_ELM_HARNESS_TEST_CASE(WindowIconifyTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowStickyTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowWithdrawnTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowRotationTest, "Window")
//TODO: These three below need work still
WAYLAND_ELM_HARNESS_TEST_CASE(WindowAlphaTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowIndicatorTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(WindowIndicatorOpacityTest, "Window")
WAYLAND_ELM_HARNESS_TEST_CASE(EcoreWlWindowTest, "Window")
