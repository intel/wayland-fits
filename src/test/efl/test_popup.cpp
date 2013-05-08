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
		, clock_(time(NULL))
	{
		elm_object_part_text_set(control_, "title,text", "Popup Title");
		elm_object_text_set(control_, "Popup Default Text");

		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		evas_object_smart_callback_add(control_, "timeout", timeoutReached, this);
	}
	
	void setup()
	{
		window_.show();
		control_.show();

		// TODO: Using time(), so smallest units of measure are 'seconds'
		// TODO: Use a monotonic mechanism instead of time() for better resolution
		queueStep(boost::bind(elm_popup_timeout_set, boost::ref(control_), 1.0f));
		queueStep(boost::bind(&PopupTimeoutTest::checkTimeout, boost::ref(*this), 1.0f));

		// If it takes more than 5 seconds for this event to fire, we have a bug
		checkTimedOut(time(NULL) + 5);
	}

	void checkTimedOut(const time_t max)
	{
		// if taking too long, fail the test
		FAIL_UNLESS(time(NULL) < max);

		if (not timedout_) {
			// prevent a hot loop, sleep for 100ms
			Application::yield(100);

			// awaiting the "timedout" signal
			queueStep(boost::bind(&PopupTimeoutTest::checkTimedOut, boost::ref(*this), max));

			return;
		}

		// Supposedly captured the timeout signal-- check expected control state
		FAIL_UNLESS_EQUAL(timedout_, true);
		FAIL_UNLESS_EQUAL(control_.isVisible(), EINA_FALSE);

		// And, check that at least a second has passed passed
		FAIL_UNLESS_GE(time(NULL), clock_ + 1);
	}	
	
	void checkTimeout(const double expected)
	{
		FAIL_UNLESS_EQUAL(elm_popup_timeout_get(control_), expected);
	}

	static void timeoutReached(void* data, Evas_Object *obj, void* event_info)
	{
		PopupTimeoutTest *t = static_cast<PopupTimeoutTest*>(data);

		t->timedout_ = true;
		t->control_.hide();

		Application::yield(10000);
	}

private:
	Window	window_;
	Popup	control_;
	bool	timedout_;
	time_t	clock_;
};

// TODO: add smart callbacks for "timeout" events for each orientation
class PopupOrientTest : public ElmTestHarness
{
public:
	PopupOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PopupOrientTest", "Popup Orientation Test")
		, control_(window_)
	{
		elm_object_part_text_set(control_, "title,text", "Popup Title");
		elm_object_text_set(control_, "Popup Default Text");

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

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
		window_.show();
		control_.show();

		foreach (const Elm_Popup_Orient orient, orients_) {
			queueStep(boost::bind(elm_popup_orient_set, boost::ref(control_), orient));
			queueStep(boost::bind(&PopupOrientTest::checkOrient, boost::ref(*this), orient));
		}
	}

	void checkOrient(const Elm_Popup_Orient expected)
	{
		control_.show();
		FAIL_UNLESS_EQUAL(elm_popup_orient_get(control_), expected);
		Application::yield();
	}

private:
	Window				window_;
	Popup				control_;
	std::vector<Elm_Popup_Orient>	orients_;
};

typedef ResizeObjectTest<Popup> PopupResizeTest;
typedef PositionObjectTest<Popup> PopupPositionTest;
typedef VisibleObjectTest<Popup> PopupVisibilityTest;

//WAYLAND_ELM_HARNESS_TEST_CASE(PopupResizeTest, "Popup")
WAYLAND_ELM_HARNESS_TEST_CASE(PopupPositionTest, "Popup")
WAYLAND_ELM_HARNESS_TEST_CASE(PopupVisibilityTest, "Popup")
WAYLAND_ELM_HARNESS_TEST_CASE(PopupTimeoutTest, "Popup")

} // namespace efl
} // namespace test
} // namespace wfits
