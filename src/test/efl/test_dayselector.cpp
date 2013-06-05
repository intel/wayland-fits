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
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include <string>
#include <vector>


#include "window.h"
#include "elmtestharness.h"
#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

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

class DayselectorUserTest : public ElmTestHarness
{
public:
	DayselectorUserTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("DayselectorUserTest", "Dayselector User Test", 350, 120)
		, dayselector_(elm_dayselector_add(window_))
		, rendered_(false)
		, changed_(false)
	{
		return;
	}

	void setup()
	{
		evas_object_size_hint_weight_set(dayselector_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, dayselector_);

		evas_event_callback_add(
			evas_object_evas_get(window_),
			EVAS_CALLBACK_RENDER_POST,
			onPostRender, this
		);

		evas_object_smart_callback_add(dayselector_, "dayselector,changed", onDayselectorChanged, this);

		window_.show();
		dayselector_.show();

		queueStep(boost::bind(&DayselectorUserTest::test, boost::ref(*this)));
	}

	void test(){
		YIELD_UNTIL(rendered_);

		for (unsigned i(0); i <= 6; ++i) {
			// ELM_DAYSELECTOR_SUN=0; dayStr = day6
			const std::string dayStr("day" + boost::lexical_cast<std::string>(i));

			changed_ = false;
			clickDay(dayStr);
			YIELD_UNTIL(changed_);

			const Eina_Bool result = elm_dayselector_day_selected_get(
				dayselector_,
				static_cast<Elm_Dayselector_Day>( (i + 1) % 7)
			);
			FAIL_IF(result == EINA_FALSE);
		}

		//Test all are clicked.
		testAllSelected();
	}

	void clickDay(const std::string &str)
	{
		EvasObject currentDay(elm_layout_content_get(dayselector_, str.c_str()), false);
		Geometry gc(currentDay.getGeometry());
		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gb(dayselector_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gc.x + gc.width / 2,
			gw.y + gf.y + gc.y + gc.height / 2
		);

		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
	}

	void testAllSelected()
	{
		for (unsigned i(0); i <= 6; ++i) {
			const Elm_Dayselector_Day day(
				static_cast<Elm_Dayselector_Day>(i)
			);
			const Eina_Bool result(
				elm_dayselector_day_selected_get(dayselector_, day)
			);
			FAIL_IF(result == EINA_FALSE);
		}
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		 evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		 DayselectorUserTest *test = static_cast<DayselectorUserTest*>(data);
		 test->rendered_ = true;
		 std::cout << "...received post render event" << std::endl;
	}

	static void onDayselectorChanged(void *data, Evas_Object *eo, void *info)
	{
		 DayselectorUserTest *test = static_cast<DayselectorUserTest*>(data);
		 test->changed_ = true;
		 std::cout << "...received changed event" << std::endl;
	}

private:
	Window		window_;
	EvasObject	dayselector_;
	bool		rendered_;
	bool		changed_;
};

typedef ResizeObjectTest<Dayselector> DayselectorResizeTest;
typedef PositionObjectTest<Dayselector> DayselectorPositionTest;
typedef VisibleObjectTest<Dayselector> DayselectorVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(DayselectorResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(DayselectorPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(DayselectorVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(DayselectorDayTest)
WFITS_EFL_HARNESS_TEST_CASE(DayselectorLocaleTest)

WFITS_EFL_HARNESS_TEST_CASE(DayselectorUserTest)
} // namespace efl
} // namespace test
} // namespace wfits
