#include <Elementary.h>
#include "common/test.h"

TEST(ShmSetEngineTest, "EFL/Engine")
{
	const std::string engine("wayland_shm");

	elm_init(
		TheGlobalTestSuite::instance().argc,
		TheGlobalTestSuite::instance().argv
	);

	elm_config_preferred_engine_set(engine.c_str()); // override's ELM_ENGINE user environment setting
	elm_config_engine_set(engine.c_str());
	FAIL_UNLESS_EQUAL(engine, std::string(elm_config_preferred_engine_get()));
	FAIL_UNLESS_EQUAL(engine, std::string(elm_config_engine_get()));

	elm_shutdown();
}

TEST(EglSetEngineTest, "EFL/Engine")
{
	const std::string engine("wayland_egl");

	elm_init(
		TheGlobalTestSuite::instance().argc,
		TheGlobalTestSuite::instance().argv
	);

	elm_config_preferred_engine_set(engine.c_str()); // override's ELM_ENGINE user environment setting
	elm_config_engine_set(engine.c_str());
	FAIL_UNLESS_EQUAL(engine, std::string(elm_config_preferred_engine_get()));
	FAIL_UNLESS_EQUAL(engine, std::string(elm_config_engine_get()));

	elm_shutdown();
}

