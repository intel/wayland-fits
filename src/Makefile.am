SUBDIRS = common core efl

TESTS = wfits

bin_PROGRAMS = ${TESTS}

wfits_SOURCES = testmain.cpp
wfits_LDADD = \
	$(top_builddir)/src/common/libwfits-common.la \
	$(top_builddir)/src/core/libwfits-core.la \
	$(top_builddir)/src/efl/libwfits-efl.la \
	-lboost_regex \
	-lboost_filesystem \
	-lboost_program_options

AM_CXXFLAGS = \
	-Wno-strict-aliasing \
	-Werror -Wall \
	-Wno-unused-variable \
	-Wno-unknown-pragmas \
	-Wno-parentheses \
	-std=gnu++0x \
	@CHECK_CFLAGS@
