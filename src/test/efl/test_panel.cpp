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

static std::string toString(Elm_Panel_Orient orient)
{
	switch(orient)
	{
		case ELM_PANEL_ORIENT_TOP:
			return "TOP";
		case ELM_PANEL_ORIENT_BOTTOM:
			return "BOTTOM";
		case ELM_PANEL_ORIENT_LEFT:
			return "LEFT";
		case ELM_PANEL_ORIENT_RIGHT:
			return "RIGHT";
		default:
			return "UNKNOWN";
	}
}

class PanelOrientTest : public ElmTestHarness
{
public:
	PanelOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PanelOrientTest", "Panel Orientation Test")
		, control_(window_)
		, content_(elm_label_add(window_))
	{
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
		elm_object_text_set(content_, "Panel");
		elm_object_content_set(control_, content_);

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		window_.show();
		content_.show();
		control_.show();
	}

	void test()
	{
		foreach (const Elm_Panel_Orient orient, orients_)
		{
			TEST_LOG("setting orient = " << toString(orient));
			synchronized(
				boost::bind(&elm_panel_orient_set, boost::ref(control_), orient)
			);

			TEST_LOG("checking orient attribute == " << toString(orient));
			FAIL_UNLESS_EQUAL(
				Application::synchronizedResult(
					[this]()->Elm_Panel_Orient {
						return elm_panel_orient_get(control_);
					}
				), orient
			);
		}
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
		return;
	}

	void setup()
	{
		elm_object_text_set(content_, "Panel");
		elm_object_content_set(control_, content_);

		window_.setSize(250, 125);
		control_.setSize(200, 100);
		control_.setPosition(25, 10);
		window_.show();
		content_.show();
		control_.show();
	}

	void test()
	{
		checkHidden(EINA_FALSE);

		TEST_LOG("toggling panel");
		synchronized(
			boost::bind(&elm_panel_toggle, boost::ref(control_))
		);

		checkHidden(EINA_TRUE);

		TEST_LOG("toggling panel");
		synchronized(
			boost::bind(&elm_panel_toggle, boost::ref(control_))
		);

		checkHidden(EINA_FALSE);
	}

	void checkHidden(Eina_Bool hidden)
	{
		TEST_LOG("checking panel hidden == " << bool(hidden));
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Eina_Bool {
					return elm_panel_hidden_get(control_);
				}
			), hidden
		);
	}

private:
	Window		window_;
	Panel		control_;
	EvasObject	content_;
};

typedef ResizeObjectTest<Panel> PanelResizeTest;
typedef PositionObjectTest<Panel> PanelPositionTest;
typedef VisibleObjectTest<Panel> PanelVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(PanelResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(PanelPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(PanelVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(PanelOrientTest)
WFITS_EFL_HARNESS_TEST_CASE(PanelToggleTest)

} // namespace efl
} // namespace test
} // namespace wfits
