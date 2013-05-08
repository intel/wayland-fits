/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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

	void checkLength(const unsigned length)
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

