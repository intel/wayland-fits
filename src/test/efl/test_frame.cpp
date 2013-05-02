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

class Frame : public EvasObject
{
public:
	Frame(EvasObject &parent)
		: EvasObject::EvasObject(elm_frame_add(parent))
	{
		return;
	}
};

class FrameGoTest : public ElmTestHarness
{
public:
	FrameGoTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FrameGoTest", "Frame Go Test")
		, control_(window_)
	{
		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		FAIL_UNLESS_EQUAL(elm_frame_collapse_get(control_), EINA_FALSE);

		queueStep(boost::bind(elm_frame_collapse_go, boost::ref(control_), EINA_TRUE));
		queueStep(boost::bind(&FrameGoTest::checkCollapse, boost::ref(*this), EINA_TRUE));

		queueStep(boost::bind(elm_frame_collapse_go, boost::ref(control_), EINA_FALSE));
		queueStep(boost::bind(&FrameGoTest::checkCollapse, boost::ref(*this), EINA_FALSE));

		queueStep(boost::bind(elm_frame_collapse_go, boost::ref(control_), EINA_TRUE));
		queueStep(boost::bind(&FrameGoTest::checkCollapse, boost::ref(*this), EINA_TRUE));
	}

	void checkCollapse(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_frame_collapse_get(control_), expected);
	}

private:
	Window	window_;
	Frame	control_;
};

typedef ResizeObjectTest<Frame> FrameResizeTest;
typedef PositionObjectTest<Frame> FramePositionTest;
typedef VisibleObjectTest<Frame> FrameVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(FrameGoTest, "Frame")
WAYLAND_ELM_HARNESS_TEST_CASE(FrameResizeTest, "Frame")
WAYLAND_ELM_HARNESS_TEST_CASE(FramePositionTest, "Frame")
WAYLAND_ELM_HARNESS_TEST_CASE(FrameVisibilityTest, "Frame")

