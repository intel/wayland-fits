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

class WindowGeometryTest :  public GtkTestHarness
{
public:
	WindowGeometryTest()
		: GtkTestHarness::GtkTestHarness()
		, win_(gtk_window_new(GTK_WINDOW_TOPLEVEL))
	{
		return;
	}

	~WindowGeometryTest()
	{
		gtk_widget_destroy(win_);
	}

	void setup()
	{
		gtk_window_set_title(GTK_WINDOW(win_), fullname(*this).c_str());
		gtk_window_resize(GTK_WINDOW(win_), 360, 225);
		gtk_widget_show(win_);

		queueStep(boost::bind(&WindowGeometryTest::test, boost::ref(*this)));
	}

	void test()
	{
		wl_surface *surface = gdk_wayland_window_get_wl_surface(
			gtk_widget_get_window(win_)
		);

		ASSERT(surface != NULL);

		const Geometry geometry(getSurfaceGeometry(surface));
		FAIL_UNLESS_EQUAL(geometry.width, 360);
		FAIL_UNLESS_EQUAL(geometry.height, 225);
	}

private:
	GtkWidget *win_;
};

WFITS_GTK_HARNESS_TEST_CASE(WindowGeometryTest);

} // namespace gtk
} // namespace test
} // namespace wfits
