if BUILD_WESTON_EXTENSIONS

moduledir = @libdir@/weston
module_LTLIBRARIES =			\
	$(weston_wfits)

weston_wfits = weston-wfits.la

weston_wfits_la_SOURCES =		\
	weston-wfits.cpp		\
	weston-wfits-init.cpp		\
	weston-wfits-query.cpp		\
	weston-wfits-input.cpp		\
	weston-wfits-manip.cpp		\
	input-emulator.cpp		\
	input-emulator-uinput.cpp	\
	input-emulator-notify.cpp	\
	$(top_srcdir)/src/extensions/protocol/wayland-fits-protocol.c

weston_wfits_la_LDFLAGS =		\
	-module -avoid-version

weston_wfits_la_LIBADD =		\
	$(WAYLAND_SERVER_LIBS)		\
	$(WESTON_LIBS)			\
	$(BOOST_LIBS)

weston_wfits_la_CPPFLAGS =		\
	-I$(top_srcdir)/src		\
	$(GCC_CFLAGS)			\
	$(WAYLAND_SERVER_CFLAGS)	\
	$(WESTON_CFLAGS)		\
	$(BOOST_CPPFLAGS)

AM_CXXFLAGS =				\
	-Wno-strict-aliasing		\
	-Werror -Wall			\
	-Wno-unused-variable		\
	-Wno-unknown-pragmas		\
	-Wno-parentheses		\
	-std=gnu++0x

endif
