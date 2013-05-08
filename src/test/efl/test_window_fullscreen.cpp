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

class WindowFullscreenTest : public ElmTestHarness
{
public:
	WindowFullscreenTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowFullscreenTest", "Window Fullscreen Test")
		, geometry_()
		, fullscreenDone_(false)
		, unfullscreenDone_(false)
		, configureDone_(false)
		, resizeDone_(false)
		, rendered_(false)
	{
		return;
	}

	void setup()
	{
		evas_event_callback_add(evas_object_evas_get(window_), EVAS_CALLBACK_RENDER_POST, onPostRender, this);
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);
		ecore_event_handler_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, onConfigure, this);

		evas_object_smart_callback_add(window_, "fullscreen", onFullscreen, this);
		evas_object_smart_callback_add(window_, "unfullscreen", onUnFullscreen, this);

		window_.show();

		for(unsigned i(0); i < 5; ++i) {
			queueStep(boost::bind(&WindowFullscreenTest::test, boost::ref(*this)));
		}
	}

	void test()
	{
		static bool initial(true);

		fullscreenDone_		= false;
		configureDone_		= false;
		resizeDone_		= false;
		unfullscreenDone_	= false;

		YIELD_UNTIL(rendered_);

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

		ASSERT(not fullscreenDone_);
		ASSERT(not configureDone_);
		ASSERT(not resizeDone_);

		std::cout << "...setting fullscreen" << std::endl;
		window_.fullscreen(EINA_TRUE);

		std::cout << "...checking for fullscreen events" << std::endl;
		YIELD_UNTIL(fullscreenDone_ and configureDone_ and resizeDone_);

		window_.checkFullscreen(EINA_TRUE);

		std::cout << "...checking client geometry == server geometry == screen geometry" << std::endl;
		YIELD_UNTIL(isScreenGeometry());

		configureDone_ = false;
		resizeDone_ = false;

		ASSERT(not unfullscreenDone_);

		std::cout << "...setting unfullscreen" << std::endl;
		window_.fullscreen(EINA_FALSE);

		std::cout << "...checking for unfullscreen events" << std::endl;
		YIELD_UNTIL(unfullscreenDone_ and configureDone_ and resizeDone_);

		window_.checkFullscreen(EINA_FALSE);

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

	bool isScreenGeometry()
	{
		int sx, sy, sw, sh;
		elm_win_screen_size_get(window_, &sx, &sy, &sw, &sh);
		const Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return g.x == sx
			and g.y == sy
			and g.width == sw
			and g.height == sh
			and window_.getX() == sx
			and window_.getY() == sy
			and window_.getWidth() == sw
			and window_.getHeight() == sh
		;
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->rendered_ = true;
		std::cout << "...got post render event" << std::endl;
	}

	static Eina_Bool onConfigure(void *data, int type, void *event)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		Ecore_Wl_Event_Window_Configure *ev = static_cast<Ecore_Wl_Event_Window_Configure *>(event);
		test->configureDone_ = true;
		std::cout << "...got configure event: "
			<< ev->x << ","
			<< ev->y << " "
			<< ev->w << "x"
			<< ev->h << std::endl;
		return ECORE_CALLBACK_PASS_ON;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->resizeDone_ = true;
		std::cout << "...got resize event" << std::endl;
	}

	static void onFullscreen(void* data, Evas_Object *obj, void* event_info)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->fullscreenDone_ = true;
		std::cout << "...got fullscreen event" << std::endl;
	}

	static void onUnFullscreen(void *data, Evas_Object*, void*)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->unfullscreenDone_ = true;
		std::cout << "...got unfullscreen event" << std::endl;
	}
	
private:
	Window		window_;
	Geometry	geometry_;
	bool		fullscreenDone_;
	bool		unfullscreenDone_;
	bool		configureDone_;
	bool		resizeDone_;
	bool		rendered_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(WindowFullscreenTest, "Window")

} // namespace efl
} // namespace test
} // namespace wfits
