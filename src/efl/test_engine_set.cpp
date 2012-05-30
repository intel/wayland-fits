#include <Elementary.h>
#include "../test.h"

BOOST_AUTO_TEST_CASE(elm_engine_set_wayland_shm)
{
	elm_init(
		boost::unit_test::framework::master_test_suite().argc,
		boost::unit_test::framework::master_test_suite().argv
	);
	elm_config_engine_set("wayland_shm");
	BOOST_CHECK_EQUAL("wayland_shm", elm_config_engine_get());
}

BOOST_AUTO_TEST_CASE(elm_engine_set_wayland_egl)
{
	elm_init(
		boost::unit_test::framework::master_test_suite().argc,
		boost::unit_test::framework::master_test_suite().argv
	);
	elm_config_engine_set("wayland_egl");
	BOOST_CHECK_EQUAL("wayland_egl", elm_config_engine_get());
}
