/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "testharness.h"

namespace wfits {
namespace test {
namespace gtk {

class WindowFullscreenTest :  public GtkTestHarness
{
public:
	WindowFullscreenTest()
		: GtkTestHarness::GtkTestHarness()
		, win_(gtk_window_new(GTK_WINDOW_TOPLEVEL))
		, isFullscreen_(false)
		, isConfigured_(false)
	{
		gtk_window_set_title(*this, fullname(*this).c_str());
		gtk_window_resize(*this, 360, 225);
	}

	~WindowFullscreenTest()
	{
		gtk_widget_destroy(win_);
	}

	void setup()
	{
		g_signal_connect(win_, "window-state-event", G_CALLBACK(onState), this);
		g_signal_connect(win_, "configure-event", G_CALLBACK(onConfigure), this);

		gtk_widget_show(win_);

		for (unsigned i(0); i < 3; ++i) {
			queueStep(boost::bind(&WindowFullscreenTest::test, boost::ref(*this)));
		}
	}

	/**
	 * The assumption is that window fullscreening is honored by the window
	 * manager/compositor and fills the entire screen size when fullscreened.
	 * When unfullscreened, the window should restore to its initial size
	 * and position.
	 */
	void test()
	{
		gint width, height;
		GdkScreen *screen(gtk_window_get_screen(*this));
		Geometry fullscreen(
			0, 0,
			gdk_screen_get_width(screen),
			gdk_screen_get_height(screen)
		);
		const Geometry initial(getSurfaceGeometry(*this));

		std::cout << "...initial server geometry " << initial << std::endl;

		gtk_window_get_size(*this, &width, &height);

		std::cout << "...checking server size == client size" << std::endl;
		FAIL_UNLESS_EQUAL(width, initial.width);
		FAIL_UNLESS_EQUAL(height, initial.height);

		std::cout << "...checking client size != fullscreen size" << std::endl;
		FAIL_IF_EQUAL(width, fullscreen.width);
		FAIL_IF_EQUAL(height, fullscreen.height);

		isConfigured_ = false;

		std::cout << "...fullscreening client" << std::endl;
		gtk_window_fullscreen(*this);

		std::cout << "...checking for fullscreen state event" << std::endl;
		YIELD_UNTIL(isFullscreen_);

		std::cout << "...checking for configure event" << std::endl;
		YIELD_UNTIL(isConfigured_);

		std::cout << "...checking client size == fullscreen size" << std::endl;
		gtk_window_get_size(*this, &width, &height);
		FAIL_UNLESS_EQUAL(width, fullscreen.width);
		FAIL_UNLESS_EQUAL(height, fullscreen.height);

		std::cout << "...checking server geometry == fullscreen geometry" << std::endl;
		YIELD_UNTIL(isServerGeometry(fullscreen));

		isConfigured_ = false;
		std::cout << "...unfullscreening client" << std::endl;
		gtk_window_unfullscreen(*this);

		std::cout << "...checking for fullscreen state event" << std::endl;
		YIELD_UNTIL(not isFullscreen_);

		std::cout << "...checking for configure event" << std::endl;
		YIELD_UNTIL(isConfigured_);

		gtk_window_get_size(*this, &width, &height);

		std::cout << "...checking client size == initial size" << std::endl;
		FAIL_UNLESS_EQUAL(width, initial.width);
		FAIL_UNLESS_EQUAL(height, initial.height);

		std::cout << "...checking server geometry == initial geometry" << std::endl;
		YIELD_UNTIL(isServerGeometry(initial));
	}

	bool isServerGeometry(const Geometry what)
	{
		const Geometry geometry(getSurfaceGeometry(*this));
		return geometry.width == what.width
			and geometry.height == what.height
			and geometry.x == what.x
			and geometry.y == what.y
		;
	}

	static gboolean onState(GtkWidget *widget, GdkEvent *event, gpointer data)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		GdkEventWindowState *ev = (GdkEventWindowState*)(event);
		if (ev->changed_mask & GDK_WINDOW_STATE_FULLSCREEN) {
			test->isFullscreen_ = ev->new_window_state & GDK_WINDOW_STATE_FULLSCREEN;
			std::cout << "...got fullscreen state change " << test->isFullscreen_ << std::endl;
		}
		return gtk_false();
	}

	static gboolean onConfigure(GtkWidget *widget, GdkEvent *event, gpointer data)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		GdkEventConfigure *ev = (GdkEventConfigure*)(event);
		Geometry conf;
		conf.x = ev->x;
		conf.y = ev->y;
		conf.width = ev->width;
		conf.height = ev->height;
		test->isConfigured_ = true;
		std::cout << "...got configure event " << conf << std::endl;
		return gtk_false();
	}

	operator wl_surface *()
	{
		return gdk_wayland_window_get_wl_surface(
			gtk_widget_get_window(win_)
		);
	}

	operator GtkWindow *()
	{
		return GTK_WINDOW(win_);
	}

private:
	GtkWidget	*win_;
	bool		isFullscreen_;
	bool		isConfigured_;
};

WFITS_GTK_HARNESS_TEST_CASE(WindowFullscreenTest);

} // namespace gtk
} // namespace test
} // namespace wfits
