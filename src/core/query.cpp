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

#include "query.h"

Query::Query(const Display& display)
	: display_(display)
	, wfits_query_(
		display.bind<wfits_query>("wfits_query", &wfits_query_interface))
{
	ASSERT(wfits_query_ != NULL);

	wfits_query_set_user_data(*this, this);
}

/*virtual*/ Query::~Query()
{
	wfits_query_destroy(*this);
}

Query::Geometry::Geometry()
	: done(false)
	, x(-1)
	, y(-1)
	, width(-1)
	, height(-1)
{
	return;
}

Query::Position::Position()
	: done(false)
	, x(-1)
	, y(-1)
{
	return;
}

static void
query_result_global_pointer_position(void *data,
				     wfits_query_result *wfits_query_result,
				     wl_fixed_t x, wl_fixed_t y)
{
	Query::Position* p = static_cast<Query::Position*>(data);
	p->x = wl_fixed_to_int(x);
	p->y = wl_fixed_to_int(y);
	p->done = true;
}

static void
query_result_surface_geometry(void *data,
			      wfits_query_result *wfits_query_result,
			      wl_fixed_t x, wl_fixed_t y, int32_t w, int32_t h)
{
	Query::Geometry* g = static_cast<Query::Geometry*>(data);
	g->x = wl_fixed_to_int(x);
	g->y = wl_fixed_to_int(y);
	g->width = w;
	g->height = h;
	g->done = true;
}

static const wfits_query_result_listener listener = {
	query_result_surface_geometry,
	query_result_global_pointer_position,
};

Query::Position Query::getGlobalPointerPosition(unsigned slowness) const
{
	Query::Position pos;

	wfits_query_result *res = wfits_query_global_pointer_position(*this);
	wfits_query_result_add_listener(res, &listener, &pos);

	while (not pos.done) {
		display().yield(slowness*0.001*1e6);
	}
	return pos;
}

void Query::expectGlobalPointerPosition(const int32_t x, const int32_t y) const
{
	Query::Position pos;
	unsigned i(0);

	for (unsigned i(0); i < 20 && (x != pos.x || y != pos.y); ++i) {
		pos = getGlobalPointerPosition(i);
	}
	FAIL_UNLESS_EQUAL(x, pos.x);
	FAIL_UNLESS_EQUAL(y, pos.y);
}


Query::Geometry Query::getSurfaceGeometry(const Surface& surface) const
{
	Query::Geometry geometry;

	wfits_query_result *res = wfits_query_surface_geometry(*this, surface);
	wfits_query_result_add_listener(res, &listener, &geometry);

	while (not geometry.done) {
		display().yield();
	}
	return geometry;
}

TEST(QueryConstruct, "Core/Wrapper/Extension")
{
	Display display;
	Query query(display);
	Query::Position position;

	FAIL_UNLESS_EQUAL(&query.display(), &display);
	FAIL_IF((wfits_query*)query == NULL);
	FAIL_UNLESS_EQUAL(wfits_query_get_user_data(query), &query);
}

TEST(QueryGlobalPointerPosition, "Core/Wrapper/Extension")
{
	Display display;
	Query query(display);
	Query::Position position;

	position = query.getGlobalPointerPosition();
	FAIL_UNLESS(position.done);
	FAIL_IF_EQUAL(position.x, -1);
	FAIL_IF_EQUAL(position.y, -1);
}

TEST(QuerySurfaceGeometry, "Core/Wrapper/Extension")
{
	Display display;
	Query query(display);
	Surface surface(display);
	Query::Geometry geometry;

	geometry = query.getSurfaceGeometry(surface);
	FAIL_UNLESS(geometry.done);
	FAIL_IF_EQUAL(geometry.x, -1);
	FAIL_IF_EQUAL(geometry.y, -1);
	FAIL_IF_EQUAL(geometry.width, -1);
	FAIL_IF_EQUAL(geometry.height, -1);
}

