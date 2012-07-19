#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"
#include "evasobject.h"
#include "elmtestharness.h"

using std::vector;

class ActionsliderPosTest : public ElmTestHarness
{
public:

	ActionsliderPosTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ActionsliderPosTest", "Actionslider Position Test")
		, control_(elm_actionslider_add(window_))
	{
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_RIGHT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);

		return;
	}

	void setup()
	{
		SET_CHECK_SHOW(window_);
		SET_CHECK_SHOW(control_);

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		vector<Elm_Actionslider_Pos>::iterator it;
		for (it = positions_.begin(); it != positions_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_actionslider_indicator_pos_set, boost::ref(control_), *it),
					boost::bind(&ActionsliderPosTest::checkPos, boost::ref(*this), *it)
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
	
		WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderPosTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
