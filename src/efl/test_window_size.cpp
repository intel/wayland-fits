#include <Elementary.h>
#include <Ecore.h>

#include "application.h"
#include "window.h"
#include "../test.h"

BOOST_AUTO_TEST_CASE(efl_window_size_shm)
{
	Application app;
	app.setEngine(Application::ENGINE_SHM);
	Window win("shm-size", "SHM Size");
	win.setSize(400, 400);
	win.setPosition(0, 0);
	win.show();

	app.yield();

	BOOST_CHECK_EQUAL(win.getWidth(), 400);
	BOOST_CHECK_EQUAL(win.getHeight(), 400);
}

BOOST_AUTO_TEST_CASE(efl_window_size_egl)
{
	Application app;
	app.setEngine(Application::ENGINE_EGL);
	Window win("egl-size", "EGL Size");
	win.setSize(400, 400);
	win.setPosition(0, 0);
	win.show();

	app.yield();

	BOOST_CHECK_EQUAL(win.getWidth(), 400);
	BOOST_CHECK_EQUAL(win.getHeight(), 400);
}

