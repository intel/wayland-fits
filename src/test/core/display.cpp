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

#include "display.h"

namespace wfits {
namespace test {
namespace core {

Display::Display()
	: wl_display_(wl_display_connect(0))
	, wl_registry_(NULL)
	, globals_()
	, xkbContext_(NULL)
{
	ASSERT(wl_display_ != NULL);

	wl_registry_ = wl_display_get_registry(*this);

	ASSERT(wl_registry_ != NULL);

	wl_display_set_user_data(*this, this);
	wl_registry_set_user_data(*this, this);

	static const struct wl_registry_listener listener = {global};

	wl_registry_add_listener(wl_registry_, &listener, this);

	dispatch();
}

/*virtual*/ Display::~Display()
{
	if (xkbContext_)
	{
		xkb_context_unref(xkbContext_);
	}
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

struct xkb_context *Display::xkbContext() const
{
	if (xkbContext_ == NULL)
	{
		xkbContext_ = xkb_context_new((enum xkb_context_flags) 0);
		ASSERT(xkbContext_ != NULL);
	}

	return xkbContext_;
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

namespace wrapper {

	TEST(Display)
	{
		Display display;

		FAIL_IF((wl_display*)display == NULL);
		FAIL_IF((wl_registry*)display == NULL);
		FAIL_UNLESS_EQUAL(wl_display_get_user_data(display), &display);
		FAIL_UNLESS_EQUAL(wl_registry_get_user_data(display), &display);

		ASSERT(not wl_display_get_error(display));
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
