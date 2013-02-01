#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"

#include "elmtestharness.h"
#include "templates.h"

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

class CheckTextTest : public ElmTestHarness
{
public:

	CheckTextTest()
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

		queueStep(boost::bind(elm_object_part_text_set, boost::ref(check_), "default", "DEFAULT"));
		queueStep(boost::bind(&CheckTextTest::checkText, boost::ref(*this), "default", "DEFAULT"));

		/* TODO: Why don't these work?
		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_object_part_text_set, boost::ref(check_), "on", "ON"),
				boost::bind(&CheckTextTest::checkText, boost::ref(*this), "on", "ON")
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_object_part_text_set, boost::ref(check_), "off", "OFF"),
				boost::bind(&CheckTextTest::checkText, boost::ref(*this), "off", "OFF")
			)
		);
		*/
	}

	void checkText(const char* part, const char* expected)
	{
		const char* actual = elm_object_part_text_get(check_, part);
		std::string actual_text(actual == NULL ? "" : actual);
		std::string expected_text(expected == NULL ? "" : expected);

		FAIL_UNLESS_EQUAL(actual_text, expected_text);
	}

private:
	Window		window_;
	EvasObject	check_;
};

typedef ResizeObjectTest<Check> CheckResizeTest;
typedef PositionObjectTest<Check> CheckPositionTest;
typedef VisibleObjectTest<Check> CheckVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(CheckResizeTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckPositionTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckVisibilityTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckStateTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckTextTest, "Check")

