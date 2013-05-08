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

#include "surface.h"

namespace wfits {
namespace test {
namespace core {

Surface::Surface(const Compositor& compositor)
	: compositor_(compositor)
	, wl_surface_(
		wl_compositor_create_surface(compositor))
{
	ASSERT(wl_surface_ != NULL);

	wl_surface_set_user_data(*this, this);

	static const wl_surface_listener listener = {enter, leave};

	wl_surface_add_listener(*this, &listener, this);

	compositor.display().roundtrip();
}

/*virtual*/ Surface::~Surface()
{
	wl_surface_destroy(*this);
}

static void handleDone(void *data, wl_callback *callback, uint32_t time)
{
	bool *done = static_cast<bool*>(data);
	*done = true;
	wl_callback_destroy(callback);
}

static const struct wl_callback_listener doneListener = {handleDone};

void Surface::commit() const
{
	bool		done(false);
	wl_callback*	callback(wl_surface_frame(*this));

	wl_callback_set_user_data(callback, &done);
	FAIL_IF(wl_callback_get_user_data(callback) != &done);

	wl_callback_add_listener(callback, &doneListener, &done);
	wl_surface_commit(*this);

	while (not done) {
		compositor().display().dispatch();
	}
}

/*static*/ void Surface::enter(
	void *data, wl_surface *wl_surface, wl_output *wl_output)
{
	Surface* surface = static_cast<Surface*>(data);
	ASSERT(wl_surface == *surface);
}

/*static*/ void Surface::leave(
	void *data, wl_surface *wl_surface, wl_output *wl_output)
{
	Surface* surface = static_cast<Surface*>(data);
	ASSERT(wl_surface == *surface);
}

namespace wrapper {

	TEST(Surface)
	{
		Display display;
		Compositor compositor(display);
		Surface surface(compositor);

		FAIL_UNLESS_EQUAL(&surface.compositor(), &compositor);
		FAIL_IF((wl_surface*)surface == NULL);
		FAIL_UNLESS_EQUAL(wl_surface_get_user_data(surface), &surface);
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
