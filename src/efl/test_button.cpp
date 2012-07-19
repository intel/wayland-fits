#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"
#include "evasobject.h"
#include "elmtestharness.h"

class ButtonResizeTest : public ElmTestHarness
{
public:

	ButtonResizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ButtonResizeTest", "Button Resize Test")
		, button_(elm_button_add(window_))
	{
		return;
	}

	void setup()
	{
		window_.show();
		button_.show();

		SET_CHECK_SIZE(button_, 75, 75);
		SET_CHECK_SIZE(button_, 120, 30);
	}

private:
	Window		window_;
	EvasObject	button_;
};

class ButtonMoveTest : public ElmTestHarness
{
public:
	ButtonMoveTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ButtonMoveTest", "Button Move Test")
		, button_(elm_button_add(window_))
	{
		return;
	}

	void setup()
	{
		window_.show();
		button_.show();

		SET_CHECK_POSITION(button_, 60, 15);
		SET_CHECK_POSITION(button_, 10, 10);
	}

private:
	Window		window_;
	EvasObject	button_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Button)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(ButtonResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ButtonMoveTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
