#ifndef __WAYLAND_EFL_APPLICATION_H__
#define __WAYLAND_EFL_APPLICATION_H__

#include "../test.h"

// namespace boost {
// 	namespace unit_test {
// 		namespace ut_detail {
// 			template<class F> class global_fixture_impl;
// 		}
// 	}
// }

class Application
{
public:
	enum Engine {
		ENGINE_SHM,
		ENGINE_EGL
	};

	virtual ~Application();

	static void setEngine(const Engine&);
	static void mainLoop();
	static void yield();
	static void exit();

private:
	friend class ::boost::unit_test::ut_detail::global_fixture_impl<Application>;
	Application();
};

#define WAYLAND_EFL_AUTO_TEST_CASE(name) \
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

#endif //__WAYLAND_EFL_APPLICATION_H__
