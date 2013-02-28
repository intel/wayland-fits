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



void ElmTestHarness::geometryDone(QueryRequest* request, GeometryCallback callback)
{
	if (not request->done) {
		steps_.push_front(boost::bind(&ElmTestHarness::geometryDone, boost::ref(*this), request, callback));
	} else {
		Geometry *geometry = static_cast<Geometry*>(request->data);
		steps_.push_front(boost::bind(callback, *geometry));
		delete request;
		delete geometry;
	}
}

void ElmTestHarness::getSurfaceGeometry(wl_surface* surface, GeometryCallback callback)
{
	QueryRequest* request = wfits_.makeGeometryRequest(surface);
	geometryDone(request, callback);
}

ElmTestHarness::Position ElmTestHarness::getGlobalPointerPosition() const
{
	QueryRequest* request = wfits_.makeGlobalPointerPositionRequest();
	Position result;

	while (not request->done) {
		Application::yield();
	}

	Position *data(static_cast<Position*>(request->data));
	result = *data;

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

void ElmTestHarness::setGlobalPointerPosition(int32_t x, int32_t y) const
{
	wfits_.setGlobalPointerPosition(x, y);
	expectGlobalPointerPosition(x, y);
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
