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

#include <boost/filesystem.hpp>

#include <vector>

#include "background.h"
#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

using boost::filesystem::path;
using std::vector;

// TODO: Once we instrument events, we can test the _interaction functions

class Flip : public EvasObject
{
public:
	Flip(EvasObject &parent)
		: EvasObject::EvasObject(elm_flip_add(parent))
	{
		return;
	}
};

class FlipGoTest : public ElmTestHarness
{
public:
	FlipGoTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FlipGoTest", "Flip Go Test")
		, control_(elm_flip_add(window_))
		, first_(window_)
		, second_(window_)
		, options_()
		, flipDone_(false)
		, nWaitForFlip_(0)
	{
		options_.push_back(ELM_FLIP_ROTATE_Y_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_Y_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_X_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_XZ_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_YZ_CENTER_AXIS);
		options_.push_back(ELM_FLIP_CUBE_LEFT);
		options_.push_back(ELM_FLIP_CUBE_RIGHT);
		options_.push_back(ELM_FLIP_CUBE_UP);
		options_.push_back(ELM_FLIP_CUBE_DOWN);
		options_.push_back(ELM_FLIP_PAGE_LEFT);
		options_.push_back(ELM_FLIP_PAGE_RIGHT);
		options_.push_back(ELM_FLIP_PAGE_UP);
		options_.push_back(ELM_FLIP_PAGE_DOWN);
	}

	void setup()
	{
		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		first_.setColor(255, 0, 0);
		second_.setColor(0, 255, 0);

		evas_object_size_hint_align_set(first_, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_size_hint_weight_set(first_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(second_, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_size_hint_weight_set(second_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		elm_object_part_content_set(control_, "front", first_);
		elm_object_part_content_set(control_, "back", second_);

		evas_object_smart_callback_add(control_, "animate,done", animateDone, this);

		control_.show();
		first_.show();
		second_.show();
		window_.show();
	}

	void test()
	{
		// The front is initially visible
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Eina_Bool {
					return elm_flip_front_visible_get(control_);
				}
			), EINA_TRUE
		);

		foreach (Elm_Flip_Mode mode, options_)
		{
			flipDone_ = false;
			synchronized(boost::bind(elm_flip_go, boost::ref(control_), mode));
			YIELD_UNTIL(flipDone_);

			FAIL_UNLESS_EQUAL(
				Application::synchronizedResult(
					[this]()->Eina_Bool {
						return elm_flip_front_visible_get(control_);
					}
				), EINA_FALSE
			);

			flipDone_ = false;
			synchronized(boost::bind(elm_flip_go, boost::ref(control_), mode));
			YIELD_UNTIL(flipDone_);

			FAIL_UNLESS_EQUAL(
				Application::synchronizedResult(
					[this]()->Eina_Bool {
						return elm_flip_front_visible_get(control_);
					}
				), EINA_TRUE
			);
		}
	}

	static void animateDone(void* data, Evas_Object *obj, void* event_info)
	{
		FlipGoTest* fgt = static_cast<FlipGoTest*>(data);
		fgt->flipDone_ = true;
	}
private:
	Window				window_;
	EvasObject			control_;
	Background			first_, second_;
	std::deque<Elm_Flip_Mode>	options_;
	bool				flipDone_;
	unsigned			nWaitForFlip_;
};

typedef ResizeObjectTest<Flip> FlipResizeTest;
typedef PositionObjectTest<Flip> FlipPositionTest;
typedef VisibleObjectTest<Flip> FlipVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(FlipGoTest)
WFITS_EFL_HARNESS_TEST_CASE(FlipResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(FlipPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(FlipVisibilityTest)

} // namespace efl
} // namespace test
} // namespace wfits
