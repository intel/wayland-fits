INCLUDES = -I$(top_srcdir)/src

moduledir = @libdir@/weston
module_LTLIBRARIES =				\
	$(weston_wfits)

# AM_CFLAGS = $(GCC_CFLAGS)
# AM_CPPFLAGS = -I$(top_srcdir)/src $(WAYLAND_SERVER_CFLAGS)
# AM_LDFLAGS = -module -avoid-version -rpath $(libdir)

weston_wfits = weston-wfits.la
weston_wfits_la_CFLAGS =			\
	$(GCC_CFLAGS)				\
	$(WAYLAND_SERVER_CFLAGS)		\
	$(PIXMAN_CFLAGS)

weston_wfits_la_LDFLAGS = -module -avoid-version
weston_wfits_la_LIBADD = $(WAYLAND_SERVER_LIBS)
weston_wfits_la_SOURCES =						\
	weston-wfits.c							\
	$(top_srcdir)/src/extensions/protocol/wayland-fits-protocol.c