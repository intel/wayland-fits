#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "application.h"
#include "window.h"
#include "background.h"

#include "elmtestharness.h"

using boost::filesystem::path;
using std::vector;

class BackgroundColorTest : public ElmTestHarness
{
public:

	BackgroundColorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BackgroundColorTest", "Background Color Test")
		, control_(window_)
	{
		evas_object_size_hint_weight_set(control_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, control_);
	}

	void setup()
	{
		control_.show();
		window_.show();

		for (int r(0); r < 256; r += 64) {
			for (int g(0); g < 256; g += 64) {
				for (int b(0); b < 256; b += 64) {
					queueStep(boost::bind(&Background::setColor, boost::ref(control_), r, g, b));
					queueStep(boost::bind(&BackgroundColorTest::checkColor, boost::ref(*this), r, g, b));
				}
			}
		}
	}

	void checkColor(int r_expected, int g_expected, int b_expected)
	{
		int r_actual, g_actual, b_actual;
		control_.getColor(&r_actual, &g_actual, &b_actual);

		FAIL_UNLESS_EQUAL(r_actual, r_expected);
		FAIL_UNLESS_EQUAL(g_actual, g_expected);
		FAIL_UNLESS_EQUAL(b_actual, b_expected);
	}

private:
	Window		window_;
	Background	control_;
};

class BackgroundImageTest : public ElmTestHarness
{
public:

	BackgroundImageTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BackgroundImageTest", "Background Image Test")
		, control_(window_)
	{
		evas_object_size_hint_weight_set(control_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, control_);
		window_.maximize(EINA_TRUE);

		options_.push_back(ELM_BG_OPTION_SCALE);
		options_.push_back(ELM_BG_OPTION_CENTER);
		options_.push_back(ELM_BG_OPTION_STRETCH);
		options_.push_back(ELM_BG_OPTION_CENTER);
		options_.push_back(ELM_BG_OPTION_TILE);
		options_.push_back(ELM_BG_OPTION_CENTER);
	}

	void setup()
	{
		control_.show();
		window_.show();

		path p(MEDIA_PATH"/bridge_of_the_gods.png");

		queueStep(boost::bind(&Background::setImage, boost::ref(control_), p));
		queueStep(boost::bind(&BackgroundImageTest::checkImage, boost::ref(*this), p));

		queueStep(boost::bind(&Window::maximize, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&BackgroundImageTest::checkMax, boost::ref(*this), EINA_TRUE));

		foreach (Elm_Bg_Option o, options_) {
			queueStep(boost::bind(&Background::setImageOpt, boost::ref(control_), o));
			queueStep(boost::bind(&BackgroundImageTest::checkImageOpt, boost::ref(*this), o));
		}
	}

	void checkMax(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(window_.isMaximized(), expected);
	}

	void checkImage(path& expected)
	{
		path actual;
		control_.getImage(actual);

		FAIL_UNLESS_EQUAL(actual, expected);
	}

	void checkImageOpt(Elm_Bg_Option expected)
	{
		FAIL_UNLESS_EQUAL(control_.getImageOpt(), expected);
	}

private:
	Window			window_;
	Background		control_;
	vector<Elm_Bg_Option>	options_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(BackgroundColorTest, "Background")
WAYLAND_ELM_HARNESS_TEST_CASE(BackgroundImageTest, "Background")

