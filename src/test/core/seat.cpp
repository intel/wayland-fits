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

#include "seat.h"

namespace wfits {
namespace test {
namespace core {

Seat::Seat(const Display& display)
	: display_(display)
	, wl_seat_(display.bind<wl_seat>("wl_seat", &wl_seat_interface))
	, capabilities_(0)
{
	ASSERT(wl_seat_ != NULL);

	wl_seat_set_user_data(*this, this);

	static const wl_seat_listener listener = {capabilities, name};
	wl_seat_add_listener(*this, &listener, this);

	display.roundtrip();
}

/*virtual*/ Seat::~Seat()
{
	wl_seat_destroy(*this);
}

/*static*/ void Seat::capabilities(void *data, wl_seat *wl_seat, uint32_t caps)
{
	Seat* seat = static_cast<Seat*>(data);

	ASSERT(wl_seat == *seat);

	seat->capabilities_ = caps;
}

/*static*/ void Seat::name(void *data, wl_seat *wl_seat, const char *name)
{
	return;
}

namespace wrapper {

	TEST(Seat)
	{
		Display display;
		Seat seat(display);

		FAIL_UNLESS_EQUAL(&seat.display(), &display);
		FAIL_IF((wl_seat*)seat == NULL);
		FAIL_UNLESS_EQUAL(wl_seat_get_user_data(seat), &seat);
		FAIL_IF_EQUAL(seat.capabilities(), 0u);
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
