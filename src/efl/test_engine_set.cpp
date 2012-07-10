#include <Elementary.h>
#include "../test.h"


BOOST_AUTO_TEST_SUITE(Wayland_EFL_Suite)

BOOST_AUTO_TEST_SUITE(Wayland_EFL_Misc_Suite)

BOOST_AUTO_TEST_CASE(elm_engine_set_wayland_shm)
{
	const std::string engine("wayland_shm");

	BOOST_MESSAGE("engine=" << engine);

	elm_init(
		boost::unit_test::framework::master_test_suite().argc,
		boost::unit_test::framework::master_test_suite().argv
	);

	elm_config_preferred_engine_set(engine.c_str()); // override's ELM_ENGINE user environment setting
	elm_config_engine_set(engine.c_str());
	BOOST_CHECK_EQUAL(engine, std::string(elm_config_preferred_engine_get()));
	BOOST_CHECK_EQUAL(engine, std::string(elm_config_engine_get()));
}

BOOST_AUTO_TEST_CASE(elm_engine_set_wayland_egl)
{
	const std::string engine("wayland_egl");

	BOOST_MESSAGE("engine=" << engine);

	elm_init(
		boost::unit_test::framework::master_test_suite().argc,
		boost::unit_test::framework::master_test_suite().argv
	);

	elm_config_preferred_engine_set(engine.c_str()); // override's ELM_ENGINE user environment setting
	elm_config_engine_set(engine.c_str());
	BOOST_CHECK_EQUAL(engine, std::string(elm_config_preferred_engine_get()));
	BOOST_CHECK_EQUAL(engine, std::string(elm_config_engine_get()));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

