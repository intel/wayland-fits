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

class ButtonClickTest :  public GtkTestHarness
{
public:
	ButtonClickTest()
		: GtkTestHarness::GtkTestHarness()
		, win_(gtk_window_new(GTK_WINDOW_TOPLEVEL))
		, button_(gtk_button_new_with_label("Test Button!"))
		, clicked_(false)
		, pressed_(false)
		, released_(false)
	{
		return;
	}

	~ButtonClickTest()
	{
		gtk_widget_destroy(win_);
	}

	void setup()
	{
		gtk_container_add (GTK_CONTAINER(win_), button_);

		g_signal_connect(button_, "button-press-event", G_CALLBACK(onPressed), this);
		g_signal_connect(button_, "button-release-event", G_CALLBACK(onReleased), this);
		g_signal_connect(button_, "clicked", G_CALLBACK(onClicked), this);
		gtk_widget_show(button_);

		gtk_window_set_title(GTK_WINDOW(win_), fullname(*this).c_str());
		gtk_window_resize(GTK_WINDOW(win_), 360, 225);
		gtk_widget_show(win_);

		gdk_window_set_events(
			gtk_widget_get_window(win_),
			static_cast<GdkEventMask>(GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK)
		);

		queueStep(boost::bind(&ButtonClickTest::test, boost::ref(*this)));
	}

	void test()
	{
		wl_surface *surface = gdk_wayland_window_get_wl_surface(
			gtk_widget_get_window(win_)
		);
		ASSERT(surface != NULL);

		const Geometry geometry(getSurfaceGeometry(surface));
		GtkAllocation allocation;

		gtk_widget_get_allocation(button_, &allocation);

		//Place pointer in center of button
		setGlobalPointerPosition(
			geometry.x + allocation.x + allocation.width / 2,
			geometry.y + allocation.y + allocation.height / 2
		);

		ASSERT(not pressed_);

		inputKeySend(BTN_LEFT, 1);

		YIELD_UNTIL(pressed_)

		ASSERT(not released_);
		ASSERT(not clicked_);

		inputKeySend(BTN_LEFT, 0);

		YIELD_UNTIL(released_ and clicked_)
	}

	static void onClicked(GtkWidget *widget, gpointer data)
	{
		ButtonClickTest *test = static_cast<ButtonClickTest*>(data);
		test->clicked_ = true;
		std::cout << "...received click event" << std::endl;
	}

	static gboolean onPressed(GtkWidget *widget, GdkEvent *event, gpointer data)
	{
		ButtonClickTest *test = static_cast<ButtonClickTest*>(data);
		test->pressed_ = true;
		std::cout << "...received press event" << std::endl;

		return gtk_false();
	}

	static gboolean onReleased(GtkWidget *widget, GdkEvent *event, gpointer data)
	{
		ButtonClickTest *test = static_cast<ButtonClickTest*>(data);
		test->released_ = true;
		std::cout << "...received release event" << std::endl;

		return gtk_false();
	}

private:
	GtkWidget	*win_;
	GtkWidget	*button_;
	bool		clicked_;
	bool		pressed_;
	bool		released_;
};

WFITS_GTK_HARNESS_TEST_CASE(ButtonClickTest);

} // namespace gtk
} // namespace test
} // namespace wfits
