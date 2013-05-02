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

#ifndef __WFITS_COMMON_HARNESS_H__
#define __WFITS_COMMON_HARNESS_H__

#include <deque>

#include "common/util.h"

class TestHarness
{
public:
	typedef boost::function<void()>	TestStep;
	typedef std::deque<TestStep>	TestSteps;

	TestHarness();

	virtual ~TestHarness();

	virtual void queueStep(TestStep);

	virtual void run();
	virtual void setup() { };
	virtual void teardown() { };

	void runNextStep();
	bool haveStep() const;

protected:
	TestSteps	steps_;
};

#define HARNESS_TEST(HarnessClass, suite) \
\
TEST(HarnessClass, suite) \
{ \
	HarnessClass().run(); \
}

#endif
