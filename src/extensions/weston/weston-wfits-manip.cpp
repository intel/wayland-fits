/*
 * Copyright © 2014 Intel Corporation
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

#include <cassert>

#include "common/util.h"
#include "weston-wfits-manip.h"

namespace wfits {
namespace weston {

/*static*/ const struct wfits_manip_interface ManipInterface::implementation_ = {
	ManipInterface::moveSurface,
};

/*static*/ bool ManipInterface::initialized_(false);

/*static*/
void ManipInterface::init()
{
	if (initialized_) return;

	assert(
		wl_global_create(
			Globals::display(),
			&wfits_manip_interface, 1,
			NULL, ManipInterface::bind
		)
	);

	initialized_ = true;
}

/*static*/
void ManipInterface::destroy()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);
}

/*static*/
void ManipInterface::bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct wl_resource *resource = wl_resource_create(
		client, &wfits_manip_interface, 1, id
	);

	if (not resource) {
		wl_client_post_no_memory(client);
		return;
	}

	wl_resource_set_implementation(
		resource, &ManipInterface::implementation_, data, NULL
	);
}

/*static*/ void
ManipInterface::moveSurface(struct wl_client *client, struct wl_resource *resource,
	struct wl_resource *surface_resource, int32_t x, int32_t y)
{
	struct weston_surface *surface = static_cast<struct weston_surface*>(
		wl_resource_get_user_data(surface_resource)
	);
#if WESTON_SDK_AT_LEAST(1, 3, 90)
	struct weston_view *view = container_of(
		surface->views.next, struct weston_view, surface_link);
	weston_view_set_position(view, x, y);
	weston_view_update_transform(view);
#else
	weston_surface_configure(surface, x, y, surface->geometry.width, surface->geometry.height);
	weston_surface_update_transform(surface);
#endif
}

} // namespace weston
} // namespace wfits
