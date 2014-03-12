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
		, changed_(false)
	{
		return;
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		elm_progressbar_value_set(control_, 0.f);

		evas_object_smart_callback_add(control_, "changed", onChanged, this);

		window_.show();
		control_.show();
	}

	void test()
	{
		TEST_LOG("testing initial progressbar value = " << 0.f);
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->double {
					return elm_progressbar_value_get(control_);
				}
			), 0.f
		);

		// increasing
		for (double v(0.05f); v < 1.0f; v += 0.05f)
		{
			testValueSet(v);
		}

		// decreasing
		for (double v(1.0f); v >= 0.0f; v -= 0.05f)
		{
			testValueSet(v);
		}
	}

	void testValueSet(double value)
	{
		TEST_LOG("setting progressbar value = " << value);
		changed_ = false;
		synchronized(
			[this, &value]() {
				elm_progressbar_value_set(control_, value);
			}
		);

		TEST_LOG("checking for changed event");
		YIELD_UNTIL(changed_);

		TEST_LOG("checking progressbar value attribute == " << value);
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->double {
					return elm_progressbar_value_get(control_);
				}
			), value
		);
	}

	static void onChanged(void* data, Evas_Object *obj, void* event_info)
	{
		static_cast<ProgressbarValueTest*>(data)->changed_ = true;
		TEST_LOG("got changed event");
	}

private:
	Window		window_;
	Progressbar	control_;
	bool		changed_;
};

class ProgressbarInvertedTest : public ElmTestHarness
{
public:
	ProgressbarInvertedTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ProgressbarInvertedTest", "Progressbar Inverted Test")
		, control_(window_)
	{
		return;
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		elm_progressbar_value_set(control_, 0.34f);

		window_.show();
		control_.show();
	}

	void test()
	{
		for (unsigned i(0); i < 5; ++i)
		{
			testInverted(EINA_TRUE);
			testInverted(EINA_FALSE);
		}
	}

	void testInverted(Eina_Bool inverted)
	{
		TEST_LOG("setting progressbar inverted = " << bool(inverted));
		synchronized(
			boost::bind(&elm_progressbar_inverted_set, boost::ref(control_), inverted)
		);

		TEST_LOG("checking progressbar inverted attribute == " << bool(inverted));
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Eina_Bool {
					return elm_progressbar_inverted_get(control_);
				}
			), inverted
		);
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
		return;
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();
	}

	void test()
	{
		testPulse(EINA_TRUE);
		testPulse(EINA_FALSE);
	}

	void testPulse(Eina_Bool pulse)
	{
		TEST_LOG("setting progressbar pulse = " << bool(pulse));
		synchronized(
			boost::bind(&elm_progressbar_pulse_set, boost::ref(control_), pulse)
		);
		TEST_LOG("checking progressbar pulse attribute == " << bool(pulse));
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Eina_Bool {
					return elm_progressbar_pulse_get(control_);
				}
			), pulse
		);

		TEST_LOG("pulsing progressbar = " << bool(pulse));
		synchronized(
			boost::bind(&elm_progressbar_pulse, boost::ref(control_), pulse)
		);
		TEST_LOG("checking progressbar pulse attribute == " << bool(pulse));
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Eina_Bool {
					return elm_progressbar_pulse_get(control_);
				}
			), pulse
		);
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
		return;
	}

	void setup()
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 50);

		elm_progressbar_value_set(control_, 0.5f);

		window_.show();
		control_.show();
	}

	void test()
	{
		TEST_LOG("setting progressbar not horizontal");
		synchronized(
			boost::bind(&elm_progressbar_horizontal_set, boost::ref(control_), EINA_FALSE)
		);
		TEST_LOG("checking progressbar not horizontal attribute");
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Eina_Bool {
					return elm_progressbar_horizontal_get(control_);
				}
			), EINA_FALSE
		);

		TEST_LOG("setting progressbar horizontal");
		synchronized(
			boost::bind(&elm_progressbar_horizontal_set, boost::ref(control_), EINA_TRUE)
		);
		TEST_LOG("checking progressbar horizontal attribute");
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Eina_Bool {
					return elm_progressbar_horizontal_get(control_);
				}
			), EINA_TRUE
		);
	}

private:
	Window		window_;
	Progressbar	control_;
};

typedef ResizeObjectTest<Progressbar> ProgressbarResizeTest;
typedef PositionObjectTest<Progressbar> ProgressbarPositionTest;
typedef VisibleObjectTest<Progressbar> ProgressbarVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(ProgressbarResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(ProgressbarPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(ProgressbarVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(ProgressbarValueTest)
WFITS_EFL_HARNESS_TEST_CASE(ProgressbarPulseTest)
WFITS_EFL_HARNESS_TEST_CASE(ProgressbarInvertedTest)
WFITS_EFL_HARNESS_TEST_CASE(ProgressbarOrientTest)

} // namespace efl
} // namespace test
} // namespace wfits
