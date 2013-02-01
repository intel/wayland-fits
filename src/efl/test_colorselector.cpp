#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"

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
	}

	void setup()
	{
		selector_.show();
		window_.show();

		queueStep(boost::bind(elm_colorselector_color_set, boost::ref(selector_), 64, 64, 64, 0));
		queueStep(boost::bind(&ColorselectorColorTest::checkColor, boost::ref(*this), 64, 64, 64, 0));
	}

	void checkColor(const int r, const int g, const int b, const int a)
	{
		int red = 0, green = 0, blue = 0, alpha = 0;
		elm_colorselector_color_get(selector_, &red, &green, &blue, &alpha);

		FAIL_UNLESS(r == red && g == green && b == blue && a == alpha);
	}

private:
	Window		window_;
	EvasObject	selector_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(ColorselectorColorTest, "ColorSelector")
