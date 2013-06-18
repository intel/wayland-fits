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

#ifndef __WFITS_CORE_POINTER_H__
#define __WFITS_CORE_POINTER_H__

#include <boost/signals2.hpp>

#include "seat.h"

namespace wfits {
namespace test {
namespace core {

class ButtonEvent
{
public:
	uint32_t serial;
	uint32_t time;
	uint32_t button;
	uint32_t state;
};

class Pointer
{
	typedef boost::signals2::signal<void (const Pointer&, const ButtonEvent&)> ButtonNotify;
public:
	typedef ButtonNotify::slot_type OnButtonCallback;

	Pointer(const Seat&);

	virtual ~Pointer();

	operator wl_pointer*() const { return wl_pointer_; }
	const Seat& seat() const { return seat_; }
	const int32_t x() const { return x_; }
	const int32_t y() const { return y_; }
	const uint32_t button() const { return button_; }
	const uint32_t buttonState() const { return buttonState_; }
	wl_surface* focus() const { return focus_; }

	bool hasFocus(wl_surface*);

	boost::signals2::connection bind(const OnButtonCallback&);

private:
	static void enter(
		void*, wl_pointer*, uint32_t, wl_surface*, wl_fixed_t, wl_fixed_t);
	static void leave(
		void*, wl_pointer*, uint32_t, wl_surface*);
	static void motion(
		void*, wl_pointer*, uint32_t, wl_fixed_t, wl_fixed_t);
	static void button(
		void*, wl_pointer*, uint32_t, uint32_t, uint32_t, uint32_t);
	static void axis(
		void*, wl_pointer*, uint32_t, uint32_t, wl_fixed_t);

	const Seat&	seat_;
	wl_surface*	focus_;
	int32_t		x_;
	int32_t		y_;
	uint32_t	button_;
	uint32_t	buttonState_;
	uint32_t	axis_;
	uint32_t	axisValue_;
	wl_pointer*	wl_pointer_;

	ButtonNotify	btnNotify_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
