#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"

#include "elmtestharness.h"
#include "templates.h"

using std::vector;

class Dayselector : public EvasObject
{
public:
	Dayselector(EvasObject &parent)
		: EvasObject::EvasObject(elm_dayselector_add(parent))
	{
		return;
	}
};

class DayselectorDayTest : public ElmTestHarness
{
public:

	DayselectorDayTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("DayselectorDayTest", "Dayselector Day Test")
		, control_(elm_dayselector_add(window_))
	{
		window_.setSize(500, 400);
		evas_object_size_hint_weight_set(control_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, control_);

		days_.push_back(ELM_DAYSELECTOR_SUN);
		days_.push_back(ELM_DAYSELECTOR_MON);
		days_.push_back(ELM_DAYSELECTOR_TUE);
		days_.push_back(ELM_DAYSELECTOR_WED);
		days_.push_back(ELM_DAYSELECTOR_THU);
		days_.push_back(ELM_DAYSELECTOR_FRI);
		days_.push_back(ELM_DAYSELECTOR_SAT);
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Elm_Dayselector_Day d, days_) {
			queueStep(boost::bind(elm_dayselector_day_selected_set, boost::ref(control_), d, EINA_TRUE));
			queueStep(boost::bind(&DayselectorDayTest::checkDay, boost::ref(*this), d, EINA_TRUE));

			queueStep(boost::bind(elm_dayselector_day_selected_set, boost::ref(control_), d, EINA_FALSE));
			queueStep(boost::bind(&DayselectorDayTest::checkDay, boost::ref(*this), d, EINA_FALSE));

			queueStep(boost::bind(elm_dayselector_day_selected_set, boost::ref(control_), d, EINA_TRUE));
			queueStep(boost::bind(&DayselectorDayTest::checkDay, boost::ref(*this), d, EINA_TRUE));
		}
	}

	void checkDay(Elm_Dayselector_Day day, Eina_Bool selected)
	{
		FAIL_UNLESS_EQUAL(elm_dayselector_day_selected_get(control_, day), selected);
	}

private:
	Window				window_;
	EvasObject			control_;
	vector<Elm_Dayselector_Day>	days_;
};

class DayselectorLocaleTest : public ElmTestHarness
{
public:

	DayselectorLocaleTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("DayselectorLocaleTest", "Dayselector Locale Test")
		, control_(elm_dayselector_add(window_))
	{
		window_.setSize(500, 400);
		evas_object_size_hint_weight_set(control_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, control_);
	}

	void setup()
	{
		window_.show();
		control_.show();

		// toggle the first day of the week
		const Elm_Dayselector_Day weekstart = elm_dayselector_week_start_get(control_);

		queueStep(boost::bind(elm_dayselector_week_start_set, boost::ref(control_), ELM_DAYSELECTOR_WED));
		queueStep(boost::bind(&DayselectorLocaleTest::checkStart, boost::ref(*this), ELM_DAYSELECTOR_WED));

		queueStep(boost::bind(elm_dayselector_week_start_set, boost::ref(control_), weekstart));
		queueStep(boost::bind(&DayselectorLocaleTest::checkStart, boost::ref(*this), weekstart));

		// Toggle the length of the weekend
		const int len = elm_dayselector_weekend_length_get(control_);

		queueStep(boost::bind(elm_dayselector_weekend_length_set, boost::ref(control_), 7));
		queueStep(boost::bind(&DayselectorLocaleTest::checkLength, boost::ref(*this), 7));

		queueStep(boost::bind(elm_dayselector_weekend_length_set, boost::ref(control_), len));
		queueStep(boost::bind(&DayselectorLocaleTest::checkLength, boost::ref(*this), len));
	}

	void checkStart(Elm_Dayselector_Day weekstart)
	{
		FAIL_UNLESS_EQUAL(elm_dayselector_week_start_get(control_), weekstart);
	}

	void checkLength(const int length)
	{
		FAIL_UNLESS_EQUAL(elm_dayselector_weekend_length_get(control_), length);
	}

private:
	Window		window_;
	EvasObject	control_;
};

typedef ResizeObjectTest<Dayselector> DayselectorResizeTest;
typedef PositionObjectTest<Dayselector> DayselectorPositionTest;
typedef VisibleObjectTest<Dayselector> DayselectorVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(DayselectorResizeTest, "DaySelector")
WAYLAND_ELM_HARNESS_TEST_CASE(DayselectorPositionTest, "DaySelector")
WAYLAND_ELM_HARNESS_TEST_CASE(DayselectorVisibilityTest, "DaySelector")
WAYLAND_ELM_HARNESS_TEST_CASE(DayselectorDayTest, "DaySelector")
WAYLAND_ELM_HARNESS_TEST_CASE(DayselectorLocaleTest, "DaySelector")

