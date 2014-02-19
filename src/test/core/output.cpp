/*
 * Copyright © 2014 Intel Corporation
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

#include "output.h"

namespace wfits {
namespace test {
namespace core {

Output::Output(const Display& display)
	: display_(display)
	, wl_output_(display.bind<wl_output>("wl_output", &wl_output_interface, 1))
	, geometry_()
{
	ASSERT(wl_output_ != NULL);

	wl_output_set_user_data(*this, this);

	static const wl_output_listener listener = {geometry, mode};
	wl_output_add_listener(*this, &listener, this);

	display.roundtrip();
}

/*virtual*/ Output::~Output()
{
	wl_output_destroy(*this);
}

/*static*/
void Output::geometry(void *data, wl_output *wl_output, int x, int y, int physical_width,
	int physical_height, int subpixel, const char *make, const char *model, int32_t transform)
{
	Output* output = static_cast<Output*>(data);

	ASSERT(wl_output == *output);

	output->geometry_.x = x;
	output->geometry_.y = y;
}

/*static*/
void Output::mode(void *data, wl_output *wl_output, uint32_t flags, int width, int height, int refresh)
{
	Output* output = static_cast<Output*>(data);

	ASSERT(wl_output == *output);

	if (flags & WL_OUTPUT_MODE_CURRENT) {
		output->geometry_.width = width;
		output->geometry_.height = height;
	}
}

namespace wrapper {

	TEST(Output)
	{
		Display display;
		Output output(display);

		FAIL_UNLESS_EQUAL(&output.display(), &display);
		FAIL_IF((wl_output*)output == NULL);
		FAIL_UNLESS_EQUAL(wl_output_get_user_data(output), &output);
		FAIL_UNLESS(output.geometry().x >= 0);
		FAIL_UNLESS(output.geometry().y >= 0);
		FAIL_UNLESS(output.geometry().width > 0);
		FAIL_UNLESS(output.geometry().height > 0);
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
