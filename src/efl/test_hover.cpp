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

		return;
	}

	void setup()
	{
		window_.show();
		button_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_hover_parent_set, boost::ref(control_), boost::ref(window_)),
				boost::bind(&HoverParentTargetTest::checkParent, boost::ref(*this), boost::ref(window_))
			)
		);
	
		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_hover_target_set, boost::ref(control_), boost::ref(button_)),
				boost::bind(&HoverParentTargetTest::checkTarget, boost::ref(*this), boost::ref(button_))
			)
		);
	}

	void checkTarget(const Evas_Object *expected)
	{
		BOOST_CHECK_EQUAL(elm_hover_target_get(control_), expected);
	}

	void checkParent(const Evas_Object *expected)
	{
		BOOST_CHECK_EQUAL(elm_hover_parent_get(control_), expected);
	}

private:
	Window			window_;
	Hover			control_;
	EvasObject		button_;
};

/*
		positions_.push_back("left");
		positions_.push_back("top-left");
		positions_.push_back("top");
		positions_.push_back("top-right");
		positions_.push_back("right");
		positions_.push_back("bottom-right");
		positions_.push_back("bottom");
		positions_.push_back("bottom-left");
		positions_.push_back("middle");
		positions_.push_back("smart");
*/

/*
		styles_.push_back("hoversel_vertical");
		styles_.push_back("default");
		styles_.push_back("menu");
		styles_.push_back("default");
		styles_.push_back("popout");
		styles_.push_back("default");
*/

typedef ResizeObjectTest<Hover> HoverResizeTest;
typedef PositionObjectTest<Hover> HoverPositionTest;
typedef VisibleObjectTest<Hover> HoverVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Hover)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverVisibilityTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverParentTargetTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

