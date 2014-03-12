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
	{
		return;
	}

	void setup()
	{
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);

		window_.show();
	}

	void test()
	{
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
		TEST_LOG("resizing to " << width << "x" << height);
		resizeDone_ = false;
		window_.setSize(width, height);

		TEST_LOG("checking for resize event");
		YIELD_UNTIL(resizeDone_);

		// When resized to < 0, window should be set to 0 or 1 size
		TEST_LOG(
			"checking client size >= "
			<< std::max(0, width) << "x" << std::max(0, height)
			<< " and <= " << std::max(1, width) << "x" << std::max(1, height)
		);
		ASSERT(
			window_.getWidth() >= std::max(0, width)
			and window_.getWidth() <= std::max(1, width)
			and window_.getHeight() >= std::max(0, height)
			and window_.getHeight() <= std::max(1, height)
		);

		TEST_LOG("checking server size == client size");
		YIELD_UNTIL(serverSizeIsEqual());
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowResizeTest *test = static_cast<WindowResizeTest*>(data);
		test->resizeDone_ = true;
		TEST_LOG("got resize event");
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
};

WFITS_EFL_HARNESS_TEST_CASE(WindowResizeTest)

} // namespace efl
} // namespace test
} // namespace wfits
