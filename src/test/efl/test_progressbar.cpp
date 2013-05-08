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

class Progressbar : public EvasObject
{
public:
	Progressbar(EvasObject &parent)
		: EvasObject::EvasObject(elm_progressbar_add(parent))
	{
		return;
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

		queueStep(boost::bind(elm_progressbar_value_set, boost::ref(control_), 0.67f));
		queueStep(boost::bind(&ProgressbarValueTest::checkValue, boost::ref(*this), time(NULL) + 2, 0.67f));
	}

	void checkValue(const time_t max, const double expected)
	{
		// if taking too long, fail the test
		FAIL_UNLESS(time(NULL) < max);

		if (not timedout_) {
			// prevent a hot loop, yield
			Application::yield();

			// awaiting the "changed" signal
			queueStep(boost::bind(&ProgressbarValueTest::checkValue, boost::ref(*this), max, expected));

			return;
		}

		// Supposedly captured the "changed" signal-- check expected control state
		FAIL_UNLESS_EQUAL(timedout_, true);
		FAIL_UNLESS_EQUAL(elm_progressbar_value_get(control_), expected);

		// At this point, the test is done
		Application::yield();
	}

	static void changedReached(void* data, Evas_Object *obj, void* event_info)
	{
		static_cast<ProgressbarValueTest*>(data)->timedout_ = true;
	}

private:
	Window		window_;
	Progressbar	control_;
	bool		timedout_;
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

		queueStep(boost::bind(elm_progressbar_inverted_set, boost::ref(control_), EINA_TRUE));
		queueStep(boost::bind(&ProgressbarInvertedTest::checkInverted, boost::ref(*this), EINA_TRUE));

// 		queueStep(boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this)));
// 		queueStep(boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this)));

		queueStep(boost::bind(elm_progressbar_inverted_set, boost::ref(control_), EINA_FALSE));
		queueStep(boost::bind(&ProgressbarInvertedTest::checkInverted, boost::ref(*this), EINA_FALSE));

// 		queueStep(boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this)));
// 		queueStep(boost::bind(&ProgressbarInvertedTest::noOp, boost::ref(*this)));
	}

	void checkInverted(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_progressbar_inverted_get(control_), expected);
	}

private:
	Window		window_;
	Progressbar	control_;
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

		queueStep(boost::bind(elm_progressbar_pulse_set, boost::ref(control_), EINA_TRUE));
		queueStep(boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_TRUE));

		queueStep(boost::bind(elm_progressbar_pulse, boost::ref(control_), EINA_TRUE));
		queueStep(boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_TRUE));

		queueStep(boost::bind(elm_progressbar_pulse_set, boost::ref(control_), EINA_FALSE));
		queueStep(boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_FALSE));

		queueStep(boost::bind(elm_progressbar_pulse, boost::ref(control_), EINA_FALSE));
		queueStep(boost::bind(&ProgressbarPulseTest::checkPulse, boost::ref(*this), EINA_FALSE));
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

class ProgressbarOrientTest : public ElmTestHarness
{
public:
	ProgressbarOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ProgressbarOrientTest", "Progressbar Orient Test")
		, control_(window_)
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 50);

		elm_progressbar_value_set(control_, 0.5f);
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueStep(boost::bind(elm_progressbar_horizontal_set, boost::ref(control_), EINA_FALSE));
		queueStep(boost::bind(&ProgressbarOrientTest::checkOrient, boost::ref(*this), EINA_FALSE));

		queueStep(boost::bind(elm_progressbar_horizontal_set, boost::ref(control_), EINA_TRUE));
		queueStep(boost::bind(&ProgressbarOrientTest::checkOrient, boost::ref(*this), EINA_TRUE));
	}

	void checkOrient(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_progressbar_horizontal_get(control_), expected);
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

} // namespace efl
} // namespace test
} // namespace wfits
