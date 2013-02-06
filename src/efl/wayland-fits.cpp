#include "common/test.h"
#include "wayland-fits.h"

WaylandFits::WaylandFits()
	: wfits_input_(NULL)
	, wfits_query_(NULL)
{
	ecore_wl_init(NULL);

	Ecore_Wl_Global *global;
	wl_registry *registry = ecore_wl_registry_get();
	wl_list *globals = ecore_wl_globals_get();

	ASSERT(registry != NULL);
	ASSERT(globals != NULL);

	wl_list_for_each(global, globals, link) {
		if (std::string(global->interface) == "wfits_input") {
			wfits_input_ = static_cast<wfits_input*>(
				wl_registry_bind(
					registry,
					global->id,
					&wfits_input_interface,
					global->version));
		} else if (std::string(global->interface) == "wfits_query") {
			wfits_query_ = static_cast<wfits_query*>(
				wl_registry_bind(
					registry,
					global->id,
					&wfits_query_interface,
					global->version));
		}
	}
	ASSERT(wfits_input_ != NULL);
	ASSERT(wfits_query_ != NULL);
}

/*virtual*/ WaylandFits::~WaylandFits()
{
	wfits_input_destroy(wfits_input_);
	wfits_query_destroy(wfits_query_);
	ecore_wl_shutdown();
}

WaylandFits::Geometry::Geometry()
	: x(-1)
	, y(-1)
	, width(-1)
	, height(-1)
{
	return;
}

WaylandFits::QueryRequest::QueryRequest()
	: done(false)
	, data(NULL)
{
	return;
}

static void
query_result_surface_geometry(void *data, wfits_query_result *result,
	wl_fixed_t x, wl_fixed_t y, int32_t w, int32_t h)
{
	WaylandFits::QueryRequest* qr = static_cast<WaylandFits::QueryRequest*>(data);
	WaylandFits::Geometry* g = static_cast<WaylandFits::Geometry*>(qr->data);
	g->x = wl_fixed_to_int(x);
	g->y = wl_fixed_to_int(y);
	g->width = w;
	g->height = h;
	qr->done = true;

	wfits_query_result_destroy(result);
}

WaylandFits::QueryRequest* WaylandFits::makeGeometryRequest(wl_surface* surface) const
{
	QueryRequest* request(new QueryRequest);
	request->data = new Geometry;

	static const wfits_query_result_listener listener = {
		query_result_surface_geometry,
	};

	wfits_query_result* result = wfits_query_surface_geometry(wfits_query_, surface);
	wfits_query_result_add_listener(result, &listener, request);

	return request;
}
