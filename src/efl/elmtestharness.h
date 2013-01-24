#ifndef __WAYLAND_EFL_ELMTESTHARNESS_H__
#define __WAYLAND_EFL_ELMTESTHARNESS_H__

#include <boost/function.hpp>
#include <deque>

#include <Elementary.h>
#include "application.h"

class ElmTestHarness
{
public:
	typedef boost::function<void()>		Callback;
	typedef std::pair<Callback, Callback>	ModifyCheckCallback;
	typedef std::deque<ModifyCheckCallback>	ModifyCheckCallbacks;

	/**
	 * Construct the test harness.
	 **/
	ElmTestHarness();

	/**
	 * This'll kick-off the setup, modify/check, teardown
	 * sequence of calls through idle callbacks.
	 **/
	void run();

	void queueCallback(ModifyCheckCallback);
	
	/**
	 * Optionally override this to do any special
	 * setup before processing ModifyCheckCallbacks.
	 * i.e. you can queue all your callbacks from here.
	 **/
	virtual void setup() { };
	
	/**
	 * Optionally override teardown to do any special
	 * cleanup before the application exits.
	 **/
	virtual void teardown() { };

	void noop() { };

private:
	static Eina_Bool idleSetup(void*);
	static Eina_Bool doTestSetup(void*, int, void*);

	static Eina_Bool idleModify(void*);
	static Eina_Bool doTestModify(void*, int, void*);

	static Eina_Bool idleCheck(void*);
	static Eina_Bool doTestCheck(void*, int, void*);

	static Eina_Bool idleTeardown(void*);
	static Eina_Bool doTestTeardown(void*, int, void*);

	bool haveMore() const { return callbacks_.size() > 0; }
	void modify();
	void check();
	
	int			eventType_; /// custom event type
	Ecore_Event_Handler*	handler_;
	ModifyCheckCallbacks	callbacks_;
};

#define WAYLAND_ELM_HARNESS_TEST_CASE(Harness, suite) \
\
TEST(Harness##_shm_engine, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_SHM); \
	Harness().run(); \
} \
\
TEST(Harness##_egl_engine, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_EGL); \
	Harness().run(); \
} \

#define WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite) \
TEST(Harness##_egl_engine, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_EGL); \
	Harness().run(); \
} \

#define WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
TEST(Harness##_shm_engine, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_SHM); \
	Harness().run(); \
} \

#endif

