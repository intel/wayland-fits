#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"
#include "elmtestharness.h"

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


BOOST_AUTO_TEST_SUITE(Wayland_EFL_Window_Suite)

	WAYLAND_ELM_HARNESS_TEST_CASE(WindowResizeTest)
	WAYLAND_ELM_HARNESS_TEST_CASE(WindowMoveTest)
	WAYLAND_ELM_HARNESS_TEST_CASE(WindowIconifyTest)
	WAYLAND_ELM_HARNESS_TEST_CASE(WindowMaximizeTest)

BOOST_AUTO_TEST_SUITE_END()
