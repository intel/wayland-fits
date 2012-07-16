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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&EvasObject::setSize, boost::ref(button_), 75, 75),
				boost::bind(&ButtonResizeTest::checkSize, boost::ref(*this), 75, 75)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&EvasObject::setSize, boost::ref(button_), 120, 30),
				boost::bind(&ButtonResizeTest::checkSize, boost::ref(*this), 120, 30)
			)
		);
	}

	void checkSize(unsigned w, unsigned h)
	{
		BOOST_CHECK_EQUAL(button_.getWidth(), w);
		BOOST_CHECK_EQUAL(button_.getHeight(), h);
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

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&EvasObject::setPosition, boost::ref(button_), 60, 15),
				boost::bind(&ButtonMoveTest::checkPosition, boost::ref(*this), 60, 15)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&EvasObject::setPosition, boost::ref(button_), 10, 10),
				boost::bind(&ButtonMoveTest::checkPosition, boost::ref(*this), 10, 10)
			)
		);
	}

	void checkPosition(unsigned x, unsigned y)
	{
		BOOST_CHECK_EQUAL(button_.getX(), x);
		BOOST_CHECK_EQUAL(button_.getY(), y);
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
