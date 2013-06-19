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
	data_device_manager.cpp		\
	data_offer.cpp			\
	data_target.cpp			\
	data_device.cpp			\
	data_source.cpp			\
	test_bind_interface.cpp		\
	test_cursor.cpp			\
	test_data.cpp			\
	test_surface.cpp		\
	test_dnd.cpp

AM_CXXFLAGS = \
	$(WAYLAND_CFLAGS)		\
	$(CHECK_CFLAGS)
