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

#include <Elementary.h>
#include <boost/bind.hpp>

#include "window.h"

#include "elmtestharness.h"

class ColorselectorColorTest : public ElmTestHarness
{
public:
	ColorselectorColorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ColorselectorColorTest", "Colorselector Color Test")
		, selector_(elm_colorselector_add(window_))
	{
		window_.setSize(800, 600);
	}

	void setup()
	{
		selector_.show();
		window_.show();

		queueStep(boost::bind(elm_colorselector_color_set, boost::ref(selector_), 64, 64, 64, 0));
		queueStep(boost::bind(&ColorselectorColorTest::checkColor, boost::ref(*this), 64, 64, 64, 0));
	}

	void checkColor(const int r, const int g, const int b, const int a)
	{
		int red = 0, green = 0, blue = 0, alpha = 0;
		elm_colorselector_color_get(selector_, &red, &green, &blue, &alpha);

		FAIL_UNLESS(r == red && g == green && b == blue && a == alpha);
	}

private:
	Window		window_;
	EvasObject	selector_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(ColorselectorColorTest, "ColorSelector")
