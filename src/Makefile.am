
TESTS = wayland-test
TESTS_ENVIRONMENT = $(SHELL) test-harness

CXXFLAGS =			\
	-std=gnu++0x		\
	$(CAIRO_CFLAGS)		\
	$(WAYLAND_CFLAGS)	\
	@CHECK_CFLAGS@

LDFLAGS = 			\
	$(CAIRO_LIBS)		\
	$(WAYLAND_LIBS)		\
	@CHECK_LIBS@		\
	-lboost_program_options

bin_PROGRAMS = $(TESTS)

wayland_test_SOURCES =		\
	test_bind_interface.cpp	\
	test.cpp		\
	testmain.cpp
