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

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class Panel : public EvasObject
{
public:
	Panel(EvasObject &parent)
		: EvasObject::EvasObject(elm_panel_add(parent))
	{
		return;
	}
};

class PanelOrientTest : public ElmTestHarness
{
public:
	PanelOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PanelOrientTest", "Panel Orientation Test")
		, control_(window_)
		, content_(elm_label_add(window_))
	{
		elm_object_text_set(content_, "Panel");
		elm_object_content_set(control_, content_);

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		orients_.push_back(ELM_PANEL_ORIENT_TOP);
		orients_.push_back(ELM_PANEL_ORIENT_BOTTOM);
		orients_.push_back(ELM_PANEL_ORIENT_TOP);
		orients_.push_back(ELM_PANEL_ORIENT_LEFT);
		orients_.push_back(ELM_PANEL_ORIENT_TOP);
		orients_.push_back(ELM_PANEL_ORIENT_RIGHT);
		orients_.push_back(ELM_PANEL_ORIENT_TOP);
	}

	void setup()
	{
		window_.show();
		content_.show();
		control_.show();

		foreach (const Elm_Panel_Orient orient, orients_) {
			queueStep(boost::bind(elm_panel_orient_set, boost::ref(control_), orient));
			queueStep(boost::bind(&PanelOrientTest::checkOrient, boost::ref(*this), orient));
		}
	}

	void checkOrient(const Elm_Panel_Orient expected)
	{
		control_.show();
		FAIL_UNLESS_EQUAL(elm_panel_orient_get(control_), expected);
		Application::yield();
	}

private:
	Window				window_;
	Panel				control_;
	EvasObject			content_;
	std::vector<Elm_Panel_Orient>	orients_;
};

// TODO - PanelToggleTest - needs callbacks
class PanelToggleTest : public ElmTestHarness
{
public:
	PanelToggleTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PanelToggleTest", "Panel Toggle Test")
		, control_(window_)
		, content_(elm_label_add(window_))
	{
		elm_object_text_set(content_, "Panel");
		elm_object_content_set(control_, content_);

		control_.setSize(200, 100);
		control_.setPosition(50, 10);
	}

	void setup()
	{
		window_.show();
		content_.show();
		control_.show();

		queueStep(boost::bind(elm_panel_toggle, boost::ref(control_)));
		queueStep(boost::bind(&EvasObject::checkVisible, boost::ref(control_), EINA_FALSE));

		queueStep(boost::bind(&PanelToggleTest::yield, boost::ref(*this)));
		queueStep(boost::bind(&PanelToggleTest::yield, boost::ref(*this)));

		queueStep(boost::bind(elm_panel_toggle, boost::ref(control_)));
		queueStep(boost::bind(&EvasObject::checkVisible, boost::ref(control_), EINA_TRUE));
	}

	void yield(void)
	{
		Application::yield();
	}

private:
	Window				window_;
	Panel				control_;
	EvasObject			content_;
};

typedef ResizeObjectTest<Panel> PanelResizeTest;
typedef PositionObjectTest<Panel> PanelPositionTest;
typedef VisibleObjectTest<Panel> PanelVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(PanelResizeTest, "Panel")
WAYLAND_ELM_HARNESS_TEST_CASE(PanelPositionTest, "Panel")
WAYLAND_ELM_HARNESS_TEST_CASE(PanelVisibilityTest, "Panel")
WAYLAND_ELM_HARNESS_TEST_CASE(PanelOrientTest, "Panel")
//WAYLAND_ELM_HARNESS_TEST_CASE(PanelToggleTest, "Panel")

} // namespace efl
} // namespace test
} // namespace wfits
