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

Harness::Harness()
	: steps_()
{
	return;
}

Harness::~Harness()
{
	return;
}

void Harness::queueStep(TestStep step)
{
	steps_.push_back(step);
}

void Harness::run()
{
	setup();

	while (haveStep())
		runNextStep();

	teardown();
}

bool Harness::haveStep() const
{
	return not steps_.empty();
}

void Harness::runNextStep()
{
	TestStep step(steps_.front());
	steps_.pop_front();
	step();
}

Geometry Harness::getSurfaceGeometry(wl_surface *surface) const
{
	Client::QueryRequest* request = client().makeGeometryRequest(surface);

	YIELD_UNTIL(request->done);

	Geometry *data(static_cast<Geometry*>(request->data));
	Geometry result = *data;

	delete data;
	delete request;

	return result;
}

Position Harness::getGlobalPointerPosition() const
{
	Client::QueryRequest* request = client().makePointerPositionRequest();

	YIELD_UNTIL(request->done);

	Position *data(static_cast<Position*>(request->data));
	Position result = *data;

	delete data;
	delete request;

	return result;
}

void Harness::expectGlobalPointerPosition(int32_t x, int32_t y) const
{
	Position actual(getGlobalPointerPosition());
	while (actual.x != x or actual.y != y)
	{
		actual = getGlobalPointerPosition();
	}
}

void Harness::expectGlobalPointerPosition(const Position& p) const
{
	expectGlobalPointerPosition(p.x, p.y);
}

void Harness::setGlobalPointerPosition(int32_t x, int32_t y) const
{
	client().movePointerTo(x, y);
	expectGlobalPointerPosition(x, y);
}

void Harness::setGlobalPointerPosition(const Position& p) const
{
	setGlobalPointerPosition(p.x, p.y);
}

void Harness::inputKeySend(int32_t key, int32_t state) const
{
	client().sendKey(key, state);
}

} // namespace test
} // namespace wfits

