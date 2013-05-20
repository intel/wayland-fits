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

#include <Elementary.h>
#include "elmtestharness.h"
#include "test/client.h"

namespace wfits {
namespace test {
namespace efl {

ElmTestHarness::ElmTestHarness()
	: test::Harness::Harness()
	, eventType_(ecore_event_type_new())
	, handler_(NULL)
{
	return;
}

void ElmTestHarness::yield(const unsigned time) const
{
	Application::yield(time);
}

const test::Client& ElmTestHarness::client() const
{
	static const test::Client c(ecore_wl_display_get());
	return c;
}

void ElmTestHarness::run()
{
	ecore_wl_init(NULL);

	ASSERT(NULL != ecore_wl_display_get());

	setup();

	ecore_idler_add(idleStep, this);

	elm_run();

	teardown();

	ecore_wl_shutdown();

	ASSERT(not haveStep());
}

static void runStepWithMessage(ElmTestHarness::TestStep step, std::string message)
{
	std::cout << "..." << message << std::endl;
	step();
}

void ElmTestHarness::queueStep(TestStep step, const std::string& message)
{
	queueStep(
		boost::bind(&runStepWithMessage, step, message)
	);
}

void ElmTestHarness::stepUntilCondition(Condition condition)
{
	if (not condition()) {
		steps_.push_front(
			boost::bind(
				&ElmTestHarness::stepUntilCondition,
				boost::ref(*this),
				condition
			)
		);
	}
}

void ElmTestHarness::assertCondition(Condition condition)
{
	ASSERT(condition());
}

void ElmTestHarness::assertCondition(Condition condition, const std::string& message)
{
	ASSERT_MSG(condition(), message);
}

/*static*/
Eina_Bool ElmTestHarness::idleStep(void* data)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	harness->handler_ = ecore_event_handler_add(
		harness->eventType_,
		doStep,
		data
	);
	ecore_event_add(harness->eventType_, NULL, NULL, NULL);

	return ECORE_CALLBACK_CANCEL;
}

/*static*/
Eina_Bool ElmTestHarness::doStep(void* data, int, void*)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	ecore_event_handler_del(harness->handler_);

	if (harness->haveStep()) {
		harness->runNextStep();
		harness->yield();
		ecore_idler_add(idleStep, data);
	} else {
		elm_exit();
	}

	return ECORE_CALLBACK_DONE;
}

class SimpleHarnessTest : public ElmTestHarness
{
public:
	SimpleHarnessTest()
		: ElmTestHarness::ElmTestHarness()
		, nsetup_(0)
		, nsteps_(0)
		, nteardown_(0)
	{
		return;
	}

	~SimpleHarnessTest()
	{
		ASSERT(1 == nsetup_);
		ASSERT(50 == nsteps_);
		ASSERT(1 == nteardown_);
	}

	void setup()
	{
		++nsetup_;
		for (unsigned i(0); i < 50; ++i) {
			queueStep(boost::bind(&SimpleHarnessTest::step, boost::ref(*this)));
		}
	}

	void step()
	{
		++nsteps_;
	}

	void teardown()
	{
		++nteardown_;
	}

private:
	unsigned nsetup_;
	unsigned nsteps_;
	unsigned nteardown_;
};

class PointerInterfaceTest : public ElmTestHarness
{
public:
	PointerInterfaceTest()
		: ElmTestHarness::ElmTestHarness()
	{
		return;
	}

	void setup()
	{
		queueStep(boost::bind(&PointerInterfaceTest::test, boost::ref(*this)));
	}

	void test()
	{
		for (unsigned x(0), y(0); x < 300; x += 51, y += 37) {
			setGlobalPointerPosition(x, y);
			Position p(getGlobalPointerPosition());
			ASSERT(p.x == x and p.y == y);
			expectGlobalPointerPosition(x, y);

			p.x += 1;
			p.y += 1;
			setGlobalPointerPosition(p);
			Position np(getGlobalPointerPosition());
			ASSERT(np.x == p.x and np.y == p.y);
			expectGlobalPointerPosition(p);
		}
	}
};

WFITS_EFL_HARNESS_TEST_CASE(SimpleHarnessTest)
WFITS_EFL_HARNESS_TEST_CASE(PointerInterfaceTest)

} // namespace efl
} // namespace test
} // namespace wfits
