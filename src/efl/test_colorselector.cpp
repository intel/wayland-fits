#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"
#include "evasobject.h"
#include "elmtestharness.h"

class ColorselectorColorTest : public ElmTestHarness
{
public:

	ColorselectorColorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ColorselectorColorTest", "Colorselector Color Test")
		, selector_(elm_colorselector_add(window_))
	{
		window_.setSize(800, 600);
		//evas_object_size_hint_weight_set(selector_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		//elm_win_resize_object_add(window_, selector_);
		return;
	}

	void setup()
	{
		selector_.show();
		window_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_colorselector_color_set, boost::ref(selector_), 64, 64, 64, 0),
				boost::bind(&ColorselectorColorTest::checkColor, boost::ref(*this), 64, 64, 64, 0)
			)
		);
	}

	void checkColor(const int r, const int g, const int b, const int a)
	{
		int red = 0, green = 0, blue = 0, alpha = 0;
		elm_colorselector_color_get(selector_, &red, &green, &blue, &alpha);

		BOOST_CHECK(r == red && g == green && b == blue && a == alpha);
	}

private:
	Window		window_;
	EvasObject	selector_;
};

/* TODO: Disabled per EFL #1153
BOOST_AUTO_TEST_SUITE(Wayland_EFL_Suite)

	BOOST_AUTO_TEST_SUITE(Wayland_EFL_Colorselector_Suite)

		WAYLAND_ELM_HARNESS_TEST_CASE(ColorselectorColorTest)

	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
*/

