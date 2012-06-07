#include <Elementary.h>

#include "elmtestharness.h"

ElmTestHarness::ElmTestHarness()
{
	eventType_	= ecore_event_type_new();
	handler_	= NULL;
}

void ElmTestHarness::run()
{
	// TODO: Add a run timeout just in case
	// a bug gets introduced into idler stuff.

	ecore_idler_add(idleSetup, this);
	elm_run();
}

void ElmTestHarness::queueCallback(ModifyCheckCallback callback)
{
	callbacks_.push_back(callback);
}

void ElmTestHarness::modify()
{
	if (haveMore())
	{
		// TODO: wrap this with try-catch
		callbacks_.front().first();
	}
}

void ElmTestHarness::check()
{
	if (haveMore())
	{
		// TODO: wrap this with try-catch
		callbacks_.front().second();
		callbacks_.pop_front();
	}
}

/*static*/
Eina_Bool ElmTestHarness::idleSetup(void* data)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);
	harness->handler_ = ecore_event_handler_add(
		harness->eventType_,
		doTestSetup,
		data
	);
	ecore_event_add(harness->eventType_, NULL, NULL, NULL);

	return ECORE_CALLBACK_CANCEL;
}

/*static*/
Eina_Bool ElmTestHarness::doTestSetup(void* data, int, void*)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	ecore_event_handler_del(harness->handler_);

	// TODO: wrap this with try-catch
	harness->setup();

	Application::yield();

	ecore_idler_add(idleModify, data);

	return ECORE_CALLBACK_DONE;
}

/*static*/
Eina_Bool ElmTestHarness::idleModify(void* data)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	harness->handler_ = ecore_event_handler_add(
		harness->eventType_,
		doTestModify,
		data
	);
	ecore_event_add(harness->eventType_, NULL, NULL, NULL);

	return ECORE_CALLBACK_CANCEL;
}

/*static*/
Eina_Bool ElmTestHarness::doTestModify(void* data, int, void*)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	ecore_event_handler_del(harness->handler_);

	harness->modify();
	
	Application::yield();

	ecore_idler_add(idleCheck, data);

	return ECORE_CALLBACK_DONE;
}

/*static*/
Eina_Bool ElmTestHarness::idleCheck(void* data)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	harness->handler_ = ecore_event_handler_add(
		harness->eventType_,
		doTestCheck,
		data
	);
	ecore_event_add(harness->eventType_, NULL, NULL, NULL);

	return ECORE_CALLBACK_CANCEL;
}

/*static*/
Eina_Bool ElmTestHarness::doTestCheck(void* data, int, void*)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	ecore_event_handler_del(harness->handler_);

	harness->check();

	Application::yield();

	harness->haveMore() ?
		ecore_idler_add(idleModify, data) : ecore_idler_add(idleTeardown, data);

	return ECORE_CALLBACK_DONE;
}

/*static*/
Eina_Bool ElmTestHarness::idleTeardown(void* data)
{
	ElmTestHarness* harness = static_cast<ElmTestHarness*>(data);

	harness->handler_ = ecore_event_handler_add(
		harness->eventType_,
		doTestTeardown,
		data
	);
	ecore_event_add(harness->eventType_, NULL, NULL, NULL);

	return ECORE_CALLBACK_CANCEL;
}

Eina_Bool ElmTestHarness::doTestTeardown(void* data, int, void*)
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
