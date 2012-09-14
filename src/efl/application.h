#ifndef __WAYLAND_EFL_APPLICATION_H__
#define __WAYLAND_EFL_APPLICATION_H__

#include "common/test.h"

class Application
{
public:
	enum Engine {
		engineBegin = 0,
		ENGINE_SHM = engineBegin,	/// wayland_shm
		ENGINE_EGL,			/// wayland_egl
		engineLast
	};

	/**
	 * Initialize's Elm and set's the default engine to ENGINE_SHM
	 **/
	Application();

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
};

#endif //__WAYLAND_EFL_APPLICATION_H__
