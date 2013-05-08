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

#include <string>
#include <linux/input.h>

#include "window.h"
#include "elmtestharness.h"
#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class Check : public EvasObject
{
public:
	Check(EvasObject &parent)
		: EvasObject::EvasObject(elm_check_add(parent))
	{
		return;
	}
};

class CheckStateTest : public ElmTestHarness
{
public:

	CheckStateTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("CheckStateTest", "Check State Test")
		, check_(elm_check_add(window_))
	{
		evas_object_resize(check_, 200, 100);
	}

	void setup()
	{
		check_.show();
		window_.show();

		queueStep(boost::bind(elm_check_state_set, boost::ref(check_), EINA_FALSE));
		queueStep(boost::bind(&CheckStateTest::checkPos, boost::ref(*this), EINA_FALSE));

		queueStep(boost::bind(elm_check_state_set, boost::ref(check_), EINA_TRUE));
		queueStep(boost::bind(&CheckStateTest::checkPos, boost::ref(*this), EINA_TRUE));
	}

	void checkPos(Eina_Bool checked)
	{
		FAIL_UNLESS_EQUAL(elm_check_state_get(check_), checked);
	}

private:
	Window		window_;
	EvasObject	check_;
};

template <const std::string& part, const std::string& text>
class CheckPartTextTest : public ElmTestHarness
{
public:
	CheckPartTextTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("CheckTextTest", "Check Text Test")
		, check_(elm_check_add(window_))
	{
		evas_object_resize(check_, 200, 100);
	}

	void setup()
	{
		check_.show();
		window_.show();

		queueStep(boost::bind(elm_object_part_text_set, boost::ref(check_), part.c_str(), text.c_str()));
		queueStep(boost::bind(&CheckPartTextTest::checkPartText, boost::ref(*this)));
	}

	void checkPartText()
	{
		const char* actual = elm_object_part_text_get(check_, part.c_str());
		std::string actual_text(actual == NULL ? "" : actual);

		FAIL_UNLESS_EQUAL(actual_text, text);
	}

private:
	Window		window_;
	EvasObject	check_;
};

class CheckUserStateTest : public ElmTestHarness
{
public:

	CheckUserStateTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("CheckUserStateTest", "Check User State Test")
		, check_(elm_check_add(window_))
		, changed_(false)
		, rendered_(false)
	{
		evas_object_resize(check_, 200, 100);
		elm_object_text_set(check_, "Test Check");
		evas_object_move(check_, 0, 0);
		evas_object_smart_callback_add(check_, "changed", onChanged, this);
	}

	void setup()
	{
		evas_event_callback_add(evas_object_evas_get(window_), EVAS_CALLBACK_RENDER_POST, onPostRender, this);

		check_.show();
		window_.show();

		queueStep(
			boost::bind(
				&ElmTestHarness::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&CheckUserStateTest::rendered_, boost::ref(*this))
			)
		);

		queueStep(boost::bind(&CheckUserStateTest::clickCheck, boost::ref(*this)));
		queueStep(
			boost::bind(
				&ElmTestHarness::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&CheckUserStateTest::changed_, boost::ref(*this))
			),
			"Testing changed_ event"
		);
		queueStep(
			boost::bind(
				&CheckUserStateTest::testCheckState,
				boost::ref(*this),
				EINA_TRUE
			),
			"Testing state == EINA_TRUE"
		);

		queueStep(boost::bind(&CheckUserStateTest::setChangedFalse, boost::ref(*this)));
		queueStep(boost::bind(&CheckUserStateTest::clickCheck, boost::ref(*this)));
		queueStep(
			boost::bind(
				&ElmTestHarness::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&CheckUserStateTest::changed_, boost::ref(*this))
			),
			"Testing changed_ event"
		);
		queueStep(
			boost::bind(
				&CheckUserStateTest::testCheckState,
				boost::ref(*this),
				EINA_FALSE
			),
			"Testing state == EINA_FALSE"
		);
	}

	void clickCheck()
	{
		Application::yield(0.01*1e6);

		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gc(check_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gc.x + gc.width / 2,
			gw.y + gf.y + gc.y + gc.height / 2
		);

		ASSERT(changed_ == false);

		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
	}

	static void onChanged(void* data, Evas_Object*, void*)
	{
		CheckUserStateTest *test = static_cast<CheckUserStateTest*>(data);
		test->changed_ = true;
		std::cout << "...received changed event" << std::endl;
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		CheckUserStateTest *test = static_cast<CheckUserStateTest*>(data);
		test->rendered_ = true;
		std::cout << "...got post render event" << std::endl;
	}

	void testCheckState(Eina_Bool checked)
	{
		FAIL_UNLESS_EQUAL(elm_check_state_get(check_), checked);
	}

	void setChangedFalse(){
		changed_ = false;
	}

private:
	Window		window_;
	EvasObject	check_;

	bool		changed_;
	bool		rendered_;
};


std::string defaultPart("default"), defaultText("DEFAULT");


typedef CheckPartTextTest<defaultPart, defaultText> CheckPartTextDefaultTest;
typedef ResizeObjectTest<Check> CheckResizeTest;
typedef PositionObjectTest<Check> CheckPositionTest;
typedef VisibleObjectTest<Check> CheckVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(CheckResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(CheckPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(CheckVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(CheckStateTest)
WFITS_EFL_HARNESS_TEST_CASE(CheckPartTextDefaultTest)
WFITS_EFL_HARNESS_TEST_CASE(CheckUserStateTest)

// NOTE: The following tests fail on all engines:
// see http://trac.enlightenment.org/e/ticket/2285
// They are not Wayland specific enough, so we probably don't care to
// test them.
//
// std::string onPart("on"), onText("ON");
// std::string offPart("off"), offText("OFF");
// typedef CheckPartTextTest<onPart, onText> CheckPartTextOnTest;
// typedef CheckPartTextTest<offPart, offText> CheckPartTextOffTest;
// WFITS_EFL_HARNESS_TEST_CASE(CheckPartTextOnTest)
// WFITS_EFL_HARNESS_TEST_CASE(CheckPartTextOffTest)

} // namespace efl
} // namespace test
} // namespace wfits
