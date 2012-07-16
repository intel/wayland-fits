#include <Elementary.h>
#include <boost/bind.hpp>

#include "application.h"
#include "window.h"
#include "background.h"
#include "evasobject.h"
#include "elmtestharness.h"

using boost::filesystem::path;

class BackgroundColorTest : public ElmTestHarness
{
public:

	BackgroundColorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BackgroundColorTest", "Background Color Test")
		, bg_(window_)
	{
		evas_object_size_hint_weight_set(bg_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, bg_);
		return;
	}

	void setup()
	{
		bg_.show();
		window_.show();

		int r, g, b;
		for (r = 0; r < 256; r += 64)
			for (g = 0; g < 256; g += 64)
				for (b = 0; b < 256; b += 64)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&Background::setColor, boost::ref(bg_), r, g, b),
					boost::bind(&BackgroundColorTest::checkColor, boost::ref(*this), r, g, b)
				)
			);
		}
	}

	void checkColor(int r, int g, int b)
	{
		int r_, g_, b_;
		bg_.getColor(&r_, &g_, &b_);

		BOOST_CHECK(r_ == r && g_ == g && b_ == b);
	}

private:
	Window		window_;
	Background	bg_;
};

class BackgroundImageTest : public ElmTestHarness
{
public:

	BackgroundImageTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BackgroundImageTest", "Background Image Test")
		, bg_(window_)
	{
		evas_object_size_hint_weight_set(bg_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, bg_);
		return;
	}

	void setup()
	{
		bg_.show();
		window_.show();

		path p(MEDIA_PATH"/bridge_of_the_gods.png");

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&Background::setImage, boost::ref(bg_), p),
				boost::bind(&BackgroundImageTest::checkImage, boost::ref(*this), p)
				)
			);

		Elm_Bg_Option option[] = { ELM_BG_OPTION_SCALE,
					   ELM_BG_OPTION_CENTER,
					   ELM_BG_OPTION_STRETCH,
					   ELM_BG_OPTION_CENTER,
					   ELM_BG_OPTION_TILE,
					   ELM_BG_OPTION_CENTER
					 };

		window_.maximize(EINA_TRUE);

		unsigned int o;
		for (o = 0; o < (sizeof(option) / sizeof(Elm_Bg_Option)); o++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&Background::setImageOpt, boost::ref(bg_), option[o]),
					boost::bind(&BackgroundImageTest::checkImageOpt, boost::ref(*this), option[o])
				)
			);
		}
	}

	void checkImage(path& p)
	{
		path ret;
		bg_.getImage(ret);

		BOOST_CHECK_EQUAL(ret, p);
	}

	void checkImageOpt(Elm_Bg_Option option)
	{
		BOOST_CHECK_EQUAL(option, bg_.getImageOpt());
	}

private:
	Window		window_;
	Background	bg_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Background)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(BackgroundColorTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(BackgroundImageTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

