#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"
#include "evasobject.h"
#include "elmtestharness.h"
#include "templates.h"

// Basic suite START
EFL_RESIZE_TEST(Check, check, 300, 200, 200, 100)
EFL_POSITION_TEST(Check, check, 100, 50, 150, 100)
EFL_VISIBILITY_TEST(Check, check)
// Basic suite END

class CheckStateTest : public ElmTestHarness
{
public:

	CheckStateTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("CheckStateTest", "Check State Test")
		, check_(elm_check_add(window_))
	{
		evas_object_resize(check_, 200, 100);
		return;
	}

	void setup()
	{
		check_.show();
		window_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_check_state_set, boost::ref(check_), EINA_FALSE),
				boost::bind(&CheckStateTest::checkPos, boost::ref(*this), EINA_FALSE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_check_state_set, boost::ref(check_), EINA_TRUE),
				boost::bind(&CheckStateTest::checkPos, boost::ref(*this), EINA_TRUE)
			)
		);
	}

	void checkPos(Eina_Bool checked)
	{
		BOOST_CHECK_EQUAL(elm_check_state_get(check_), checked);
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
		return;
	}

	void setup()
	{
		check_.show();
		window_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_object_part_text_set, boost::ref(check_), "default", "DEFAULT"),
				boost::bind(&CheckTextTest::checkText, boost::ref(*this), "default", "DEFAULT")
			)
		);

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

		BOOST_CHECK_EQUAL(actual_text, expected_text);
	}

private:
	Window		window_;
	EvasObject	check_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Check)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(CheckStateTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(CheckTextTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

