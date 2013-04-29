/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <vector>
#include <string>
#include <linux/input.h>

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
	}

	void setup()
	{
		window_.show();
		bubble_.show();

		bubble_.setSize(200, 100);

		foreach (Elm_Bubble_Pos p, positions_) {
			queueStep(boost::bind(&elm_bubble_pos_set, boost::ref(bubble_), p));
			queueStep(boost::bind(&BubbleCornerTest::checkPos, boost::ref(*this), p));
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
	}

	void setup()
	{
		bubble_.show();
		window_.show();

		bubble_.setSize(200, 100);

		queueStep(boost::bind(elm_object_part_text_set, boost::ref(bubble_), sentinels_[0].c_str(), sentinels_[1].c_str()));
		queueStep(boost::bind(&BubbleTextTest::checkText, boost::ref(*this), sentinels_[0], sentinels_[1]));
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

class BubbleUserClickTest : public ElmTestHarness
{
public:

	BubbleUserClickTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("BubbleClickTest", "Bubble Click Test")
		, bubble_(elm_bubble_add(window_))
		, clicked_(false)
	{
		evas_object_smart_callback_add(bubble_, "clicked", onClick, this);
	}

	void setup()
	{
		window_.show();
		bubble_.show();

		bubble_.setSize(200, 100);

		queueStep(boost::bind(&BubbleUserClickTest::clickBubble, boost::ref(*this)));
		queueStep(
			boost::bind(
				&ElmTestHarness::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&BubbleUserClickTest::clicked_, boost::ref(*this))
			),
			"checking clicked_ event"
		);
	}

	void clickBubble()
	{
		Application::yield(0.01*1e6);

		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gb(bubble_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gb.x + gb.width / 2,
			gw.y + gf.y + gb.y + gb.height / 2
		);

		ASSERT(clicked_ == false);

		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
	}

	static void onClick(void* data, Evas_Object*, void*)
	{
		BubbleUserClickTest *test = static_cast<BubbleUserClickTest*>(data);
		test->clicked_ = true;
		std::cout << "...received click event" << std::endl;
	}

private:
	Window		window_;
	EvasObject	bubble_;
	bool		clicked_;
};

typedef ResizeObjectTest<Bubble> BubbleResizeTest;
typedef PositionObjectTest<Bubble> BubblePositionTest;
typedef VisibleObjectTest<Bubble> BubbleVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(BubbleResizeTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubblePositionTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubbleVisibilityTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubbleCornerTest, "Bubble")
WAYLAND_ELM_HARNESS_TEST_CASE(BubbleTextTest, "Bubble")

WAYLAND_ELM_HARNESS_TEST_CASE(BubbleUserClickTest, "Bubble")

