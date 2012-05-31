#ifndef __WAYLAND_EFL_APPLICATION_H__
#define __WAYLAND_EFL_APPLICATION_H__

namespace boost {
	namespace unit_test {
		namespace ut_detail {
			template<class F> class global_fixture_impl;
		}
	}
}

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

#endif
