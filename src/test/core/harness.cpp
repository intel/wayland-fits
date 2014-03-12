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

namespace wfits {
namespace test {
namespace core {

Client::Client(const Display& display)
	: test::Client(display)
	, display_(display)
{
	return;
}

/*virtual*/
void Client::doYield(const unsigned time) const
{
	display_.yield(time);
}

Harness::Harness()
	: test::Harness::Harness()
	, display_()
	, client_(display_)
{
	queueStep(
		boost::bind(&Harness::setGlobalPointerPosition, boost::ref(*this), 0, 0)
	);
}

Harness::~Harness()
{
	return;
}

void Harness::runStep(TestStep step) const
{
	step();
	yield();
}

void Harness::queueStep(TestStep step)
{
	test::Harness::queueStep(
		boost::bind(&Harness::runStep, boost::ref(*this), step));
}

const test::Client& Harness::client() const
{
	return client_;
}

class SimpleTest : public Harness
{
public:
	SimpleTest() :
		Harness::Harness()
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

class GlobalPointerTest : public Harness
{
public:
	void setup()
	{
		queueStep(boost::bind(&GlobalPointerTest::test, boost::ref(*this)));
	}

	void test()
	{
		Position p(getGlobalPointerPosition());
		FAIL_IF_EQUAL(p.x, -1);
		FAIL_IF_EQUAL(p.y, -1);

		for (int y(0); y < 480; y += 80) {
			for (int x(0); x < 640; x += 80) {
				setGlobalPointerPosition(x, y);
				p = getGlobalPointerPosition();
				FAIL_UNLESS_EQUAL(p.x, x);
				FAIL_UNLESS_EQUAL(p.y, y);
			}
		}
	}
};

namespace harness {

	WFITS_CORE_HARNESS_TEST_CASE(SimpleTest)
	WFITS_CORE_HARNESS_TEST_CASE(GlobalPointerTest)

} // namespace harness

} // namespace core
} // namespace test
} // namespace wfits
