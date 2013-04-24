#include <ctime>
#include <linux/input.h>

#include <vector>

#include "templates.h"

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
		, clock_(time(NULL))
	{
		elm_object_text_set(content_, "Notification");

		control_.setSize(300, 300);
		control_.setPosition(50, 10);
		elm_object_content_set(control_, content_);

		evas_object_smart_callback_add(control_, "timeout", timeoutReached, this);
	}
	
	void setup()
	{
		window_.show();
		content_.show();
		control_.show();

		// TODO: Using time(), so smallest units of measure are 'seconds'
		// TODO: Use a monotonic mechanism instead of time() for better resolution
		queueStep(boost::bind(elm_notify_timeout_set, boost::ref(control_), 1.0f));
		queueStep(boost::bind(&NotifyTimeoutTest::checkTimeout, boost::ref(*this), 1.0f));

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
			queueStep(boost::bind(&NotifyTimeoutTest::checkTimedOut, boost::ref(*this), max));

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
		FAIL_UNLESS_EQUAL(elm_notify_timeout_get(control_), expected);
	}

	static void timeoutReached(void* data, Evas_Object *obj, void* event_info)
	{
		NotifyTimeoutTest *ntt = static_cast<NotifyTimeoutTest*>(data);

		ntt->timedout_ = true;
		ntt->control_.hide();

		Application::yield(10000);
	}

private:
	Window		window_;
	Notify		control_;
	EvasObject	content_;
	bool		timedout_;
	time_t		clock_;
};


// TODO: add smart callbacks for "timeout" events for each orientation
class NotifyOrientTest : public ElmTestHarness
{
public:
	NotifyOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("NotifyOrientTest", "Notify Orientation Test")
		, control_(window_)
		, content_(elm_label_add(window_))
	{
		elm_object_text_set(content_, "Notification");
		elm_object_content_set(control_, content_);

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		orients_.push_back(ELM_NOTIFY_ORIENT_TOP);
		orients_.push_back(ELM_NOTIFY_ORIENT_CENTER);
		orients_.push_back(ELM_NOTIFY_ORIENT_BOTTOM);
		orients_.push_back(ELM_NOTIFY_ORIENT_LEFT);
		orients_.push_back(ELM_NOTIFY_ORIENT_RIGHT);
		orients_.push_back(ELM_NOTIFY_ORIENT_TOP_LEFT);
		orients_.push_back(ELM_NOTIFY_ORIENT_TOP_RIGHT);
		orients_.push_back(ELM_NOTIFY_ORIENT_BOTTOM_LEFT);
		orients_.push_back(ELM_NOTIFY_ORIENT_BOTTOM_RIGHT);
		orients_.push_back(ELM_NOTIFY_ORIENT_TOP);
	}

	void setup()
	{
		window_.show();
		content_.show();
		control_.show();

		foreach (const Elm_Notify_Orient orient, orients_) {
			// FIXME: elm_notify_orient_set is deprecated
			queueStep(boost::bind(elm_notify_orient_set, boost::ref(control_), orient));
			queueStep(boost::bind(&NotifyOrientTest::checkOrient, boost::ref(*this), orient));
		}
	}

	void checkOrient(const Elm_Notify_Orient expected)
	{
		control_.show();
		// FIXME: elm_notify_orient_get is deprecated
		FAIL_UNLESS_EQUAL(elm_notify_orient_get(control_), expected);
		Application::yield();
	}

private:
	Window				window_;
	Notify				control_;
	EvasObject			content_;
	std::vector<Elm_Notify_Orient>	orients_;
};

class NotifyUserClickTest : public ElmTestHarness
{
public:
	NotifyUserClickTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("NotifyUserClickTest", "Notify User Click Test")
		, notify_(elm_notify_add(window_))
		, notifyLabel_(elm_label_add(window_))
		, blockClicked_(false)
	{
		elm_object_text_set(notifyLabel_, "Notification");
		elm_object_content_set(notify_, notifyLabel_);

		elm_notify_allow_events_set(notify_, EINA_FALSE);
		evas_object_size_hint_weight_set(notify_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		evas_object_smart_callback_add(notify_, "block,clicked", onBlockClick, this);

	}

	void setup()
	{
		window_.show();
		notifyLabel_.show();
		notify_.show();

		// This will flush out the notify sliding animation.
		// However, this is highly dependent on the default animation
		// and if that changes, then this may not work.  But currently
		// 40 yields seems to do the trick. Same trick as in WindowResizeTest
		for (unsigned i(0); i < 40; ++i)
			queueStep(boost::bind(&Application::yield, 0.001*1e6));

		queueStep(boost::bind(&NotifyUserClickTest::clickNotifyLabel, boost::ref(*this)));
		queueStep(
			boost::bind(
				&NotifyUserClickTest::testNotifyState,
				boost::ref(*this),
				false
			),
			"Testing blockClicked_ == false"
		);

		queueStep(boost::bind(&NotifyUserClickTest::clickNotify, boost::ref(*this)));
		queueStep(
			boost::bind(
				&NotifyUserClickTest::testNotifyState,
				boost::ref(*this),
				true
			),
			"Testing blockClicked_ == true"
		);
	}

	void clickNotifyLabel()
	{
		Application::yield(0.01*1e6);

		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gnl(notifyLabel_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gnl.x + gnl.width / 2,
			gw.y + gf.y + gnl.y + gnl.height / 2
		);

		ASSERT(blockClicked_ == false);

		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
	}

	void clickNotify()
	{
		Application::yield(0.01*1e6);

		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gn(notify_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gn.x + gn.width / 2,
			gw.y + gf.y + gn.y + gn.height / 2
		);

		ASSERT(blockClicked_ == false);

		inputKeySend(BTN_LEFT, 1);
		inputKeySend(BTN_LEFT, 0);
	}

	static void onBlockClick(void* data, Evas_Object*, void*)
	{
		NotifyUserClickTest *test = static_cast<NotifyUserClickTest*>(data);
		test->blockClicked_ = true;
		std::cout << "...received block,clicked event" << std::endl;
	}

	void testNotifyState(bool clicked)
	{
		FAIL_UNLESS_EQUAL(blockClicked_, clicked);
	}

private:
	Window		window_;
	EvasObject	notify_;
	EvasObject	notifyLabel_;
	bool		blockClicked_;
};

typedef ResizeObjectTest<Notify> NotifyResizeTest;
typedef PositionObjectTest<Notify> NotifyPositionTest;
typedef VisibleObjectTest<Notify> NotifyVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(NotifyResizeTest, "Notify")
WAYLAND_ELM_HARNESS_TEST_CASE(NotifyPositionTest, "Notify")
WAYLAND_ELM_HARNESS_TEST_CASE(NotifyVisibilityTest, "Notify")
WAYLAND_ELM_HARNESS_TEST_CASE(NotifyTimeoutTest, "Notify")
WAYLAND_ELM_HARNESS_TEST_CASE(NotifyOrientTest, "Notify")

WAYLAND_ELM_HARNESS_TEST_CASE(NotifyUserClickTest, "Notify")

