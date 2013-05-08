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

class WindowResizeTest : public ElmTestHarness
{
public:
	WindowResizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowResizeTest", "Window Resize Test")
		, resizeDone_(false)
		, rendered_(false)
	{
		return;
	}

	void setup()
	{
		evas_event_callback_add(evas_object_evas_get(window_), EVAS_CALLBACK_RENDER_POST, onPostRender, this);
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);

		window_.show();

		queueStep(boost::bind(&WindowResizeTest::test, boost::ref(*this)));
	}

	void test()
	{
		YIELD_UNTIL(rendered_);

		testResize(-10, -10);
		testResize(-1, 10);
		testResize(10, -1);
		testResize(0, 0);

		for (int w(1); w <= 400; w += 89) {
			for (int h(3); h <= 400; h += 91) {
				testResize(w, h);
			}
		}

		testResize(3000, 3000);
	}

	void testResize(int width, int height)
	{
		resizeDone_ = false;

		std::cout << "...resizing to " << width << "x" << height << std::endl;
		window_.setSize(width, height);

		std::cout << "...checking for resize event" << std::endl;
		YIELD_UNTIL(resizeDone_);

		std::cout << "...checking client size == " << std::max(0, width) << "x" << std::max(0, height) << std::endl;
		ASSERT(
			window_.getWidth() == std::max(0, width)
			and window_.getHeight() == std::max(0, height)
		);

		std::cout << "...checking server size" << std::endl;
		YIELD_UNTIL(serverSizeIsEqual());
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		WindowResizeTest *test = static_cast<WindowResizeTest*>(data);
		test->rendered_ = true;
		std::cout << "...got post render event" << std::endl;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowResizeTest *test = static_cast<WindowResizeTest*>(data);
		test->resizeDone_ = true;
		std::cout << "...got resize event" << std::endl;
	}

	bool serverSizeIsEqual()
	{
		const Geometry geometry(getSurfaceGeometry(window_.get_wl_surface()));
		const Geometry fg(window_.getFramespaceGeometry());

		return window_.getWidth() + fg.width == geometry.width
			and window_.getHeight() + fg.height == geometry.height;
	}

private:
	Window	window_;
	bool	resizeDone_;
	bool	rendered_;
};

WFITS_EFL_HARNESS_TEST_CASE(WindowResizeTest)

} // namespace efl
} // namespace test
} // namespace wfits
