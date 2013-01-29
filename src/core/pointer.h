#ifndef __WFITS_CORE_POINTER_H__
#define __WFITS_CORE_POINTER_H__

#include "seat.h"

class Surface;

class Pointer
{
public:
	Pointer(const Seat&);

	virtual ~Pointer();

	operator wl_pointer*() const { return wl_pointer_; }
	const Seat& seat() const { return seat_; }
	const int32_t x() const { return x_; }
	const int32_t y() const { return y_; }
	const Surface* focus() const { return focus_; }

	bool hasFocus(const Surface*);
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
	Surface*	focus_;
	int32_t		x_;
	int32_t		y_;
	uint32_t	button_;
	uint32_t	buttonState_;
	uint32_t	axis_;
	uint32_t	axisValue_;
	wl_pointer*	wl_pointer_;
};

#endif
