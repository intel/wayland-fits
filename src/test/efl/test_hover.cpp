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

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class Hover : public EvasObject
{
public:
	Hover(EvasObject &parent)
		: EvasObject::EvasObject(elm_hover_add(parent))
	{
		return;
	}
};

class HoverParentTargetTest : public ElmTestHarness
{
public:
	HoverParentTargetTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("HoverTargetTest", "Entry Emoticon Test")
		, control_(window_)
		, button_(elm_button_add(window_))
	{
		return;
	}

	void setup()
	{
		button_.setPosition(200, 100);
		button_.setSize(200, 100);

		window_.show();
		button_.show();
	}

	void test()
	{
		synchronized(boost::bind(elm_hover_parent_set, boost::ref(control_), boost::ref(window_)));
		synchronized(boost::bind(&HoverParentTargetTest::checkParent, boost::ref(*this), boost::ref(window_)));

		synchronized(boost::bind(elm_hover_target_set, boost::ref(control_), boost::ref(button_)));
		synchronized(boost::bind(&HoverParentTargetTest::checkTarget, boost::ref(*this), boost::ref(button_)));
	}

	void checkTarget(const Evas_Object *expected)
	{
		FAIL_UNLESS_EQUAL(elm_hover_target_get(control_), expected);
	}

	void checkParent(const Evas_Object *expected)
	{
		FAIL_UNLESS_EQUAL(elm_hover_parent_get(control_), expected);
	}

private:
	Window		window_;
	Hover		control_;
	EvasObject	button_;
};

typedef ResizeObjectTest<Hover> HoverResizeTest;
typedef PositionObjectTest<Hover> HoverPositionTest;
typedef VisibleObjectTest<Hover> HoverVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(HoverResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(HoverPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(HoverVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(HoverParentTargetTest)

} // namespace efl
} // namespace test
} // namespace wfits
