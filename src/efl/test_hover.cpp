#include <vector>
#include <string>

#include "background.h"
#include "templates.h"

using std::vector;
using std::string;

class Hover : public EvasObject
{
public:
	Hover(EvasObject &parent)
		: EvasObject::EvasObject(elm_hover_add(parent))
	{
		return;
	}
};

class HoverParentTargetTest : public ElmTestHarness
{
public:
	HoverParentTargetTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("HoverTargetTest", "Entry Emoticon Test")
		, control_(window_)
		, button_(elm_button_add(window_))
	{
		button_.setPosition(200, 100);
		button_.setSize(200, 100);
	}

	void setup()
	{
		window_.show();
		button_.show();

		queueStep(boost::bind(elm_hover_parent_set, boost::ref(control_), boost::ref(window_)));
		queueStep(boost::bind(&HoverParentTargetTest::checkParent, boost::ref(*this), boost::ref(window_)));

		queueStep(boost::bind(elm_hover_target_set, boost::ref(control_), boost::ref(button_)));
		queueStep(boost::bind(&HoverParentTargetTest::checkTarget, boost::ref(*this), boost::ref(button_)));
	}

	void checkTarget(const Evas_Object *expected)
	{
		FAIL_UNLESS_EQUAL(elm_hover_target_get(control_), expected);
	}

	void checkParent(const Evas_Object *expected)
	{
		FAIL_UNLESS_EQUAL(elm_hover_parent_get(control_), expected);
	}

private:
	Window		window_;
	Hover		control_;
	EvasObject	button_;
};

typedef ResizeObjectTest<Hover> HoverResizeTest;
typedef PositionObjectTest<Hover> HoverPositionTest;
typedef VisibleObjectTest<Hover> HoverVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(HoverResizeTest, "Hover")
WAYLAND_ELM_HARNESS_TEST_CASE(HoverPositionTest, "Hover")
WAYLAND_ELM_HARNESS_TEST_CASE(HoverVisibilityTest, "Hover")
WAYLAND_ELM_HARNESS_TEST_CASE(HoverParentTargetTest, "Hover")

