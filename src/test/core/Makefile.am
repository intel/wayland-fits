noinst_LTLIBRARIES = libwfits-core.la

libwfits_core_la_SOURCES =		\
	display.cpp			\
	compositor.cpp			\
	keyboard.cpp			\
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
	test_multi_resource.cpp		\
	test_surface.cpp		\
	test_dnd.cpp

libwfits_core_la_LIBADD =		\
	$(WAYLAND_LIBS)			\
	$(XKBCOMMON_LIBS)		\
	$(CHECK_LIBS)

libwfits_core_la_CPPFLAGS =		\
	-I$(top_srcdir)/src		\
	$(WAYLAND_CFLAGS)		\
	$(XKBCOMMON_CFLAGS)		\
	$(CHECK_CFLAGS)

AM_CXXFLAGS =				\
	-Wno-strict-aliasing		\
	-Werror -Wall			\
	-Wno-unused-variable		\
	-Wno-unknown-pragmas		\
	-Wno-parentheses		\
	-Wno-deprecated-declarations	\
	-std=gnu++0x
