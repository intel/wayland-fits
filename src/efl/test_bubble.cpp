#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>
#include <string>

#include "window.h"

#include "elmtestharness.h"
#include "templates.h"

using std::vector;
using std::string;

class Bubble : public EvasObject
{
public:
	Bubble(EvasObject &parent)
		: EvasObject::EvasObject(elm_bubble_add(parent))
	{
		return;
	}
};

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

		foreach (Elm_Bubble_Pos p, positions_)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&elm_bubble_pos_set, boost::ref(bubble_), p),
					boost::bind(&BubbleCornerTest::checkPos, boost::ref(*this), p)
				)
			);
		}
	}

	void checkPos(const Elm_Bubble_Pos expected)
	{
		FAIL_UNLESS_EQUAL(elm_bubble_pos_get(bubble_), expected);
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

		FAIL_UNLESS_EQUAL(actual, expected);
	}

private:
	Window		window_;
	EvasObject	bubble_;
	vector<string>	sentinels_;
};

typedef ResizeObjectTest<Bubble> BubbleResizeTest;
typedef PositionObjectTest<Bubble> BubblePositionTest;
typedef VisibleObjectTest<Bubble> BubbleVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(BubbleResizeTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubblePositionTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubbleVisibilityTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubbleCornerTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubbleTextTest, "Bubble")

