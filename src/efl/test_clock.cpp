#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"

#include "elmtestharness.h"
#include "macros.h"


// Basic suite START
EFL_RESIZE_TEST(Clock, clock, 300, 200, 200, 100)
EFL_POSITION_TEST(Clock, clock, 100, 50, 150, 100)
EFL_VISIBILITY_TEST(Clock, clock)
// Basic suite END

class ClockTimeTest : public ElmTestHarness
{
public:

	ClockTimeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ClockTimeTest", "Clock Time Test")
		, clock_(elm_clock_add(window_))
	{
		window_.setSize(600, 400);
		clock_.setSize(500, 100);
		clock_.setPosition(50, 50);
		return;
	}

	void setup()
	{
		clock_.show();
		window_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_clock_show_am_pm_set, boost::ref(clock_), EINA_FALSE),
				boost::bind(&ClockTimeTest::checkAmPm, boost::ref(*this), EINA_FALSE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_clock_show_am_pm_set, boost::ref(clock_), EINA_TRUE),
				boost::bind(&ClockTimeTest::checkAmPm, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_clock_show_seconds_set, boost::ref(clock_), EINA_FALSE),
				boost::bind(&ClockTimeTest::checkSecs, boost::ref(*this), EINA_FALSE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_clock_show_seconds_set, boost::ref(clock_), EINA_TRUE),
				boost::bind(&ClockTimeTest::checkSecs, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_clock_time_set, boost::ref(clock_), 6, 42, 12),
				boost::bind(&ClockTimeTest::checkTime, boost::ref(*this), 6, 42, 12)
			)
		);
		
	}

	void checkAmPm(Eina_Bool ampm)
	{
		BOOST_CHECK_EQUAL(elm_clock_show_am_pm_get(clock_), ampm);
	}

	void checkSecs(Eina_Bool secs)
	{
		BOOST_CHECK_EQUAL(elm_clock_show_seconds_get(clock_), secs);
	}

	void checkTime(int h, int m, int s)
	{
		int hr, min, sec;
		elm_clock_time_get(clock_, &hr, &min, &sec);
		BOOST_CHECK(h == hr && m == min && s == sec);
	}

private:
	Window		window_;
	EvasObject	clock_;
};

class ClockEditionTest : public ElmTestHarness
{
public:

	ClockEditionTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ClockEditionTest", "Clock Edition Test")
		, clock_(elm_clock_add(window_))
	{
		window_.setSize(600, 400);
		clock_.setSize(500, 100);
		clock_.setPosition(50, 50);
		elm_clock_show_seconds_set(clock_, EINA_TRUE);
		return;
	}

	void setup()
	{
		clock_.show();
		window_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_clock_edit_set, boost::ref(clock_), EINA_TRUE),
				boost::bind(&ClockEditionTest::checkEdit, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_clock_edit_set, boost::ref(clock_), EINA_FALSE),
				boost::bind(&ClockEditionTest::checkEdit, boost::ref(*this), EINA_FALSE)
			)
		);
	}

	void checkEdit(Eina_Bool edit)
	{
		BOOST_CHECK_EQUAL(elm_clock_edit_get(clock_), edit);
	}

private:
	Window		window_;
	EvasObject	clock_;
};

class ClockDigitEditionTest : public ElmTestHarness
{
public:

	ClockDigitEditionTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ClockDigitEditionTest", "Clock Edition Test")
		, clock_(elm_clock_add(window_))
	{
		window_.setSize(600, 400);
		clock_.setSize(500, 100);
		clock_.setPosition(50, 50);
		elm_clock_edit_set(clock_, EINA_TRUE);
		elm_clock_show_seconds_set(clock_, EINA_TRUE);
		return;
	}

	void setup()
	{
		clock_.show();
		window_.show();

		Elm_Clock_Edit_Mode edition[] = {
			ELM_CLOCK_EDIT_HOUR_DECIMAL,
			ELM_CLOCK_EDIT_HOUR_UNIT,
		 	ELM_CLOCK_EDIT_MIN_DECIMAL,
			ELM_CLOCK_EDIT_MIN_UNIT,
			ELM_CLOCK_EDIT_SEC_DECIMAL,
			ELM_CLOCK_EDIT_SEC_UNIT };

		Elm_Clock_Edit_Mode mask = ELM_CLOCK_EDIT_DEFAULT;
		for (unsigned int i = 0; i < (sizeof(edition) / sizeof(unsigned int)); ++i)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_clock_edit_mode_set, boost::ref(clock_), edition[i]),
					boost::bind(&ClockDigitEditionTest::checkEdit, boost::ref(*this), edition[i])
				)
			);

			mask = (Elm_Clock_Edit_Mode) (mask | edition[i]);
			if (mask == edition[i])
				continue;

			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_clock_edit_mode_set, boost::ref(clock_), mask),
					boost::bind(&ClockDigitEditionTest::checkEdit, boost::ref(*this), mask)
				)
			);
		}
	}

	void checkEdit(Elm_Clock_Edit_Mode mask)
	{
		BOOST_CHECK_EQUAL(elm_clock_edit_mode_get(clock_), mask);
	}

private:
	Window		window_;
	EvasObject	clock_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Clock)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(ClockTimeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ClockEditionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ClockDigitEditionTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

