#include <Elementary.h>

#include "application.h"

Application::Application()
{
	elm_init(
		boost::unit_test::framework::master_test_suite().argc,
		boost::unit_test::framework::master_test_suite().argv
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
	BOOST_ASSERT(strEngine == std::string(elm_config_preferred_engine_get()));

	elm_config_engine_set(strEngine.c_str());
	BOOST_ASSERT(strEngine == std::string(elm_config_engine_get()));
}

/*static*/
void Application::yield(const unsigned time)
{
	ecore_main_loop_iterate();
	usleep(time);
}


// only one instance per test binary
BOOST_GLOBAL_FIXTURE(Application);
