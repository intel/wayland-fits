#include "display.h"

Display::Display()
	: wl_display_(wl_display_connect(0))
	, wl_registry_(NULL)
	, globals_()
{
	ASSERT(wl_display_ != NULL);

	wl_registry_ = wl_display_get_registry(*this);

	wl_display_set_user_data(*this, this);
	
	ASSERT(wl_registry_ != NULL);

	static const struct wl_registry_listener listener = {global};

	wl_registry_add_listener(wl_registry_, &listener, this);

	dispatch();
}

/*virtual*/ Display::~Display()
{
	wl_registry_destroy(wl_registry_);
	wl_display_disconnect(*this);
}

void Display::roundtrip() const
{
	ASSERT(wl_display_roundtrip(*this) >= 0);
}

void Display::dispatch() const
{
	ASSERT(wl_display_dispatch(*this) >= 0);
}

void Display::yield(const unsigned time) const
{
	roundtrip();
	usleep(time);
}

/*static*/ void Display::global(
	void *data, struct wl_registry *wl_registry, uint32_t id,
	const char* interface, uint32_t version)
{
	Display* display = static_cast<Display*>(data);

	ASSERT(display->wl_registry_ == wl_registry);

	// FIXME: With multi-display, we get multiple wl_output global
	// interface's presented to us.
	display->globals_[std::string(interface)] = GlobalInfo(id, version);
}

TEST(Display, "Core/Wrapper")
{
	Display display;

	FAIL_IF((wl_display*)display == NULL);
	FAIL_UNLESS_EQUAL(wl_display_get_user_data(display), &display);

	ASSERT(not wl_display_get_error(display));
}
