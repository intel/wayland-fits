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

#include "common/util.h"
#include "tools.h"
#include "client.h"

namespace wfits {
namespace test {

/*static*/
void Client::bind_wfits(void *data, wl_registry *registry, uint32_t id,
	const char *interface, uint32_t version)
{
	Client *client = static_cast<Client*>(data);

	if (std::string(interface) == "wfits_input") {
		client->wfits_input_ = static_cast<wfits_input*>(
			wl_registry_bind(
				registry, id, &wfits_input_interface, version
			)
		);
	}
	else if (std::string(interface) == "wfits_query") {
		client->wfits_query_ = static_cast<wfits_query*>(
			wl_registry_bind(
				registry, id, &wfits_query_interface, version
			)
		);
	}
}

Client::Client(wl_display* dpy)
	: wfits_input_(NULL)
	, wfits_query_(NULL)
{
	ASSERT(NULL != dpy);

	wl_registry *reg = wl_display_get_registry(dpy);

	static const struct wl_registry_listener listener = {bind_wfits};

	wl_registry_add_listener(reg, &listener, this);

	wl_display_roundtrip(dpy);

	ASSERT(NULL != wfits_input_);
	ASSERT(NULL != wfits_query_);
}

/*virtual*/ Client::~Client()
{
	if (wfits_input_ != NULL)
		wfits_input_destroy(wfits_input_);
	if (wfits_query_ != NULL)
		wfits_query_destroy(wfits_query_);
}

Client::QueryRequest::QueryRequest()
	: done(false)
	, data(NULL)
{
	return;
}

static void
query_result_surface_geometry(void *data, wfits_query_result *result,
	wl_fixed_t x, wl_fixed_t y, int32_t w, int32_t h)
{
	Client::QueryRequest* qr = static_cast<Client::QueryRequest*>(data);
	Geometry* g = static_cast<Geometry*>(qr->data);
	g->x = wl_fixed_to_int(x);
	g->y = wl_fixed_to_int(y);
	g->width = w;
	g->height = h;
	qr->done = true;

	wfits_query_result_destroy(result);
}

Client::QueryRequest* Client::makeGeometryRequest(wl_surface* surface) const
{
	QueryRequest* request(new QueryRequest); // caller responsible for deleting
	request->data = new Geometry; // caller responsible for deleting

	static const wfits_query_result_listener listener = {
		query_result_surface_geometry,
	};

	wfits_query_result* result = wfits_query_surface_geometry(wfits_query_, surface);
	wfits_query_result_add_listener(result, &listener, request);

	return request;
}

static void
query_result_global_pointer_position(void *data, wfits_query_result *result,
	wl_fixed_t x, wl_fixed_t y)
{
	Client::QueryRequest* qr = static_cast<Client::QueryRequest*>(data);
	Position* g = static_cast<Position*>(qr->data);
	g->x = wl_fixed_to_int(x);
	g->y = wl_fixed_to_int(y);
	qr->done = true;

	wfits_query_result_destroy(result);
}

Client::QueryRequest* Client::makePointerPositionRequest() const
{
	QueryRequest* request(new QueryRequest); // caller responsible for deleting
	request->data = new Position; // caller responsible for deleting

	static const wfits_query_result_listener listener = {
		NULL,
		query_result_global_pointer_position,
	};

	wfits_query_result* result = wfits_query_global_pointer_position(wfits_query_);
	wfits_query_result_add_listener(result, &listener, request);

	return request;
}

void Client::movePointerTo(int32_t x, int32_t y) const
{
	wfits_input_move_pointer(wfits_input_, x, y);
}

void Client::movePointerTo(const Position& position) const
{
	movePointerTo(position.x, position.y);
}

void Client::sendKey(uint32_t key, uint32_t state) const
{
	wfits_input_key_send(wfits_input_, key, state);
}

} // namespace test
} // namespace client
