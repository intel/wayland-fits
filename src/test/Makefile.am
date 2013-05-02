SUBDIRS = core efl gtk+

TESTS = wfits

INCLUDES = -I$(top_srcdir)/src

bin_PROGRAMS = ${TESTS}

wfits_SOURCES =				\
	tools.cpp			\
	harness.cpp			\
	testmain.cpp			\
	$(top_srcdir)/src/extensions/protocol/wayland-fits-protocol.c

wfits_LDADD =				\
	core/libwfits-core.la		\
	-lboost_regex			\
	-lboost_filesystem		\
	-lboost_program_options		\
	$(CHECK_LIBS)

if BUILD_EFL_TESTS
wfits_LDADD += efl/libwfits-efl.la
endif

if BUILD_GTK_TESTS
wfits_LDADD += gtk+/libwfits-gtk.la
endif

AM_CXXFLAGS =				\
	-Wno-strict-aliasing		\
	-Werror -Wall			\
	-Wno-unused-variable		\
	-Wno-unknown-pragmas		\
	-Wno-parentheses		\
	-std=gnu++0x			\
	$(CHECK_CFLAGS)