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

namespace wfits {
namespace test {
namespace efl {

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
	}

	void test()
	{
		foreach (Elm_Bubble_Pos p, positions_) {
			synchronized(boost::bind(&elm_bubble_pos_set, boost::ref(bubble_), p));
			synchronized(boost::bind(&BubbleCornerTest::checkPos, boost::ref(*this), p));
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
	}

	void test()
	{
		synchronized(boost::bind(elm_object_part_text_set, boost::ref(bubble_), sentinels_[0].c_str(), sentinels_[1].c_str()));
		synchronized(boost::bind(&BubbleTextTest::checkText, boost::ref(*this), sentinels_[0], sentinels_[1]));
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
		, rendered_(false)
	{
		return;
	}

	void setup()
	{
		evas_object_smart_callback_add(bubble_, "clicked", onClick, this);
		evas_event_callback_add(evas_object_evas_get(window_), EVAS_CALLBACK_RENDER_POST, onPostRender, this);

		bubble_.setSize(200, 100);
		window_.show();
		bubble_.show();
	}

	void test()
	{
		YIELD_UNTIL(rendered_);

		TEST_LOG("clicking the bubble");
		clicked_ = false;
		clickBubble();

		TEST_LOG("checking for clicked event");
		YIELD_UNTIL(clicked_);
	}

	void clickBubble()
	{
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
		TEST_LOG("received click event");
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		BubbleUserClickTest *test = static_cast<BubbleUserClickTest*>(data);
		test->rendered_ = true;
		TEST_LOG("got post render event");
	}

private:
	Window		window_;
	EvasObject	bubble_;
	bool		clicked_;
	bool		rendered_;
};

typedef ResizeObjectTest<Bubble> BubbleResizeTest;
typedef PositionObjectTest<Bubble> BubblePositionTest;
typedef VisibleObjectTest<Bubble> BubbleVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(BubbleResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(BubblePositionTest)
WFITS_EFL_HARNESS_TEST_CASE(BubbleVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(BubbleCornerTest)
WFITS_EFL_HARNESS_TEST_CASE(BubbleTextTest)
WFITS_EFL_HARNESS_TEST_CASE(BubbleUserClickTest)

} // namespace efl
} // namespace test
} // namespace wfits
