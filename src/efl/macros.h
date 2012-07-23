#ifndef __WAYLAND_EFL_TEMPLATES_H__
#define __WAYLAND_EFL_TEMPLATES_H__

#include <Elementary.h>
#include <boost/bind.hpp>

#include "evasobject.h"
#include "window.h"
#include "elmtestharness.h"
#include "application.h"




#define EFL_RESIZE_TEST(_suite, _obj, _w1, _h1, _w2, _h2) \
class _suite ## ResizeTest : public ElmTestHarness \
{ \
public: \
	\
	_suite ## ResizeTest() \
		: ElmTestHarness::ElmTestHarness() \
		, window_(#_suite "ResizeTest", #_suite "Resize Test") \
		, control_(elm_ ## _obj ## _add(window_)) \
	{ \
		return; \
	} \
	\
	void setup() \
	{ \
		window_.show(); \
		control_.show(); \
		\
		EFL_CHECK_SIZE(control_, _w1, _h1); \
		EFL_CHECK_SIZE(control_, _w2, _h2); \
	} \
	\
private: \
	Window		window_; \
	EvasObject	control_; \
}; \
BOOST_AUTO_TEST_SUITE(EFL) \
	BOOST_AUTO_TEST_SUITE(_suite) \
		WAYLAND_ELM_HARNESS_TEST_CASE(_suite ## ResizeTest) \
	BOOST_AUTO_TEST_SUITE_END() \
BOOST_AUTO_TEST_SUITE_END() \



#define EFL_POSITION_TEST(_suite, _obj, _x1, _y1, _x2, _y2) \
class _suite ## PositionTest : public ElmTestHarness \
{ \
public: \
	\
	_suite ## PositionTest() \
		: ElmTestHarness::ElmTestHarness() \
		, window_(#_suite "PositionTest", #_suite "Position Test") \
		, control_(elm_ ## _obj ## _add(window_)) \
	{ \
		return; \
	} \
	\
	void setup() \
	{ \
		window_.show(); \
		control_.show(); \
		\
		EFL_CHECK_POSITION(control_, _x1, _x1); \
		EFL_CHECK_POSITION(control_, _x2, _x2); \
	} \
	\
private: \
	Window		window_; \
	EvasObject	control_; \
}; \
BOOST_AUTO_TEST_SUITE(EFL) \
	BOOST_AUTO_TEST_SUITE(_suite) \
		WAYLAND_ELM_HARNESS_TEST_CASE(_suite ## PositionTest) \
	BOOST_AUTO_TEST_SUITE_END() \
BOOST_AUTO_TEST_SUITE_END() \



#define EFL_VISIBILITY_TEST(_suite, _obj) \
class _suite ## VisibilityTest : public ElmTestHarness \
{ \
public: \
	\
	_suite ## VisibilityTest() \
		: ElmTestHarness::ElmTestHarness() \
		, window_(#_suite "VisibilityTest", #_suite "Visibility Test") \
		, control_(elm_ ## _obj ## _add(window_)) \
	{ \
		return; \
	} \
	\
	void setup() \
	{ \
		window_.show(); \
		\
		EFL_CHECK_SHOW(control_); \
		EFL_CHECK_HIDE(control_); \
	} \
	\
private: \
	Window		window_; \
	EvasObject	control_; \
}; \
BOOST_AUTO_TEST_SUITE(EFL) \
	BOOST_AUTO_TEST_SUITE(_suite) \
		WAYLAND_ELM_HARNESS_TEST_CASE(_suite ## VisibilityTest) \
	BOOST_AUTO_TEST_SUITE_END() \
BOOST_AUTO_TEST_SUITE_END() \


/* XXX: For now, keep it regular-- when BASIC_TESTS is uncommented and working,
 * XXX: makes it easier to replace across files
 */
// Basic suite START
//EFL_RESIZE_TEST(suite, obj, 300, 200, 200, 100)
//EFL_POSITION_TEST(suite, obj, 100, 50, 150, 100)
//EFL_VISIBILITY_TEST(suite, obj)
// Basic suite END


/* XXX
#define EFL_BASIC_TESTS(suite, obj) \
	EFL_RESIZE_TEST(suite, obj, 300, 200, 200, 100) \
	EFL_POSITION_TEST(suite, obj, 100, 50, 150, 100) \
	EFL_VISIBILITY_TEST(suite, obj) \
*/


#endif

