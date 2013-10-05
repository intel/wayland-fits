EXTRA_DIST =			\
	wayland-fits.xml

BUILT_SOURCES =					\
	wayland-fits-protocol.c			\
	wayland-fits-server-protocol.h		\
	wayland-fits-client-protocol.h

CLEANFILES = $(BUILT_SOURCES)

%-protocol.c : %.xml
	$(AM_V_GEN)$(wayland_scanner) code < $< > $@

%-server-protocol.h : %.xml
	$(AM_V_GEN)$(wayland_scanner) server-header < $< > $@

%-client-protocol.h : %.xml
	$(AM_V_GEN)$(wayland_scanner) client-header < $< > $@
