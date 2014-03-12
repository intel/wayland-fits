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
		, initialGeometry_()
		, fullscreenDone_(false)
		, unfullscreenDone_(false)
		, configureDone_(false)
		, resizeDone_(false)
	{
		return;
	}

	void setup()
	{
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);
		ecore_event_handler_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, onConfigure, this);

		evas_object_smart_callback_add(window_, "fullscreen", onFullscreen, this);
		evas_object_smart_callback_add(window_, "unfullscreen", onUnFullscreen, this);

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
			fullscreenDone_		= false;
			configureDone_		= false;
			resizeDone_		= false;
			unfullscreenDone_	= false;

			TEST_LOG("asserting client geometry == server geometry == initial server geometry");
			ASSERT(isInitialGeometry());

			ASSERT(not fullscreenDone_);
			ASSERT(not configureDone_);
			ASSERT(not resizeDone_);

			TEST_LOG("setting fullscreen");
			window_.fullscreen(EINA_TRUE);

			TEST_LOG("checking for fullscreen events");
			YIELD_UNTIL(fullscreenDone_ and configureDone_ and resizeDone_);

			TEST_LOG("checking client fullscreen attributes");
			window_.checkFullscreen(EINA_TRUE);

			TEST_LOG("checking client geometry == server geometry == screen geometry");
			YIELD_UNTIL(isScreenGeometry());

			configureDone_ = false;
			resizeDone_ = false;
			ASSERT(not unfullscreenDone_);

			TEST_LOG("setting unfullscreen");
			window_.fullscreen(EINA_FALSE);

			TEST_LOG("checking for unfullscreen events");
			YIELD_UNTIL(unfullscreenDone_ and configureDone_ and resizeDone_);

			TEST_LOG("checking client unfullscreen attributes");
			window_.checkFullscreen(EINA_FALSE);

			TEST_LOG("checking client geometry == server geometry == initial server geometry");
			YIELD_UNTIL(isInitialGeometry());
		}
	}

	bool isInitialGeometry()
	{
		const Geometry sg(getSurfaceGeometry(window_.get_wl_surface()));
		const Geometry fg(window_.getFramespaceGeometry());
	
		return sg.x == initialGeometry_.x
			and sg.y == initialGeometry_.y
			and sg.width == initialGeometry_.width
			and sg.height == initialGeometry_.height
			and window_.getWidth() + fg.width == initialGeometry_.width
			and window_.getHeight() + fg.height == initialGeometry_.height
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

	static Eina_Bool onConfigure(void *data, int type, void *event)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		Ecore_Wl_Event_Window_Configure *ev = static_cast<Ecore_Wl_Event_Window_Configure *>(event);
		test->configureDone_ = true;
		TEST_LOG(
			"got configure event: "
			<< ev->x << ","
			<< ev->y << " "
			<< ev->w << "x"
			<< ev->h
		);
		return ECORE_CALLBACK_PASS_ON;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->resizeDone_ = true;
		TEST_LOG("got resize event");
	}

	static void onFullscreen(void* data, Evas_Object *obj, void* event_info)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->fullscreenDone_ = true;
		TEST_LOG("got fullscreen event");
	}

	static void onUnFullscreen(void *data, Evas_Object*, void*)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->unfullscreenDone_ = true;
		TEST_LOG("got unfullscreen event");
	}

private:
	Window		window_;
	Geometry	initialGeometry_;
	bool		fullscreenDone_;
	bool		unfullscreenDone_;
	bool		configureDone_;
	bool		resizeDone_;
};

WFITS_EFL_HARNESS_TEST_CASE(WindowFullscreenTest)

} // namespace efl
} // namespace test
} // namespace wfits
