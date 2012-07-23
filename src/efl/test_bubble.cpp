#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>
#include <string>

#include "window.h"

#include "elmtestharness.h"
#include "templates.h"

using std::vector;
using std::string;

// Basic suite START
EFL_RESIZE_TEST(Bubble, bubble, 300, 200, 200, 100)
EFL_POSITION_TEST(Bubble, bubble, 100, 50, 150, 100)
EFL_VISIBILITY_TEST(Bubble, bubble)
// Basic suite END

class BubbleCornerTest : public ElmTestHarness
{
public:

	BubbleCornerTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BubbleCornerTest", "Bubble Corner Test")
		, bubble_(elm_bubble_add(window_))
	{
		positions_.push_back(ELM_BUBBLE_POS_TOP_LEFT);
		positions_.push_back(ELM_BUBBLE_POS_TOP_RIGHT);
		positions_.push_back(ELM_BUBBLE_POS_TOP_LEFT);
		positions_.push_back(ELM_BUBBLE_POS_BOTTOM_LEFT);
		positions_.push_back(ELM_BUBBLE_POS_TOP_LEFT);
		positions_.push_back(ELM_BUBBLE_POS_BOTTOM_RIGHT);
		positions_.push_back(ELM_BUBBLE_POS_TOP_LEFT);

		return;
	}

	void setup()
	{
		window_.show();
		bubble_.show();

		bubble_.setSize(200, 100);

		vector<Elm_Bubble_Pos>::iterator it;
		for (it = positions_.begin(); it != positions_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&elm_bubble_pos_set, boost::ref(bubble_), *it),
					boost::bind(&BubbleCornerTest::checkPos, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkPos(const Elm_Bubble_Pos expected)
	{
		BOOST_CHECK_EQUAL(elm_bubble_pos_get(bubble_), expected);
	}

private:
	Window			window_;
	EvasObject		bubble_;
	vector<Elm_Bubble_Pos>	positions_;
};

class BubbleTextTest : public ElmTestHarness
{
public:

	BubbleTextTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BubbleTextTest", "Bubble Text Test")
		, bubble_(elm_bubble_add(window_))
	{
		sentinels_.push_back("default");
		sentinels_.push_back("DEFAULT");

		return;
	}

	void setup()
	{
		bubble_.show();
		window_.show();

		bubble_.setSize(200, 100);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_object_part_text_set, boost::ref(bubble_), sentinels_[0].c_str(), sentinels_[1].c_str()),
				boost::bind(&BubbleTextTest::checkText, boost::ref(*this), sentinels_[0], sentinels_[1])
			)
		);
	}

	void checkText(const string& part, const std::string& expected)
	{
		const char* actual_raw = elm_object_part_text_get(bubble_, part.c_str());
		string actual(actual_raw == NULL ? "" : actual_raw);

		BOOST_CHECK_EQUAL(actual, expected);
	}

private:
	Window		window_;
	EvasObject	bubble_;
	vector<string>	sentinels_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Bubble)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(BubbleCornerTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(BubbleTextTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
