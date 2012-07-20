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
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::setSize, boost::ref(window_), 400, 200),
				boost::bind(&Window::checkSize, boost::ref(window_), 400, 200)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::setSize, boost::ref(window_), 100, 120),
				boost::bind(&Window::checkSize, boost::ref(window_), 100, 120)
			)
		);
	}

private:
	Window		window_;
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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::setPosition, boost::ref(window_), 10, 20),
				boost::bind(&Window::checkPosition, boost::ref(window_), 10, 20)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::setPosition, boost::ref(window_), 15, 25),
				boost::bind(&Window::checkPosition, boost::ref(window_), 15, 25)
			)
		);
	}

private:
	Window		window_;
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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::iconify, boost::ref(window_), EINA_TRUE),
				boost::bind(&Window::checkIconified, boost::ref(window_), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::iconify, boost::ref(window_), EINA_FALSE),
				boost::bind(&Window::checkIconified, boost::ref(window_), EINA_FALSE)
			)
		);
	}

private:
	Window		window_;
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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::maximize, boost::ref(window_), EINA_TRUE),
				boost::bind(&Window::checkMaximized, boost::ref(window_), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::maximize, boost::ref(window_), EINA_FALSE),
				boost::bind(&Window::checkMaximized, boost::ref(window_), EINA_FALSE)
			)
		);
	}

private:
	Window		window_;
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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::fullscreen, boost::ref(window_), EINA_TRUE),
				boost::bind(&Window::checkFullscreen, boost::ref(window_), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::fullscreen, boost::ref(window_), EINA_FALSE),
				boost::bind(&Window::checkFullscreen, boost::ref(window_), EINA_FALSE)
			)
		);
	}

private:
	Window		window_;
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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::sticky, boost::ref(window_), EINA_TRUE),
				boost::bind(&Window::checkSticky, boost::ref(window_), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::sticky, boost::ref(window_), EINA_FALSE),
				boost::bind(&Window::checkSticky, boost::ref(window_), EINA_FALSE)
			)
		);
	}

private:
	Window		window_;
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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::withdrawn, boost::ref(window_), EINA_TRUE),
				boost::bind(&Window::checkWithdrawn, boost::ref(window_), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::withdrawn, boost::ref(window_), EINA_FALSE),
				boost::bind(&Window::checkWithdrawn, boost::ref(window_), EINA_FALSE)
			)
		);
	}

private:
	Window		window_;
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

		return;
	}

	void setup()
	{
		window_.show();

		vector<int>::iterator it;
		for (it = degrees_.begin(); it != degrees_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&Window::rotate, boost::ref(window_), *it),
					boost::bind(&Window::checkRotation, boost::ref(window_), *it)
				)
			);
		}
	}

private:
	Window		window_;
	vector<int>	degrees_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Window)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowMoveTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowIconifyTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowMaximizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowFullscreenTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowStickyTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowWithdrawnTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(WindowRotationTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

