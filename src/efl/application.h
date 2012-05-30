#ifndef __WAYLAND_EFL_APPLICATION_H__
#define __WAYLAND_EFL_APPLICATION_H__

class Application
{
public:
	enum Engine {
		ENGINE_SHM,
		ENGINE_EGL
	};

	Application();
	virtual ~Application();

	void setEngine(const Engine&);
	void mainLoop();
	void yield();
	void exit();
};

#endif
