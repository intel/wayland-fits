#include <Elementary.h>
#include <Ecore.h>

#include "application.h"
#include "window.h"

BOOST_AUTO_TEST_SUITE(Wayland_EFL_Window_Suite)

WAYLAND_EFL_AUTO_TEST_CASE(efl_window_resize)
{
	Window win;

	win.show();

	win.setSize(400, 200);

	Application::yield();

	BOOST_CHECK_EQUAL(win.getWidth(), 400);
	BOOST_CHECK_EQUAL(win.getHeight(), 200);

	win.setSize(100, 120);

	Application::yield();

	BOOST_CHECK_EQUAL(win.getWidth(), 100);
	BOOST_CHECK_EQUAL(win.getHeight(), 120);
}

WAYLAND_EFL_AUTO_TEST_CASE(efl_window_move)
{
	Window win;

	win.show();

	win.setPosition(10, 20);

	Application::yield();

	BOOST_CHECK_EQUAL(win.getX(), 10);
	BOOST_CHECK_EQUAL(win.getY(), 20);

	win.setPosition(15, 25);

	Application::yield();

	BOOST_CHECK_EQUAL(win.getX(), 15);
	BOOST_CHECK_EQUAL(win.getY(), 25);
}

BOOST_AUTO_TEST_SUITE_END()
