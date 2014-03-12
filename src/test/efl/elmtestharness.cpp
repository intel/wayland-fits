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

#include "elmtestharness.h"

namespace wfits {
namespace test {
namespace efl {

/*virtual*/ void ElmTestHarness::queueStep(TestStep)
{
	ASSERT_MSG(0, "Deprecated: " << BOOST_CURRENT_FUNCTION);
}

ElmTestHarness::ElmTestHarness()
	: test::Harness::Harness()
{
	return;
}

ElmTestHarness::~ElmTestHarness()
{
	return;
}

const test::Client& ElmTestHarness::client() const
{
	return Application::client();
}

void ElmTestHarness::testThreadRunner()
{
	// FIXME: we need a better way to detect when the windows/widgets
	// have stabilized their initial rendering, including animations.
	// For now, allow the main loop to run momentarily to *hopefully*
	// flush out all the widget and window animations
	yield(0.5*1e6, true);

	setGlobalPointerPosition(0, 0);

	test();

	Application::exit();
}

void ElmTestHarness::run()
{
	setup();

	client(); // initialize the client

	// launch the test thread
	boost::thread testThread(
		boost::bind(&ElmTestHarness::testThreadRunner, boost::ref(*this))
	);

	Application::run();

	testThread.join();

	teardown();
}

void ElmTestHarness::assertCondition(Condition condition)
{
	ASSERT(condition());
}

void ElmTestHarness::assertCondition(Condition condition, const std::string& message)
{
	ASSERT_MSG(condition(), message);
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
		ecore_wl_init(NULL);
		++nsetup_;

	}

	void test()
	{
		for (unsigned i(0); i < 50; ++i) {
			++nsteps_;
		}
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
		ecore_wl_init(NULL);
	}

	void test()
	{
		for (int x(0), y(0); x < 300; x += 51, y += 37) {
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
