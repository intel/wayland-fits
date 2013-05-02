SUBDIRS = common extensions test core efl gtk+

TESTS = wfits

bin_PROGRAMS = ${TESTS}

wfits_SOURCES = testmain.cpp
wfits_LDADD =							\
	$(top_builddir)/src/test/libwfits-test.la		\
	$(top_builddir)/src/core/libwfits-core.la		\
	-lboost_regex						\
	-lboost_filesystem					\
	-lboost_program_options					\
	$(CHECK_LIBS)

if BUILD_EFL_TESTS
wfits_LDADD += $(top_builddir)/src/efl/libwfits-efl.la
endif

if BUILD_GTK_TESTS
wfits_LDADD += $(top_builddir)/src/gtk+/libwfits-gtk.la
endif

AM_CXXFLAGS =			\
	-Wno-strict-aliasing	\
	-Werror -Wall		\
	-Wno-unused-variable	\
	-Wno-unknown-pragmas	\
	-Wno-parentheses	\
	-std=gnu++0x		\
	$(CHECK_CFLAGS)
