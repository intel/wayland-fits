#include "seat.h"

Seat::Seat(const Display& display)
	: display_(display)
	, wl_seat_(display.bind<wl_seat>("wl_seat", &wl_seat_interface))
	, capabilities_(0)
{
	ASSERT(wl_seat_ != NULL);

	wl_seat_set_user_data(*this, this);

	static const wl_seat_listener listener = {capabilities};
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

TEST(Seat, "Core/Wrapper")
{
	Display display;
	Seat seat(display);

	FAIL_UNLESS_EQUAL(&seat.display(), &display);
	FAIL_IF((wl_seat*)seat == NULL);
	FAIL_UNLESS_EQUAL(wl_seat_get_user_data(seat), &seat);
	FAIL_IF_EQUAL(seat.capabilities(), 0u);
}