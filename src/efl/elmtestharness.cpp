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

ElmTestHarness::ElmTestHarness()
	: TestHarness::TestHarness()
	, eventType_(ecore_event_type_new())
	, handler_(NULL)
	, wfits_()
{
	return;
}

void ElmTestHarness::run()
{
	// TODO: Add a run timeout just in case
	// a bug gets introduced into idler stuff.

	ecore_idler_add(idleSetup, this);
	elm_run();
}

static void runStepWithMessage(TestHarness::TestStep step, std::string message)
{
	std::cout << "..." << message << std::endl;
	step();
}

void ElmTestHarness::queueStep(TestHarness::TestStep step, const std::string& message)
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

Geometry ElmTestHarness::getSurfaceGeometry(wl_surface* surface)
{
	WaylandFits::QueryRequest* request = wfits_.makeGeometryRequest(surface);

	while (not request->done) {
		Application::yield();
	}

	Geometry *data(static_cast<Geometry*>(request->data));
	Geometry result = *data;

	delete data;
	delete request;

	return result;
}

Position ElmTestHarness::getGlobalPointerPosition() const
{
	WaylandFits::QueryRequest* request = wfits_.makeGlobalPointerPositionRequest();

	while (not request->done) {
		Application::yield();
	}

	Position *data(static_cast<Position*>(request->data));
	Position result = *data;

	delete data;
	delete request;

	return result;
}

void ElmTestHarness::expectGlobalPointerPosition(int32_t x, int32_t y) const
{
	Position actual(getGlobalPointerPosition());
	while (actual.x != x or actual.y != y)
	{
		actual = getGlobalPointerPosition();
	}
}

void ElmTestHarness::expectGlobalPointerPosition(const Position& p) const
{
	expectGlobalPointerPosition(p.x, p.y);
}

void ElmTestHarness::setGlobalPointerPosition(int32_t x, int32_t y) const
{
	wfits_.setGlobalPointerPosition(x, y);
	expectGlobalPointerPosition(x, y);
}

void ElmTestHarness::setGlobalPointerPosition(const Position& p) const
{
	setGlobalPointerPosition(p.x, p.y);
}

void ElmTestHarness::inputKeySend(int32_t key, int32_t state) const
{
	wfits_.inputKeySend(key, state);
}

/*static*/
Eina_Bool ElmTestHarness::idleSetup(void* data)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);
	harness->handler_ = ecore_event_handler_add(
		harness->eventType_,
		doSetup,
		data
	);
	ecore_event_add(harness->eventType_, NULL, NULL, NULL);

	return ECORE_CALLBACK_CANCEL;
}

/*static*/
Eina_Bool ElmTestHarness::doSetup(void* data, int, void*)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	ecore_event_handler_del(harness->handler_);

	// TODO: wrap this with try-catch
	harness->setup();

	Application::yield();

	ecore_idler_add(idleStep, data);

	return ECORE_CALLBACK_DONE;
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
		Application::yield();
		ecore_idler_add(idleStep, data);
	} else {
		ecore_idler_add(idleTeardown, data);
	}

	return ECORE_CALLBACK_DONE;
}

/*static*/
Eina_Bool ElmTestHarness::idleTeardown(void* data)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	harness->handler_ = ecore_event_handler_add(
		harness->eventType_,
		doTeardown,
		data
	);
	ecore_event_add(harness->eventType_, NULL, NULL, NULL);

	return ECORE_CALLBACK_CANCEL;
}

Eina_Bool ElmTestHarness::doTeardown(void* data, int, void*)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	ecore_event_handler_del(harness->handler_);

	harness->handler_ = NULL;

	// TODO: wrap this with try-catch
	harness->teardown();

	Application::yield();

	elm_exit();

	return ECORE_CALLBACK_DONE;
}
