
if BUILD_EFL_TESTS

TESTS = wayland-efl-test
TESTS_ENVIRONMENT = $(SHELL) ../test-harness

CXXFLAGS +=			\
	-std=gnu++0x		\
	$(CAIRO_CFLAGS)		\
	$(WAYLAND_CFLAGS)	\
	@EFL_CFLAGS@

LDFLAGS += 			\
	$(CAIRO_LIBS)		\
	$(WAYLAND_LIBS)		\
	@EFL_LIBS@		\
	-lboost_unit_test_framework

bin_PROGRAMS = $(TESTS)

wayland_efl_test_SOURCES =	\
	application.cpp		\
	evasobject.cpp		\
	window.cpp		\
	test_evas_object.cpp	\
	test_engine_set.cpp	\
	../testmain.cpp

wayland_efl_test_CXXFLAGS = -DTESTLOG='"@abs_builddir@/wayland-efl-test.log"'

endif
