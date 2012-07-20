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
				boost::bind(&WindowResizeTest::checkSize, boost::ref(*this), 400, 200)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::setSize, boost::ref(window_), 100, 120),
				boost::bind(&WindowResizeTest::checkSize, boost::ref(*this), 100, 120)
			)
		);
	}

	void checkSize(unsigned w, unsigned h)
	{
		BOOST_CHECK_EQUAL(window_.getWidth(), w);
		BOOST_CHECK_EQUAL(window_.getHeight(), h);
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
				boost::bind(&WindowMoveTest::checkPosition, boost::ref(*this), 10, 20)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::setPosition, boost::ref(window_), 15, 25),
				boost::bind(&WindowMoveTest::checkPosition, boost::ref(*this), 15, 25)
			)
		);
	}

	void checkPosition(unsigned x, unsigned y)
	{
		BOOST_CHECK_EQUAL(window_.getX(), x);
		BOOST_CHECK_EQUAL(window_.getY(), y);
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
				boost::bind(&WindowIconifyTest::checkIconified, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::iconify, boost::ref(window_), EINA_FALSE),
				boost::bind(&WindowIconifyTest::checkIconified, boost::ref(*this), EINA_FALSE)
			)
		);
	}

	void checkIconified(Eina_Bool isIconified)
	{
		BOOST_CHECK_EQUAL(window_.isIconified(), isIconified);
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
				boost::bind(&WindowMaximizeTest::checkMaximized, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::maximize, boost::ref(window_), EINA_FALSE),
				boost::bind(&WindowMaximizeTest::checkMaximized, boost::ref(*this), EINA_FALSE)
			)
		);
	}

	void checkMaximized(Eina_Bool isMaximized)
	{
		BOOST_CHECK_EQUAL(window_.isMaximized(), isMaximized);
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
				boost::bind(&WindowFullscreenTest::checkFullscreen, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::fullscreen, boost::ref(window_), EINA_FALSE),
				boost::bind(&WindowFullscreenTest::checkFullscreen, boost::ref(*this), EINA_FALSE)
			)
		);
	}

	void checkFullscreen(Eina_Bool isFullscreen)
	{
		BOOST_CHECK_EQUAL(window_.isFullscreen(), isFullscreen);
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
				boost::bind(&WindowStickyTest::checkSticky, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::sticky, boost::ref(window_), EINA_FALSE),
				boost::bind(&WindowStickyTest::checkSticky, boost::ref(*this), EINA_FALSE)
			)
		);
	}

	void checkSticky(Eina_Bool isSticky)
	{
		BOOST_CHECK_EQUAL(window_.isSticky(), isSticky);
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
				boost::bind(&WindowWithdrawnTest::checkWithdrawn, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Window::withdrawn, boost::ref(window_), EINA_FALSE),
				boost::bind(&WindowWithdrawnTest::checkWithdrawn, boost::ref(*this), EINA_FALSE)
			)
		);
	}

	void checkWithdrawn(Eina_Bool isWithdrawn)
	{
		BOOST_CHECK_EQUAL(window_.isWithdrawn(), isWithdrawn);
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
					boost::bind(&WindowRotationTest::checkRotation, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkRotation(const int expected)
	{
		BOOST_CHECK_EQUAL(window_.getRotation(), expected);
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

