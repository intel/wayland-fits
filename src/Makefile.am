
TESTS = wayland-test
TESTS_ENVIRONMENT = $(SHELL) test-harness

CXXFLAGS =			\
	-std=gnu++0x		\
	$(CAIRO_CFLAGS)		\
	$(WAYLAND_CFLAGS)

LDFLAGS = 			\
	$(CAIRO_LIBS)		\
	$(WAYLAND_LIBS)		\
	-lboost_unit_test_framework

bin_PROGRAMS = $(TESTS)

wayland_test_SOURCES =			\
	test_bind_interface.cpp		\
	testmain.cpp

wayland_test_CXXFLAGS = -DTESTLOG='"@abs_builddir@/wayland-test.log"'
