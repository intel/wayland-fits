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

#ifndef __WFITS_CORE_HARNESS_H__
#define __WFITS_CORE_HARNESS_H__

#include "test/harness.h"
#include "display.h"

namespace wfits {
namespace test {
namespace core {

class Client : public test::Client
{
public:
	Client(const Display&);

protected:
	/*virtual*/ void doYield(const unsigned time = 0.01 * 1e6) const;

private:
	const Display& display_;
};

class Harness : public test::Harness
{
public:
	Harness();

	virtual ~Harness();
	
	void queueStep(TestStep);

	const Display& display() const { return display_; }

	/*virtual*/ const test::Client& client() const;

private:
	void runStep(TestStep step) const;

	Display display_;
	core::Client client_;
};

#define WFITS_CORE_HARNESS_TEST_CASE(HarnessClass) \
	HARNESS_TEST(HarnessClass)

} // namespace core
} // namespace test
} // namespace wfits

#endif
