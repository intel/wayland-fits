SUBDIRS = core efl gtk+

TESTS = wfits

INCLUDES = -I$(top_srcdir)/src

bin_PROGRAMS = ${TESTS}

wfits_SOURCES =				\
	tools.cpp			\
	harness.cpp			\
	client.cpp			\
	testmain.cpp			\
	$(top_srcdir)/src/extensions/protocol/wayland-fits-protocol.c

wfits_LDADD =				\
	core/libwfits-core.la		\
	-lboost_regex			\
	-lboost_filesystem		\
	-lboost_program_options		\
	-lboost_system			\
	$(CHECK_LIBS)			\
	$(WAYLAND_LIBS)

wfits_LDFLAGS =				\
	-Wl,--whole-archive,core/.libs/libwfits-core.a -Wl,--no-whole-archive

if BUILD_EFL_TESTS
wfits_LDFLAGS +=			\
	-Wl,--whole-archive,efl/.libs/libwfits-efl.a -Wl,--no-whole-archive
wfits_LDADD +=				\
	efl/libwfits-efl.la		\
	$(EFL_LIBS)
endif

if BUILD_GTK_TESTS
wfits_LDFLAGS +=			\
	-Wl,--whole-archive,gtk+/.libs/libwfits-gtk.a -Wl,--no-whole-archive
wfits_LDADD +=				\
	gtk+/libwfits-gtk.la		\
	$(GTK_LIBS)
endif

AM_CXXFLAGS =				\
	-Wno-strict-aliasing		\
	-Werror -Wall			\
	-Wno-unused-variable		\
	-Wno-unknown-pragmas		\
	-Wno-parentheses		\
	-std=gnu++0x			\
	$(CHECK_CFLAGS)