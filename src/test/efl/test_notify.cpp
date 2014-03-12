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
#include <linux/input.h>

#include <vector>

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class Notify : public EvasObject
{
public:
	Notify(EvasObject &parent)
		: EvasObject::EvasObject(elm_notify_add(parent))
	{
		return;
	}
};

class NotifyTimeoutTest : public ElmTestHarness
{
public:
	NotifyTimeoutTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("NotifyTimeoutTest", "Notify Timeout Test")
		, control_(window_)
		, content_(elm_label_add(window_))
		, timedout_(false)
		, start_(time(NULL))
		, end_(time(NULL))
	{
		return;
	}

	void setup()
	{
		elm_object_text_set(content_, "Notification");

		control_.setSize(300, 300);
		control_.setPosition(50, 10);
		elm_object_content_set(control_, content_);

		evas_object_smart_callback_add(control_, "timeout", onTimeout, this);

		window_.show();
		content_.show();
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
				elm_notify_timeout_set(control_, 1.0f);
			}
		);

		TEST_LOG("checking timeout attribute == " << 1.0f << "s");
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->double {
					return elm_notify_timeout_get(control_);
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
		NotifyTimeoutTest *t = static_cast<NotifyTimeoutTest*>(data);
		t->end_ = time(NULL);
		t->timedout_ = true;
		TEST_LOG("got timeout event; it took " << (t->end_ - t->start_) << "s");
	}

private:
	Window		window_;
	Notify		control_;
	EvasObject	content_;
	bool		timedout_;
	time_t		start_;
	time_t		end_;
};

static std::string toString(Elm_Notify_Orient orient)
{
	switch(orient)
	{
		case ELM_NOTIFY_ORIENT_TOP:
			return "TOP";
		case ELM_NOTIFY_ORIENT_CENTER:
			return "CENTER";
		case ELM_NOTIFY_ORIENT_BOTTOM:
			return "BOTTOM";
		case ELM_NOTIFY_ORIENT_LEFT:
			return "LEFT";
		case ELM_NOTIFY_ORIENT_RIGHT:
			return "RIGHT";
		case ELM_NOTIFY_ORIENT_TOP_LEFT:
			return "TOP_LEFT";
		case ELM_NOTIFY_ORIENT_TOP_RIGHT:
			return "TOP_RIGHT";
		case ELM_NOTIFY_ORIENT_BOTTOM_LEFT:
			return "BOTTOM_LEFT";
		case ELM_NOTIFY_ORIENT_BOTTOM_RIGHT:
			return "BOTTOM_RIGHT";
		default:
			return "UNKNOWN";
	}
}

// TODO: add smart callbacks for "timeout" events for each orientation
class NotifyOrientTest : public ElmTestHarness
{
	typedef boost::shared_ptr<Notify> SharedNotify;
	typedef std::vector<SharedNotify> Notifies;
public:
	NotifyOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("NotifyOrientTest", "Notify Orientation Test")
		, notifies_()
	{
		orients_ = {
			ELM_NOTIFY_ORIENT_TOP,
			ELM_NOTIFY_ORIENT_CENTER,
			ELM_NOTIFY_ORIENT_BOTTOM,
			ELM_NOTIFY_ORIENT_LEFT,
			ELM_NOTIFY_ORIENT_RIGHT,
			ELM_NOTIFY_ORIENT_TOP_LEFT,
			ELM_NOTIFY_ORIENT_TOP_RIGHT,
			ELM_NOTIFY_ORIENT_BOTTOM_LEFT,
			ELM_NOTIFY_ORIENT_BOTTOM_RIGHT
		};
	}

	void setup()
	{
		foreach (const Elm_Notify_Orient orient, orients_)
		{
			TEST_LOG("creating " << toString(orient) << " oriented notify");

			SharedNotify notify(new Notify(window_));
			elm_notify_orient_set(*notify, orient);

			Evas_Object* content(elm_label_add(window_));
			elm_object_text_set(content, toString(orient).c_str());
			elm_object_content_set(*notify, content);

			notifies_.push_back(notify);
			notify->show();
			evas_object_show(content);
		}
		window_.show();
	}

	void test()
	{
		unsigned i(0);
		foreach (const Elm_Notify_Orient orient, orients_)
		{
			TEST_LOG("checking orient attribute == " << toString(orient));
			SharedNotify notify = notifies_[i++];
			FAIL_UNLESS_EQUAL(
				Application::synchronizedResult(
					[this, &notify]()->Elm_Notify_Orient {
						return elm_notify_orient_get(*notify);
					}
				), orient
			);
		}
	}

private:
	Window				window_;
	Notifies			notifies_;
	std::vector<Elm_Notify_Orient>	orients_;
};

class NotifyUserClickTest : public ElmTestHarness
{
public:
	NotifyUserClickTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("NotifyUserClickTest", "Notify User Click Test")
		, notify_(elm_notify_add(window_))
		, notifyButton_(elm_button_add(window_))
		, blockClicked_(false)
		, buttonClicked_(false)
		, rendered_(false)
	{
		return;
	}

	void setup()
	{

		elm_object_text_set(notifyButton_, "Notification");
		elm_object_content_set(notify_, notifyButton_);

		elm_notify_allow_events_set(notify_, EINA_FALSE);
		evas_object_size_hint_weight_set(notify_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		evas_event_callback_add(
			evas_object_evas_get(window_),
			EVAS_CALLBACK_RENDER_POST,
			onPostRender, this
		);

		evas_object_smart_callback_add(notifyButton_, "clicked", onButtonClick, this);
		evas_object_smart_callback_add(notify_, "block,clicked", onBlockClick, this);

		window_.show();
		notifyButton_.show();
		notify_.show();
	}

	void test(){
		YIELD_UNTIL(rendered_);

		// FIXME:
		// This is highly dependent on the default sliding animation
		// and if that changes, then this may not work.  That is, the
		// animation needs to be finished prior to the following tests.
		// Currently, we rely on the base's 0.5 second yield prior
		// to this test method being called.

		ASSERT(not buttonClicked_);
		clickNotifyButton();
		YIELD_UNTIL(buttonClicked_)

		ASSERT(not blockClicked_);
		clickNotify();
		YIELD_UNTIL(blockClicked_);

	}

	void clickNotifyButton()
	{
		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gb(notifyButton_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gb.x + gb.width / 2,
			gw.y + gf.y + gb.y + gb.height / 2
		);

		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
	}

	void clickNotify()
	{
		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gn(notify_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gn.x + gn.width / 2,
			gw.y + gf.y + gn.y + gn.height / 2
		);

		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		NotifyUserClickTest *test = static_cast<NotifyUserClickTest*>(data);
		test->rendered_ = true;
		TEST_LOG("received post render event");
	}

	static void onBlockClick(void* data, Evas_Object*, void*)
	{
		NotifyUserClickTest *test = static_cast<NotifyUserClickTest*>(data);
		test->blockClicked_ = true;
		TEST_LOG("received block,clicked event");
	}

	static void onButtonClick(void* data, Evas_Object*, void*)
	{
		NotifyUserClickTest *test = static_cast<NotifyUserClickTest*>(data);
		test->buttonClicked_ = true;
		TEST_LOG("received clicked event");
	}

private:
	Window		window_;
	EvasObject	notify_;
	EvasObject	notifyButton_;
	bool		blockClicked_;
	bool		buttonClicked_;
	bool		rendered_;
};

typedef ResizeObjectTest<Notify> NotifyResizeTest;
typedef PositionObjectTest<Notify> NotifyPositionTest;
typedef VisibleObjectTest<Notify> NotifyVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(NotifyResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(NotifyPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(NotifyVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(NotifyTimeoutTest)
WFITS_EFL_HARNESS_TEST_CASE(NotifyOrientTest)
WFITS_EFL_HARNESS_TEST_CASE(NotifyUserClickTest)

} // namespace efl
} // namespace test
} // namespace wfits
