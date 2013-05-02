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

class WindowMaximizeTest : public ElmTestHarness
{
public:
	WindowMaximizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowMaximizeTest", "Window Maximize Test")
		, geometry_()
		, isMaximized_(false)
		, configureDone_(false)
		, resizeDone_(false)
		, rendered_(false)
	{
 		return;
	}

	void setup()
	{
		evas_event_callback_add(evas_object_evas_get(window_), EVAS_CALLBACK_RENDER_POST, onPostRender, this);
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, onResize, this);
		ecore_event_handler_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, onConfigure, this);

		evas_object_smart_callback_add(window_, "maximized", onMaximize, this);
		evas_object_smart_callback_add(window_, "unmaximized", onUnMaximize, this);

		window_.show();

		for(unsigned i(0); i < 5; ++i) {
			queueStep(boost::bind(&WindowMaximizeTest::test, boost::ref(*this)));
		}
	}

	void test()
	{
		static bool initial(true);

		YIELD_UNTIL(rendered_);

		isMaximized_ = false;
		configureDone_ = false;
		resizeDone_ = false;

		if (initial) {
			geometry_ = getSurfaceGeometry(window_.get_wl_surface());

			std::cout << "...initial server geometry is: "
				<< geometry_.x << ","
				<< geometry_.y << " "
				<< geometry_.width << "x"
				<< geometry_.height << std::endl;
			initial = false;
		}

		std::cout << "...asserting client geometry == server geometry == initial server geometry" << std::endl;
		ASSERT(isInitialGeometry());

		std::cout << "...maximizing window" << std::endl;
		window_.maximize(EINA_TRUE);

		std::cout << "...checking for events" << std::endl;
		YIELD_UNTIL(isMaximized_ and configureDone_ and resizeDone_);

		// FIXME: How do we test if the window is actually maximized (i.e. what should the size be)?
		std::cout << "...checking server geometry != initial server geometry" << std::endl;
		YIELD_UNTIL(not serverGeometryIsInitial());

		std::cout << "...checking client size == server size" << std::endl;
		YIELD_UNTIL(serverSizeIsEqual());

		configureDone_ = false;
		resizeDone_ = false;

		ASSERT(isMaximized_);

		std::cout << "...unmaximizing window" << std::endl;
		window_.maximize(EINA_FALSE);

		std::cout << "...checking for events" << std::endl;
		YIELD_UNTIL(not isMaximized_ and configureDone_ and resizeDone_);

		std::cout << "...checking client geometry == server geometry == initial server geometry" << std::endl;
		YIELD_UNTIL(isInitialGeometry());
	}

	bool isInitialGeometry()
	{
		const Geometry sg(getSurfaceGeometry(window_.get_wl_surface()));
		const Geometry fg(window_.getFramespaceGeometry());

		return sg.x == geometry_.x
			and sg.y == geometry_.y
			and sg.width == geometry_.width
			and sg.height == geometry_.height
			and window_.getWidth() + fg.width == geometry_.width
			and window_.getHeight() + fg.height == geometry_.height
		;
		// NOTE: server does not support client side positioning
	}
	
	bool serverSizeIsEqual()
	{
		const Geometry g(getSurfaceGeometry(window_.get_wl_surface()));
		const Geometry fg(window_.getFramespaceGeometry());

		return window_.getWidth() + fg.width == g.width
			and window_.getHeight() + fg.height == g.height;
	}

	bool serverGeometryIsInitial()
	{
		const Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return geometry_.x == g.x
			and geometry_.y == g.y
			and geometry_.width == g.width
			and geometry_.height == g.height;
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->rendered_ = true;
		std::cout << "...got post render event" << std::endl;
	}

	static Eina_Bool onConfigure(void *data, int type, void *event)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		Ecore_Wl_Event_Window_Configure *ev = static_cast<Ecore_Wl_Event_Window_Configure *>(event);
		test->configureDone_ = true;

		std::cout << "...got configure event: "
			<< ev->x << " "
			<< ev->y << " "
			<< ev->w << " "
			<< ev->h << std::endl;
		return ECORE_CALLBACK_PASS_ON;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->resizeDone_ = true;
		std::cout << "...got resize event" << std::endl;
	}

	static void onMaximize(void* data, Evas_Object *obj, void* event_info)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = true;
		std::cout << "...got maximize event" << std::endl;
	}

	static void onUnMaximize(void *data, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = false;
		std::cout << "...got unmaximize event" << std::endl;
	}

private:
	Window		window_;
	Geometry	geometry_;
	bool		isMaximized_;
	bool		configureDone_;
	bool		resizeDone_;
	bool		rendered_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(WindowMaximizeTest, "Window")
