#include <Elementary.h>

#include "../test.h"
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
	// FIXME: hmmm... doesn't behave nicely across muliple boost test instances
	elm_shutdown();
}

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
	elm_config_engine_set(strEngine.c_str());
	BOOST_ASSERT(strEngine == std::string(elm_config_preferred_engine_get()));
	BOOST_ASSERT(strEngine == std::string(elm_config_engine_get()));
}

void Application::mainLoop()
{
	elm_run();
}

void Application::yield()
{
	ecore_main_loop_iterate();
	sleep(1);
}

void Application::exit()
{
	elm_exit();
}
