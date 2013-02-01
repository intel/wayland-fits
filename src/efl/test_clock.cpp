#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"

#include "elmtestharness.h"
#include "templates.h"

class Clock : public EvasObject
{
public:
	Clock(EvasObject &parent)
		: EvasObject::EvasObject(elm_clock_add(parent))
	{
		return;
	}
};

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
	}

	void setup()
	{
		clock_.show();
		window_.show();

		queueStep(boost::bind(elm_clock_show_am_pm_set, boost::ref(clock_), EINA_FALSE));
		queueStep(boost::bind(&ClockTimeTest::checkAmPm, boost::ref(*this), EINA_FALSE));

		queueStep(boost::bind(elm_clock_show_am_pm_set, boost::ref(clock_), EINA_TRUE));
		queueStep(boost::bind(&ClockTimeTest::checkAmPm, boost::ref(*this), EINA_TRUE));

		queueStep(boost::bind(elm_clock_show_seconds_set, boost::ref(clock_), EINA_FALSE));
		queueStep(boost::bind(&ClockTimeTest::checkSecs, boost::ref(*this), EINA_FALSE));

		queueStep(boost::bind(elm_clock_show_seconds_set, boost::ref(clock_), EINA_TRUE));
		queueStep(boost::bind(&ClockTimeTest::checkSecs, boost::ref(*this), EINA_TRUE));

		queueStep(boost::bind(elm_clock_time_set, boost::ref(clock_), 6, 42, 12));
		queueStep(boost::bind(&ClockTimeTest::checkTime, boost::ref(*this), 6, 42, 12));
	}

	void checkAmPm(Eina_Bool ampm)
	{
		FAIL_UNLESS_EQUAL(elm_clock_show_am_pm_get(clock_), ampm);
	}

	void checkSecs(Eina_Bool secs)
	{
		FAIL_UNLESS_EQUAL(elm_clock_show_seconds_get(clock_), secs);
	}

	void checkTime(int h, int m, int s)
	{
		int hr, min, sec;
		elm_clock_time_get(clock_, &hr, &min, &sec);
		FAIL_UNLESS(h == hr && m == min && s == sec);
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
	}

	void setup()
	{
		clock_.show();
		window_.show();

		queueStep(boost::bind(elm_clock_edit_set, boost::ref(clock_), EINA_TRUE));
		queueStep(boost::bind(&ClockEditionTest::checkEdit, boost::ref(*this), EINA_TRUE));

		queueStep(boost::bind(elm_clock_edit_set, boost::ref(clock_), EINA_FALSE));
		queueStep(boost::bind(&ClockEditionTest::checkEdit, boost::ref(*this), EINA_FALSE));
	}

	void checkEdit(Eina_Bool edit)
	{
		FAIL_UNLESS_EQUAL(elm_clock_edit_get(clock_), edit);
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
			ELM_CLOCK_EDIT_SEC_UNIT
		};

		Elm_Clock_Edit_Mode mask = ELM_CLOCK_EDIT_DEFAULT;
		foreach(Elm_Clock_Edit_Mode e, edition) {
			queueStep(boost::bind(elm_clock_edit_mode_set, boost::ref(clock_), e));
			queueStep(boost::bind(&ClockDigitEditionTest::checkEdit, boost::ref(*this), e));

			mask = (Elm_Clock_Edit_Mode) (mask | e);
			if (mask == e) {
				continue;
			}

			queueStep(boost::bind(elm_clock_edit_mode_set, boost::ref(clock_), mask));
			queueStep(boost::bind(&ClockDigitEditionTest::checkEdit, boost::ref(*this), mask));
		}
	}

	void checkEdit(Elm_Clock_Edit_Mode mask)
	{
		FAIL_UNLESS_EQUAL(elm_clock_edit_mode_get(clock_), mask);
	}

private:
	Window		window_;
	EvasObject	clock_;
};

typedef ResizeObjectTest<Clock> ClockResizeTest;
typedef PositionObjectTest<Clock> ClockPositionTest;
typedef VisibleObjectTest<Clock> ClockVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(ClockResizeTest, "Clock")
WAYLAND_ELM_HARNESS_TEST_CASE(ClockPositionTest, "Clock")
WAYLAND_ELM_HARNESS_TEST_CASE(ClockVisibilityTest, "Clock")
WAYLAND_ELM_HARNESS_TEST_CASE(ClockTimeTest, "Clock")
WAYLAND_ELM_HARNESS_TEST_CASE(ClockEditionTest, "Clock")
WAYLAND_ELM_HARNESS_TEST_CASE(ClockDigitEditionTest, "Clock")
