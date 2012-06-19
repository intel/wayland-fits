
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
	evasobject.cpp		\
	actionslider.cpp	\
	window.cpp		\
	application.cpp		\
	elmtestharness.cpp	\
	test_button.cpp		\
	test_window.cpp		\
	test_engine_set.cpp	\
	test_actionslider.cpp	\
	../testmain.cpp

endif
