#include <Elementary.h>
#include <Ecore.h>

#include "application.h"
#include "window.h"
#include "../test.h"

#define TEST_ALL_ENGINES(name) \
\
void name##_test_run(); \
\
BOOST_AUTO_TEST_CASE(name##_shm_engine) \
{ \
	Application::setEngine(Application::ENGINE_SHM); \
	name##_test_run(); \
} \
\
BOOST_AUTO_TEST_CASE(name##_egl_engine) \
{ \
	Application::setEngine(Application::ENGINE_EGL); \
	name##_test_run(); \
} \
\
void name##_test_run()


TEST_ALL_ENGINES(efl_window_resize_move)
{
	Window win;

	win.show();

	win.setSize(400, 200);
	win.setPosition(10, 20);

	Application::yield();

	BOOST_CHECK_EQUAL(win.getWidth(), 400);
	BOOST_CHECK_EQUAL(win.getHeight(), 200);
	BOOST_CHECK_EQUAL(win.getX(), 10);
	BOOST_CHECK_EQUAL(win.getY(), 20);

	win.setSize(100, 120);
	win.setPosition(15, 25);

	Application::yield();

	BOOST_CHECK_EQUAL(win.getWidth(), 100);
	BOOST_CHECK_EQUAL(win.getHeight(), 120);
	BOOST_CHECK_EQUAL(win.getX(), 15);
	BOOST_CHECK_EQUAL(win.getY(), 25);
}

TEST_ALL_ENGINES(efl_button_resize_move)
{
	Window win;
	win.setSize(400, 400);
	win.show();

	EvasObject btn(elm_button_add(win));
	btn.show();

	btn.setSize(120, 30);
	btn.setPosition(60, 15);

	Application::yield();

	BOOST_CHECK_EQUAL(btn.getWidth(), 120);
	BOOST_CHECK_EQUAL(btn.getHeight(), 30);
	BOOST_CHECK_EQUAL(btn.getX(), 60);
	BOOST_CHECK_EQUAL(btn.getY(), 15);

	btn.setSize(75, 75);
	btn.setPosition(10, 10);

	Application::yield();

	BOOST_CHECK_EQUAL(btn.getWidth(), 75);
	BOOST_CHECK_EQUAL(btn.getHeight(), 75);
	BOOST_CHECK_EQUAL(btn.getX(), 10);
	BOOST_CHECK_EQUAL(btn.getY(), 10);
}
