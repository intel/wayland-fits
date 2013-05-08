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

#include "harness.h"

CoreTestHarness::CoreTestHarness()
	: TestHarness::TestHarness()
	, display_()
{
	return;
}

CoreTestHarness::~CoreTestHarness()
{
	return;
}

void CoreTestHarness::runStep(CoreTestHarness::TestStep step) const
{
	step();
	yield();
}

void CoreTestHarness::queueStep(TestStep step)
{
	TestHarness::queueStep(
		boost::bind(&CoreTestHarness::runStep, boost::ref(*this), step));
}

void CoreTestHarness::yield(const unsigned time) const
{
	display().yield(time);
}

const wfits::test::Client& CoreTestHarness::client() const
{
	static const wfits::test::Client c(display_);
	return c;
}

class SimpleTest : public CoreTestHarness
{
public:
	SimpleTest() :
		CoreTestHarness::CoreTestHarness()
		, tested(0)
	{
		return;
	}

	void setup()
	{
		queueStep(boost::bind(&SimpleTest::test, boost::ref(*this)));
	}

	void test()
	{
		if (++tested < 10)
		{
			queueStep(boost::bind(&SimpleTest::test, boost::ref(*this)));
		}
	}

	void teardown()
	{
		FAIL_UNLESS_EQUAL(tested, 10);
	}

	unsigned tested;
};

WFITS_CORE_HARNESS_TEST_CASE(SimpleTest, "Harness")
