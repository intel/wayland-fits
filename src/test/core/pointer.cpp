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

#include "pointer.h"

namespace wfits {
namespace test {
namespace core {

Pointer::Pointer(const Seat& seat)
	: seat_(seat)
	, focus_(NULL)
	, focusSerial_(0)
	, x_(-1)
	, y_(-1)
	, button_(0)
	, buttonState_(0)
	, axis_(0)
	, axisValue_(0.f)
	, wl_pointer_(NULL)
	, btnNotify_()
{
	ASSERT(seat.capabilities() & WL_SEAT_CAPABILITY_POINTER);

	wl_pointer_ = wl_seat_get_pointer(seat);

	ASSERT(wl_pointer_ != NULL);

	wl_pointer_set_user_data(*this, this);

	static const wl_pointer_listener listener = {
		enter, leave, motion, button, axis};

	wl_pointer_add_listener(*this, &listener, this);
}

/*virtual*/ Pointer::~Pointer()
{
	wl_pointer_destroy(*this);
}

bool Pointer::hasFocus(wl_surface* surface)
{
	return focus() == surface;
}

/*static*/ void Pointer::enter(
	void *data, wl_pointer *wl_pointer, uint32_t serial,
	wl_surface *wl_surface, wl_fixed_t x, wl_fixed_t y)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

// 	std::cout << "Pointer::enter(): " << wl_fixed_to_int(x) << " "
// 		<< wl_fixed_to_int(y) << std::endl;
	
	pointer->focus_ = wl_surface;
	pointer->focusSerial_ = serial;
	pointer->x_ = wl_fixed_to_int(x);
	pointer->y_ = wl_fixed_to_int(y);
}

/*static*/ void Pointer::leave(
	void *data, wl_pointer *wl_pointer, uint32_t serial,
	wl_surface *wl_surface)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

// 	std::cout << "Pointer::leave(): " << std::endl;
	
	pointer->focus_ = NULL;
	pointer->x_ = -1;
	pointer->y_ = -1;
}

/*static*/ void Pointer::motion(
	void *data, wl_pointer *wl_pointer, uint32_t time,
	wl_fixed_t x, wl_fixed_t y)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

// 	std::cout << "Pointer::motion(): " << wl_fixed_to_int(x) << " "
// 		<< wl_fixed_to_int(y) << std::endl;

	pointer->x_ = wl_fixed_to_int(x);
	pointer->y_ = wl_fixed_to_int(y);
}

/*static*/ void Pointer::button(
	void *data, wl_pointer *wl_pointer, uint32_t serial,
	uint32_t time, uint32_t button, uint32_t state)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

	std::cout << "Pointer::button(): " << button << " "
		<< state << std::endl;

	pointer->button_ = button;
	pointer->buttonState_ = state;

	ButtonEvent event;
	event.serial	= serial;
	event.time	= time;
	event.button	= button;
	event.state	= state;
	pointer->btnNotify_(*pointer, event);
}

/*static*/ void Pointer::axis(
	void *data, wl_pointer *wl_pointer, uint32_t time,
	uint32_t axis, wl_fixed_t value)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

	std::cout << "Pointer::axis(): " << axis << " "
		<< wl_fixed_to_double(value) << std::endl;

	pointer->axis_ = axis;
	pointer->axisValue_ = wl_fixed_to_double(value);
}

boost::signals2::connection Pointer::bind(const OnButtonCallback& callback)
{
	return btnNotify_.connect(callback);
}

namespace wrapper {

	TEST(Pointer)
	{
		Display display;
		Seat seat(display);
		Pointer pointer(seat);

		FAIL_UNLESS_EQUAL(&pointer.seat(), &seat);
		FAIL_IF((wl_pointer*)pointer == NULL);
		FAIL_UNLESS_EQUAL(wl_pointer_get_user_data(pointer), &pointer);
		FAIL_UNLESS(pointer.hasFocus(NULL));
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
