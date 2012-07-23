#include <vector>

#include "macros.h"

using std::vector;


// Basic suite START
EFL_RESIZE_TEST(Actionslider, actionslider, 300, 200, 200, 100)
EFL_POSITION_TEST(Actionslider, actionslider, 100, 50, 150, 100)
EFL_VISIBILITY_TEST(Actionslider, actionslider)
// Basic suite END

class ActionsliderIndicatorTest : public ElmTestHarness
{
public:

	ActionsliderIndicatorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ActionsliderIndicatorTest", "Actionslider Position Test")
		, control_(elm_actionslider_add(window_))
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

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		vector<Elm_Actionslider_Pos>::iterator it;
		for (it = positions_.begin(); it != positions_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_actionslider_indicator_pos_set, boost::ref(control_), *it),
					boost::bind(&ActionsliderIndicatorTest::checkPos, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkPos(const Elm_Actionslider_Pos expected)
	{
		BOOST_CHECK_EQUAL(elm_actionslider_indicator_pos_get(control_), expected);
	}

private:
	Window				window_;
	EvasObject			control_;
	vector<Elm_Actionslider_Pos>	positions_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(ActionSlider)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderIndicatorTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
