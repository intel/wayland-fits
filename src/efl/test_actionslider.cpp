#include <vector>

#include "templates.h"

using std::vector;

class Actionslider : public EvasObject
{
public:
	Actionslider(EvasObject &parent)
		: EvasObject::EvasObject(elm_actionslider_add(parent))
	{
		return;
	}
};

class ActionsliderIndicatorTest : public ElmTestHarness
{
public:
	ActionsliderIndicatorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ActionsliderIndicatorTest", "Actionslider Position Test")
		, control_(window_)
	{
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_RIGHT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);
		positions_.push_back(ELM_ACTIONSLIDER_RIGHT);
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_RIGHT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);
	}

	void setup()
	{
		window_.show();
		control_.show();

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		foreach (Elm_Actionslider_Pos p, positions_) {
			queueStep(boost::bind(elm_actionslider_indicator_pos_set, boost::ref(control_), p));
			queueStep(boost::bind(&ActionsliderIndicatorTest::checkPos, boost::ref(*this), p));
		}
	}

	void checkPos(const Elm_Actionslider_Pos expected)
	{
		FAIL_UNLESS_EQUAL(elm_actionslider_indicator_pos_get(control_), expected);
	}

private:
	Window				window_;
	Actionslider			control_;
	vector<Elm_Actionslider_Pos>	positions_;
};

typedef ResizeObjectTest<Actionslider> ActionsliderResizeTest;
typedef PositionObjectTest<Actionslider> ActionsliderPositionTest;
typedef VisibleObjectTest<Actionslider> ActionsliderVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderResizeTest, "ActionSlider")
WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderPositionTest, "ActionSlider")
WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderVisibilityTest, "ActionSlider")
WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderIndicatorTest, "ActionSlider")

