#include "templates.h"

class Progressbar : public EvasObject
{
public:
	Progressbar(EvasObject &parent)
		: EvasObject::EvasObject(elm_progressbar_add(parent))
	{
	}
};

class ProgressbarValueTest : public ElmTestHarness
{
public:

	ProgressbarValueTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ProgressbarValueTest", "Progressbar Value Test")
		, control_(window_)
		, timedout_(false)
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		evas_object_smart_callback_add(control_, "changed", changedReached, this);
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_progressbar_value_set, boost::ref(control_), 0.67f),
				boost::bind(&ProgressbarValueTest::checkValue, boost::ref(*this), time(NULL) + 2, 0.67f)
			)
		);
	}

	void checkValue(const time_t max, const double expected)
	{
		// if taking too long, fail the test
		FAIL_UNLESS(time(NULL) < max);

		if (not timedout_)
		{
			// prevent a hot loop, yield
			Application::yield();

			// awaiting the "changed" signal, so queue another noOp
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&ProgressbarValueTest::noOp, boost::ref(*this)),
					boost::bind(&ProgressbarValueTest::checkValue, boost::ref(*this), max, expected)
				)
			);

			return;
		}

		// Supposedly captured the "changed" signal-- check expected control state
		FAIL_UNLESS_EQUAL(timedout_, true);
		FAIL_UNLESS_EQUAL(elm_progressbar_value_get(control_), expected);

		// At this point, the test is done
		Application::yield();
	}

	void noOp(void)
	{
		// no op
	}

	static void changedReached(void* data, Evas_Object *obj, void* event_info)
	{
		static_cast<ProgressbarValueTest*>(data)->timedout_ = true;
	}

private:
	Window			window_;
	Progressbar		control_;
	bool			timedout_;
};

class ProgressbarInvertedTest : public ElmTestHarness
{
public:
	ProgressbarInvertedTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ProgressbarInvertedTest", "Progressbar Inverted Test")
		, control_(window_)
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		elm_progressbar_value_set(control_, 0.34f);
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_progressbar_inverted_set, boost::ref(control_), EINA_TRUE),
				boost::bind(&ProgressbarInvertedTest::checkInverted, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this)),
				boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this))
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_progressbar_inverted_set, boost::ref(control_), EINA_FALSE),
				boost::bind(&ProgressbarInvertedTest::checkInverted, boost::ref(*this), EINA_FALSE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this)),
				boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this))
			)
		);

	}

	void noOp(void)
	{
		// no op
	}

	void checkInverted(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_progressbar_inverted_get(control_), expected);
	}

private:
	Window			window_;
	Progressbar		control_;
};

class ProgressbarPulseTest : public ElmTestHarness
{
public:

	ProgressbarPulseTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ProgressbarPulseTest", "Progressbar Pulse Test")
		, control_(window_)
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_progressbar_pulse_set, boost::ref(control_), EINA_TRUE),
				boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_progressbar_pulse, boost::ref(control_), EINA_TRUE),
				boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_progressbar_pulse_set, boost::ref(control_), EINA_FALSE),
				boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_FALSE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_progressbar_pulse, boost::ref(control_), EINA_FALSE),
				boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_FALSE)
			)
		);
	}

	void checkPulse(const Eina_Bool expected)
	{
		Application::yield(10000);
		FAIL_UNLESS_EQUAL(elm_progressbar_pulse_get(control_), expected);
	}

private:
	Window		window_;
	Progressbar	control_;
};

typedef ResizeObjectTest<Progressbar> ProgressbarResizeTest;
typedef PositionObjectTest<Progressbar> ProgressbarPositionTest;
typedef VisibleObjectTest<Progressbar> ProgressbarVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(ProgressbarResizeTest, "Progressbar")
WAYLAND_ELM_HARNESS_TEST_CASE(ProgressbarPositionTest, "Progressbar")
WAYLAND_ELM_HARNESS_TEST_CASE(ProgressbarVisibilityTest, "Progressbar")
WAYLAND_ELM_HARNESS_TEST_CASE(ProgressbarValueTest, "Progressbar")
WAYLAND_ELM_HARNESS_TEST_CASE(ProgressbarPulseTest, "Progressbar")
WAYLAND_ELM_HARNESS_TEST_CASE(ProgressbarInvertedTest, "Progressbar")

