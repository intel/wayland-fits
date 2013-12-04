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

#include <cassert>

#include "common/util.h"
#include "weston-wfits-query.h"

namespace wfits {
namespace weston {

/*static*/ const struct wfits_query_interface QueryInterface::implementation = {
	QueryInterface::surfaceGeometry,
	QueryInterface::globalPointerPosition,
};

/*static*/ bool QueryInterface::initialized_(false);

/*static*/
void QueryInterface::init()
{
	if (initialized_) return;

	assert(
		wl_global_create(
			Globals::display(),
			&wfits_query_interface, 1,
			NULL, QueryInterface::bind
		)
	);

	initialized_ = true;
}

/*static*/
void QueryInterface::destroy()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);
}

/*static*/
void QueryInterface::bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
	struct wl_resource *resource = wl_resource_create(
		client, &wfits_query_interface, 1, id
	);

	if (not resource) {
		wl_client_post_no_memory(client);
		return;
	}

	wl_resource_set_implementation(
		resource, &QueryInterface::implementation, data, NULL
	);
}

struct geometry {
	float x, y;
	int32_t width, height;
};

static
struct geometry getSurfaceGeometry(struct weston_surface *surface)
{
	struct geometry geometry;

#if WESTON_VERSION_AT_LEAST(1, 3, 90)
	assert(wl_list_length(&surface->views) == 1);

	struct weston_view *view = container_of(
		surface->views.next, struct weston_view, surface_link);

	geometry.x = view->geometry.x;
	geometry.y = view->geometry.y;
	geometry.width = surface->width;
	geometry.height = surface->height;
#else
	geometry.x = surface->geometry.x;
	geometry.y = surface->geometry.y;
	geometry.width = surface->geometry.width;
	geometry.height = surface->geometry.height;
#endif
	return geometry;
}

/*static*/
void QueryInterface::surfaceGeometry(struct wl_client *client, struct wl_resource *resource,
		       struct wl_resource *surface_resource, uint32_t id)
{
	struct wl_resource *result = wl_resource_create(
		client, &wfits_query_result_interface, 1, id
	);

	struct weston_surface *surface = static_cast<struct weston_surface*>(
		wl_resource_get_user_data(surface_resource)
	);

	struct geometry geometry(getSurfaceGeometry(surface));

	wfits_query_result_send_surface_geometry(
		result,
		wl_fixed_from_double(geometry.x),
		wl_fixed_from_double(geometry.y),
		geometry.width,
		geometry.height
	);
}

/*static*/
void QueryInterface::globalPointerPosition(struct wl_client *client,
			      struct wl_resource *resource,
			      uint32_t id)
{
	struct wl_resource *result = wl_resource_create(
		client, &wfits_query_result_interface, 1, id
	);

	wl_fixed_t cx, cy;
	Globals::pointerXY(&cx, &cy);

	wfits_query_result_send_global_pointer_position(result, cx, cy);
}

} // namespace weston
} // namespace wfits
