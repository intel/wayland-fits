#ifndef __WAYLAND_EFL_APPLICATION_H__
#define __WAYLAND_EFL_APPLICATION_H__

#include "../test.h"

class Application
{
public:
	enum Engine {
		engineBegin = 0,
		ENGINE_SHM = engineBegin,	/// wayland_shm
		ENGINE_EGL,			/// wayland_egl
		engineLast
	};

	virtual ~Application();

	/**
	 * This will have an effect on objects created after
	 * calling this.
	 **/
	static void setEngine(const Engine&);

	/**
	 * Process pending events and sleep for specified
	 * microseconds.
	 **/
	static void yield(const unsigned time = 0.01 * 1e6);

private:
	friend class ::boost::unit_test::ut_detail::global_fixture_impl<Application>;

	/**
	 * Initialize's Elm and set's the default engine to ENGINE_SHM
	 **/
	Application();
};

#endif //__WAYLAND_EFL_APPLICATION_H__
