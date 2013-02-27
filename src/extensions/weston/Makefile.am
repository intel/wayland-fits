if BUILD_WESTON_EXTENSIONS

INCLUDES = -I$(top_srcdir)/src

moduledir = @libdir@/weston
module_LTLIBRARIES =				\
	$(weston_wfits)

weston_wfits = weston-wfits.la
weston_wfits_la_CFLAGS =			\
	$(GCC_CFLAGS)				\
	$(WAYLAND_SERVER_CFLAGS)		\
	$(WESTON_CFLAGS)
weston_wfits_la_LDFLAGS = -module -avoid-version
weston_wfits_la_LIBADD =			\
	$(WAYLAND_SERVER_LIBS)			\
	$(WESTON_LIBS)
weston_wfits_la_SOURCES =						\
	weston-wfits.c							\
	$(top_srcdir)/src/extensions/protocol/wayland-fits-protocol.c

endif
