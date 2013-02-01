#ifndef __WAYLAND_EFL_ELMTESTHARNESS_H__
#define __WAYLAND_EFL_ELMTESTHARNESS_H__

#include <Elementary.h>
#include "common/harness.h"
#include "application.h"

class ElmTestHarness : public TestHarness
{
public:
	/**
	 * Construct the test harness.
	 **/
	ElmTestHarness();

	void run();

private:
	static Eina_Bool idleSetup(void*);
	static Eina_Bool doSetup(void*, int, void*);

	static Eina_Bool idleStep(void*);
	static Eina_Bool doStep(void*, int, void*);

	static Eina_Bool idleTeardown(void*);
	static Eina_Bool doTeardown(void*, int, void*);

	int			eventType_; /// custom event type
	Ecore_Event_Handler*	handler_;
};

#define WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite) \
TEST(Harness, "EFL/EGL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_EGL); \
	Harness().run(); \
}

#define WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
TEST(Harness##_shm_engine, "EFL/SHM/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_SHM); \
	Harness().run(); \
}

#define WAYLAND_ELM_HARNESS_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite)

#endif

