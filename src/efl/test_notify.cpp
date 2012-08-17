#include <ctime>
#include "templates.h"

class Notify : public EvasObject
{
public:
	Notify(EvasObject &parent)
		: EvasObject::EvasObject(elm_notify_add(parent))
	{
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
		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_notify_timeout_set, boost::ref(control_), 1.0f),
				boost::bind(&NotifyTimeoutTest::checkTimeout, boost::ref(*this), 1.0f)
			)
		);

		// If it takes more than 2 seconds for this event to fire, we have a bug
		checkTimedOut(time(NULL) + 2);
	}

	void noOp(void)
	{
		// no op
	}

	void checkTimedOut(const time_t max)
	{
		// if taking too long, fail the test
		BOOST_REQUIRE(time(NULL) < max);

		if (not timedout_)
		{
			// prevent a hot loop, sleep for 1/100 of a second
			Application::yield(10000);

			// awaiting the "timedout" signal, so queue another noOp
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&NotifyTimeoutTest::noOp, boost::ref(*this)),
					boost::bind(&NotifyTimeoutTest::checkTimedOut, boost::ref(*this), max)
				)
			);

			return;
		}

		// Supposedly captured the timeout signal-- check expected control state
		BOOST_CHECK_EQUAL(timedout_, true);
		BOOST_CHECK_EQUAL(control_.isVisible(), EINA_FALSE);

		// And, check that at least a second has passed passed
		BOOST_CHECK_GE(time(NULL), clock_ + 1);
	}	
	
	void checkTimeout(const double expected)
	{
		BOOST_CHECK_EQUAL(elm_notify_timeout_get(control_), expected);
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

typedef ResizeObjectTest<Notify> NotifyResizeTest;
typedef PositionObjectTest<Notify> NotifyPositionTest;
typedef VisibleObjectTest<Notify> NotifyVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Notify)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(NotifyResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(NotifyPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(NotifyVisibilityTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(NotifyTimeoutTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

