INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libwfits-test.la

libwfits_test_la_SOURCES =	\
	tools.cpp		\
	harness.cpp

libwfits_test_la_LIBADD =	\
	$(CHECK_LIBS)		\
	-lboost_regex

AM_CXXFLAGS =			\
	$(CHECK_CFLAGS)
