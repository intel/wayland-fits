#include "surface.h"
#include "pointer.h"

Pointer::Pointer(const Seat& seat)
	: seat_(seat)
	, focus_(NULL)
	, x_(-1)
	, y_(-1)
	, button_(0)
	, buttonState_(0)
	, axis_(0)
	, axisValue_(0.f)
	, wl_pointer_(NULL)
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

bool Pointer::hasFocus(const Surface* surface)
{
	return focus_ == surface;
}

/*static*/ void Pointer::enter(
	void *data, wl_pointer *wl_pointer, uint32_t serial,
	wl_surface *wl_surface, wl_fixed_t x, wl_fixed_t y)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

	std::cout << "Pointer::enter(): " << wl_fixed_to_int(x) << " "
		<< wl_fixed_to_int(y) << std::endl;
	
	pointer->focus_ = static_cast<Surface*>(
		wl_surface_get_user_data(wl_surface));
	pointer->x_ = wl_fixed_to_int(x);
	pointer->y_ = wl_fixed_to_int(y);
}

/*static*/ void Pointer::leave(
	void *data, wl_pointer *wl_pointer, uint32_t serial,
	wl_surface *wl_surface)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

	std::cout << "Pointer::leave(): " << std::endl;
	
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

	std::cout << "Pointer::motion(): " << wl_fixed_to_int(x) << " "
		<< wl_fixed_to_int(y) << std::endl;

	pointer->x_ = wl_fixed_to_int(x);
	pointer->y_ = wl_fixed_to_int(y);
}

/*static*/ void Pointer::button(
	void *data, wl_pointer *wl_pointer, uint32_t serial,
	uint32_t time, uint32_t button, uint32_t state)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

	pointer->button_ = button;
	pointer->buttonState_ = state;
}

/*static*/ void Pointer::axis(
	void *data, wl_pointer *wl_pointer, uint32_t time,
	uint32_t axis, wl_fixed_t value)
{
	Pointer* pointer = static_cast<Pointer*>(data);
	ASSERT(wl_pointer == *pointer);

	pointer->axis_ = axis;
	pointer->axisValue_ = wl_fixed_to_double(value);
}

TEST(Pointer, "Core/Wrapper")
{
	Display display;
	Seat seat(display);
	Pointer pointer(seat);

	FAIL_UNLESS_EQUAL(&pointer.seat(), &seat);
	FAIL_IF((wl_pointer*)pointer == NULL);
	FAIL_UNLESS_EQUAL(wl_pointer_get_user_data(pointer), &pointer);
	FAIL_UNLESS(pointer.hasFocus(NULL));
}

