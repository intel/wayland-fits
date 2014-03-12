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

#include "window.h"
#include "elmtestharness.h"

namespace wfits {
namespace test {
namespace efl {

class WindowMaximizeTest : public ElmTestHarness
{
public:
	WindowMaximizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowMaximizeTest", "Window Maximize Test")
		, initialGeometry_()
		, isMaximized_(false)
		, configureDone_(false)
		, resizeDone_(false)
	{
		return;
	}

	void setup()
	{
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, onResize, this);
		ecore_event_handler_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, onConfigure, this);

		evas_object_smart_callback_add(window_, "maximized", onMaximize, this);
		evas_object_smart_callback_add(window_, "unmaximized", onUnMaximize, this);

		window_.show();
	}

	void test()
	{
		initialGeometry_ = getSurfaceGeometry(window_.get_wl_surface());
		TEST_LOG(
			"initial server geometry is: "
			<< initialGeometry_.x << ","
			<< initialGeometry_.y << " "
			<< initialGeometry_.width << "x"
			<< initialGeometry_.height
		);

		for (unsigned i(0); i < 5; ++i)
		{
			isMaximized_ = false;
			configureDone_ = false;
			resizeDone_ = false;

			TEST_LOG("asserting client geometry == server geometry == initial server geometry");
			ASSERT(isInitialGeometry());

			TEST_LOG("maximizing window");
			window_.maximize(EINA_TRUE);

			TEST_LOG("checking for events");
			YIELD_UNTIL(isMaximized_ and configureDone_ and resizeDone_);

			// FIXME: How do we test if the window is actually maximized (i.e. what should the size be)?
			TEST_LOG("checking server geometry != initial server geometry");
			YIELD_UNTIL(not serverGeometryIsInitial());

			TEST_LOG("checking client size == server size");
			YIELD_UNTIL(serverSizeIsEqual());

			configureDone_ = false;
			resizeDone_ = false;

			ASSERT(isMaximized_);

			TEST_LOG("unmaximizing window");
			window_.maximize(EINA_FALSE);

			TEST_LOG("checking for events");
			YIELD_UNTIL(not isMaximized_ and configureDone_ and resizeDone_);

			TEST_LOG("checking client geometry == server geometry == initial server geometry");
			YIELD_UNTIL(isInitialGeometry());
		}
	}

	bool isInitialGeometry()
	{
		const Geometry sg(getSurfaceGeometry(window_.get_wl_surface()));
		const Geometry fg = window_.getFramespaceGeometry();

		return sg.x == initialGeometry_.x
			and sg.y == initialGeometry_.y
			and sg.width == initialGeometry_.width
			and sg.height == initialGeometry_.height
			and window_.getWidth() + fg.width == initialGeometry_.width
			and window_.getHeight() + fg.height == initialGeometry_.height
		;
		// NOTE: server does not support client side positioning
	}
	
	bool serverSizeIsEqual()
	{
		const Geometry g(getSurfaceGeometry(window_.get_wl_surface()));
		const Geometry fg = window_.getFramespaceGeometry();

		return window_.getWidth() + fg.width == g.width
			and window_.getHeight() + fg.height == g.height;
	}

	bool serverGeometryIsInitial()
	{
		const Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return initialGeometry_.x == g.x
			and initialGeometry_.y == g.y
			and initialGeometry_.width == g.width
			and initialGeometry_.height == g.height;
	}

	static Eina_Bool onConfigure(void *data, int type, void *event)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		Ecore_Wl_Event_Window_Configure *ev = static_cast<Ecore_Wl_Event_Window_Configure *>(event);
		test->configureDone_ = true;

		TEST_LOG("got configure event: "
			<< ev->x << " "
			<< ev->y << " "
			<< ev->w << " "
			<< ev->h
		);
		return ECORE_CALLBACK_PASS_ON;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->resizeDone_ = true;
		TEST_LOG("got resize event");
	}

	static void onMaximize(void* data, Evas_Object *obj, void* event_info)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = true;
		TEST_LOG("got maximize event");
	}

	static void onUnMaximize(void *data, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = false;
		TEST_LOG("got unmaximize event");
	}

private:
	Window		window_;
	Geometry	initialGeometry_;
	bool		isMaximized_;
	bool		configureDone_;
	bool		resizeDone_;
};

WFITS_EFL_HARNESS_TEST_CASE(WindowMaximizeTest)

} // namespace efl
} // namespace test
} // namespace wfits
