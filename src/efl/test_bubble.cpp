#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"
#include "evasobject.h"
#include "elmtestharness.h"

class BubblePosTest : public ElmTestHarness
{
public:

	BubblePosTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BubblePosTest", "Bubble Position Test")
		, bubble_(elm_bubble_add(window_))
	{
		evas_object_resize(bubble_, 200, 100);
		return;
	}

	void setup()
	{
		window_.show();
		evas_object_show(bubble_);

		Elm_Bubble_Pos pos[] = { ELM_BUBBLE_POS_TOP_LEFT,
					 ELM_BUBBLE_POS_TOP_RIGHT,
					 ELM_BUBBLE_POS_TOP_LEFT,
					 ELM_BUBBLE_POS_BOTTOM_LEFT,
					 ELM_BUBBLE_POS_TOP_LEFT,
					 ELM_BUBBLE_POS_BOTTOM_RIGHT,
					 ELM_BUBBLE_POS_TOP_LEFT
				      };

		for (int i = 0; i < (sizeof(pos) / sizeof(int)); ++i)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&elm_bubble_pos_set, boost::ref(bubble_), pos[i]),
					boost::bind(&BubblePosTest::checkPos, boost::ref(*this), pos[i])
				)
			);
		}
	}

	void checkPos(Elm_Bubble_Pos pos)
	{
		BOOST_CHECK_EQUAL(elm_bubble_pos_get(bubble_), pos);
	}

private:
	Window		window_;
	EvasObject	bubble_;
};

class BubbleTextTest : public ElmTestHarness
{
public:

	BubbleTextTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BubbleTextTest", "Bubble Text Test")
		, bubble_(elm_bubble_add(window_))
	{
		evas_object_resize(bubble_, 200, 100);
		return;
	}

	void setup()
	{
		evas_object_show(bubble_);
		window_.show();

		std::string def[2] = { "default", "DEFAULT" };

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_object_part_text_set, boost::ref(bubble_), def[0].c_str(), def[1].c_str()),
				boost::bind(&BubbleTextTest::checkText, boost::ref(*this), def[0], def[1])
			)
		);
	}

	void checkText(const std::string& part, const std::string& text)
	{
		const char* raw = elm_object_part_text_get(bubble_, part.c_str());
		std::string control(raw == NULL ? "" : raw);

		BOOST_CHECK_EQUAL(control, text);
	}

private:
	Window		window_;
	EvasObject	bubble_;
};

BOOST_AUTO_TEST_SUITE(Wayland_EFL_Suite)

	BOOST_AUTO_TEST_SUITE(Wayland_EFL_Bubble_Suite)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(BubblePosTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(BubbleTextTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
