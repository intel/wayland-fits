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

		std::cout << actual_text << " " << text << std::endl;
		FAIL_UNLESS_EQUAL(actual_text, text);
	}

private:
	Window		window_;
	EvasObject	check_;
};

std::string defaultPart("default"), defaultText("DEFAULT");
std::string onPart("on"), onText("ON");
std::string offPart("off"), offText("OFF");

typedef CheckPartTextTest<defaultPart, defaultText> CheckPartTextDefaultTest;
typedef CheckPartTextTest<onPart, onText> CheckPartTextOnTest;
typedef CheckPartTextTest<offPart, offText> CheckPartTextOffTest;
typedef ResizeObjectTest<Check> CheckResizeTest;
typedef PositionObjectTest<Check> CheckPositionTest;
typedef VisibleObjectTest<Check> CheckVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(CheckResizeTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckPositionTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckVisibilityTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckStateTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckPartTextDefaultTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckPartTextOnTest, "Check")
WAYLAND_ELM_HARNESS_TEST_CASE(CheckPartTextOffTest, "Check")

