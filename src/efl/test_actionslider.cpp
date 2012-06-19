#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"
#include "actionslider.h"
#include "evasobject.h"
#include "elmtestharness.h"

class ActionsliderPosTest : public ElmTestHarness
{
public:

	ActionsliderPosTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ActionsliderPosTest", "Actionslider Position Test")
		, as_(window_)
	{
		return;
	}

	void setup()
	{
		elm_win_resize_object_add(window_, as_);
		window_.show();
		as_.show();

		const Elm_Actionslider_Pos position[] = {
			 				  ELM_ACTIONSLIDER_LEFT,
							  ELM_ACTIONSLIDER_CENTER,
							  ELM_ACTIONSLIDER_RIGHT
							};

		unsigned int p;
		for (p = 0; p < (sizeof(position) / sizeof(Elm_Actionslider_Pos)); p++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&Actionslider::setPos, boost::ref(as_), position[p]),
					boost::bind(&ActionsliderPosTest::checkPos, boost::ref(*this), position[p])
				)
			);
		}
	}

	void checkPos(Elm_Actionslider_Pos pos)
	{
		BOOST_CHECK_EQUAL(as_.getPos(), pos);
	}

private:
	Window		window_;
	Actionslider	as_;
};

BOOST_AUTO_TEST_SUITE(Wayland_EFL_Actionslider_Suite)

	WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderPosTest)

BOOST_AUTO_TEST_SUITE_END()
