INCLUDES = -I$(top_srcdir)/src

noinst_LTLIBRARIES = libwfits-core.la

libwfits_core_la_SOURCES =		\
	display.cpp			\
	compositor.cpp			\
	shell.cpp			\
	seat.cpp			\
	pointer.cpp			\
	surface.cpp			\
	shm.cpp				\
	shell_surface.cpp		\
	harness.cpp			\
	test_bind_interface.cpp		\
	test_cursor.cpp			\
	test_data.cpp			\
	test_surface.cpp

AM_CXXFLAGS = \
	$(WAYLAND_CFLAGS)		\
	$(CHECK_CFLAGS)
