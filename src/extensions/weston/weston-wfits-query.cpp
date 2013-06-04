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
		wl_display_add_global(
			Globals::display(),
			&wfits_query_interface,
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
	wl_client_add_object(
		client, &wfits_query_interface,
		&QueryInterface::implementation, id, data
	);
}

/*static*/
void QueryInterface::surfaceGeometry(struct wl_client *client, struct wl_resource *resource,
		       struct wl_resource *surface_resource, uint32_t result_id)
{
	struct wl_resource result;
	struct weston_surface *surface =
		static_cast<struct weston_surface*>(surface_resource->data);

	result.object.interface = &wfits_query_result_interface;
	result.object.id = result_id;
	result.destroy = NULL;
	result.client = client;
	result.data = NULL;

	wl_client_add_resource(client, &result);

	wfits_query_result_send_surface_geometry(
		&result,
		wl_fixed_from_double(surface->geometry.x),
		wl_fixed_from_double(surface->geometry.y),
		surface->geometry.width,
		surface->geometry.height
	);
	wl_resource_destroy(&result);
}

/*static*/
void QueryInterface::globalPointerPosition(struct wl_client *client,
			      struct wl_resource *resource,
			      uint32_t result_id)
{
	struct wl_resource result;
	struct weston_seat *seat(Globals::seat());
	wl_fixed_t cx, cy;
	Globals::pointerXY(&cx, &cy);

	result.object.interface = &wfits_query_result_interface;
	result.object.id = result_id;
	result.destroy = NULL;
	result.client = client;
	result.data = NULL;

	wl_client_add_resource(client, &result);
	wfits_query_result_send_global_pointer_position(&result, cx, cy);
	wl_resource_destroy(&result);
}

} // namespace weston
} // namespace wfits
