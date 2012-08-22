
if BUILD_EFL_TESTS

TESTS = wayland-efl-test
TESTS_ENVIRONMENT = $(SHELL) ../test-harness

MEDIA = \"$(PWD)/../../media\"

CXXFLAGS +=			\
	-std=gnu++0x		\
	$(CAIRO_CFLAGS)		\
	$(WAYLAND_CFLAGS)	\
	-DMEDIA_PATH=$(MEDIA)	\
	@EFL_CFLAGS@		\
	@CHECK_CFLAGS@

LDFLAGS += 			\
	$(CAIRO_LIBS)		\
	$(WAYLAND_LIBS)		\
	@EFL_LIBS@		\
	@CHECK_LIBS@		\
	-lboost_system		\
	-lboost_filesystem	\
	-lboost_program_options

bin_PROGRAMS = $(TESTS)

wayland_efl_test_SOURCES =		\
	application.cpp			\
	background.cpp			\
	elmtestharness.cpp		\
	evasobject.cpp			\
	window.cpp			\
	test_actionslider.cpp		\
	test_background.cpp		\
	test_bubble.cpp			\
	test_button.cpp			\
	test_check.cpp			\
	test_clock.cpp			\
	test_colorselector.cpp		\
	test_dayselector.cpp		\
	test_engine_set.cpp		\
	test_entry.cpp			\
	test_fileselector_button.cpp	\
	test_fileselector_entry.cpp	\
	test_fileselector.cpp		\
	test_flip.cpp			\
	test_flipselector.cpp		\
	test_frame.cpp			\
	test_hover.cpp			\
	test_hoversel.cpp		\
	test_icon.cpp			\
	test_image.cpp			\
	test_inwin.cpp			\
	test_list.cpp			\
	test_label.cpp			\
	test_map.cpp			\
	test_mapbuf.cpp			\
	test_notify.cpp			\
	test_panel.cpp			\
	test_popup.cpp			\
	test_progressbar.cpp		\
	test_window.cpp			\
	../test.cpp			\
	../testmain.cpp

#	test_index.cpp

endif
