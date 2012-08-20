#include <Elementary.h>

#include "application.h"

Application::Application()
{
	elm_init(
		TheGlobalTestSuite::instance().argc,
		TheGlobalTestSuite::instance().argv
	);

	setEngine(ENGINE_SHM);
}

/* virtual */
Application::~Application()
{
	elm_shutdown();
}

/* static */
void Application::setEngine(const Engine& engine)
{
	std::string strEngine;
	switch (engine) {
		case ENGINE_EGL:
			strEngine = "wayland_egl";
			break;
		case ENGINE_SHM:
		default:
			strEngine = "wayland_shm";
	}

	elm_config_preferred_engine_set(strEngine.c_str()); // override's ELM_ENGINE user environment setting
	ASSERT(strEngine == std::string(elm_config_preferred_engine_get()));

	elm_config_engine_set(strEngine.c_str());
	ASSERT(strEngine == std::string(elm_config_engine_get()));
}

/*static*/
void Application::yield(const unsigned time)
{
	ecore_main_loop_iterate();
	usleep(time);
}


// only one instance per test binary
// BOOST_GLOBAL_FIXTURE(Application);
