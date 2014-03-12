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

#include <ctime>
#include <vector>

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class Popup : public EvasObject
{
public:
	Popup(EvasObject &parent)
		: EvasObject::EvasObject(elm_popup_add(parent))
	{
		return;
	}
};

class PopupTimeoutTest : public ElmTestHarness
{
public:
	PopupTimeoutTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PopupTimeoutTest", "Popup Timeout Test")
		, control_(window_)
		, timedout_(false)
		, start_(time(NULL))
		, end_(time(NULL))
	{
		return;
	}

	void setup()
	{
		elm_object_part_text_set(control_, "title,text", "Popup Title");
		elm_object_text_set(control_, "Popup Default Text");

		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		evas_object_smart_callback_add(control_, "timeout", onTimeout, this);

		window_.show();
		control_.show();
	}

	void test()
	{
		TEST_LOG("setting timeout = " << 1.0f << "s");
		synchronized(
			[this]() {
				timedout_ = false;
				start_ = time(NULL);
				end_ = time(NULL);
				elm_popup_timeout_set(control_, 1.0f);
			}
		);
		TEST_LOG("checking timeout attribute == " << 1.0f << "s");
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->double {
					return elm_popup_timeout_get(control_);
				}
			), 1.0f
		);

		TEST_LOG("checking for timeout event");
		YIELD_UNTIL(timedout_);

		TEST_LOG("checking timeout occurred around " << 1.0f << "s and no longer than " << 5.0f << "s");
		FAIL_UNLESS_GE(end_, start_ + 1);
		FAIL_UNLESS_LE(end_, start_ + 5);
	}

	static void onTimeout(void* data, Evas_Object *obj, void* event_info)
	{
		PopupTimeoutTest *t = static_cast<PopupTimeoutTest*>(data);
		t->end_ = time(NULL);
		t->timedout_ = true;
		TEST_LOG("got timeout event; it took " << (t->end_ - t->start_) << "s");
	}

private:
	Window	window_;
	Popup	control_;
	bool	timedout_;
	time_t	start_;
	time_t	end_;
};

static std::string orientToString(Elm_Popup_Orient orient)
{
	switch(orient)
	{
		case ELM_POPUP_ORIENT_TOP:
			return "TOP";
		case ELM_POPUP_ORIENT_CENTER:
			return "CENTER";
		case ELM_POPUP_ORIENT_BOTTOM:
			return "BOTTOM";
		case ELM_POPUP_ORIENT_LEFT:
			return "LEFT";
		case ELM_POPUP_ORIENT_RIGHT:
			return "RIGHT";
		case ELM_POPUP_ORIENT_TOP_LEFT:
			return "TOP_LEFT";
		case ELM_POPUP_ORIENT_TOP_RIGHT:
			return "TOP_RIGHT";
		case ELM_POPUP_ORIENT_BOTTOM_LEFT:
			return "BOTTOM_LEFT";
		case ELM_POPUP_ORIENT_BOTTOM_RIGHT:
			return "BOTTOM_RIGHT";
		default:
			return "UNKNOWN";
	}
}

// TODO: add smart callbacks for "timeout" events for each orientation
class PopupOrientTest : public ElmTestHarness
{
public:
	PopupOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PopupOrientTest", "Popup Orientation Test")
		, control_(window_)
	{
		orients_.push_back(ELM_POPUP_ORIENT_TOP);
		orients_.push_back(ELM_POPUP_ORIENT_CENTER);
		orients_.push_back(ELM_POPUP_ORIENT_BOTTOM);
		orients_.push_back(ELM_POPUP_ORIENT_LEFT);
		orients_.push_back(ELM_POPUP_ORIENT_RIGHT);
		orients_.push_back(ELM_POPUP_ORIENT_TOP_LEFT);
		orients_.push_back(ELM_POPUP_ORIENT_TOP_RIGHT);
		orients_.push_back(ELM_POPUP_ORIENT_BOTTOM_LEFT);
		orients_.push_back(ELM_POPUP_ORIENT_BOTTOM_RIGHT);
		orients_.push_back(ELM_POPUP_ORIENT_TOP);
	}

	void setup()
	{
		elm_object_part_text_set(control_, "title,text", "Popup Title");
		elm_object_text_set(control_, "Popup Default Text");

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();
	}

	void test()
	{
		foreach (const Elm_Popup_Orient orient, orients_)
		{
			TEST_LOG("setting orient = " << orientToString(orient));
			synchronized(
				boost::bind(&elm_popup_orient_set, boost::ref(control_), orient)
			);

			TEST_LOG("checking orient attribute == " << orientToString(orient));
			FAIL_UNLESS_EQUAL(
				Application::synchronizedResult(
					[this]()->Elm_Popup_Orient {
						return elm_popup_orient_get(control_);
					}
				), orient
			);
		}
	}

private:
	Window				window_;
	Popup				control_;
	std::vector<Elm_Popup_Orient>	orients_;
};

typedef ResizeObjectTest<Popup> PopupResizeTest;
typedef PositionObjectTest<Popup> PopupPositionTest;
typedef VisibleObjectTest<Popup> PopupVisibilityTest;

//WFITS_EFL_HARNESS_TEST_CASE(PopupResizeTest)
// WFITS_EFL_HARNESS_TEST_CASE(PopupPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(PopupVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(PopupTimeoutTest)
WFITS_EFL_HARNESS_TEST_CASE(PopupOrientTest)

} // namespace efl
} // namespace test
} // namespace wfits
