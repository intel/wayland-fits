
CXXFLAGS = -std=gnu++0x $(CAIRO_CFLAGS) $(WAYLAND_CFLAGS)
LDFLAGS = 			\
	$(CAIRO_LIBS)		\
	$(WAYLAND_LIBS)		\
	-lboost_unit_test_framework

bin_PROGRAMS = runtests
runtests_SOURCES =			\
	test_bind_interface.cpp		\
	testmain.cpp
