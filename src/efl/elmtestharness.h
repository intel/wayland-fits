#ifndef __WAYLAND_EFL_ELMTESTHARNESS_H__
#define __WAYLAND_EFL_ELMTESTHARNESS_H__

#include <Elementary.h>
#include "common/harness.h"
#include "application.h"
#include "wayland-fits.h"

class ElmTestHarness : public TestHarness
{
public:
	typedef WaylandFits::Geometry Geometry;
	typedef WaylandFits::Position Position;
	typedef WaylandFits::QueryRequest QueryRequest;

	typedef boost::function<void (Geometry)> GeometryCallback;

	/**
	 * Construct the test harness.
	 **/
	ElmTestHarness();

	void run();

	void getSurfaceGeometry(wl_surface*, GeometryCallback);

	void setGlobalPointerPosition(int32_t, int32_t) const;
	Position getGlobalPointerPosition() const;
	void expectGlobalPointerPosition(int32_t, int32_t) const;

private:
	static Eina_Bool idleSetup(void*);
	static Eina_Bool doSetup(void*, int, void*);

	static Eina_Bool idleStep(void*);
	static Eina_Bool doStep(void*, int, void*);

	static Eina_Bool idleTeardown(void*);
	static Eina_Bool doTeardown(void*, int, void*);

	void geometryDone(QueryRequest*, GeometryCallback);

	int			eventType_; /// custom event type
	Ecore_Event_Handler*	handler_;
	WaylandFits		wfits_;
};

#define WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite) \
TEST(Egl##Harness, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_EGL); \
	Harness().run(); \
}

#define WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
TEST(Shm##Harness, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_SHM); \
	Harness().run(); \
}

#define WAYLAND_ELM_HARNESS_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite)


#endif

