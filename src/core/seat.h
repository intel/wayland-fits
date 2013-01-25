#ifndef __WFITS_CORE_SEAT_H__
#define __WFITS_CORE_SEAT_H__

#include "display.h"

class Seat
{
public:
	Seat(const Display&);

	virtual ~Seat();

	operator wl_seat*() const { return wl_seat_; }
	const Display& display() const { return display_; }
	uint32_t capabilities() const { return capabilities_; }

private:
	static void capabilities(void*, wl_seat*, uint32_t);
	
	const Display&		display_;
	wl_seat*		wl_seat_;
	uint32_t		capabilities_;
};

#endif
