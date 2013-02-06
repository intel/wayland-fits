#ifndef __WAYLAND_EFL_WAYLAND_FITS_H__
#define __WAYLAND_EFL_WAYLAND_FITS_H__

#include <Ecore.h>
#include <Ecore_Wayland.h>
#include "extensions/protocol/wayland-fits-client-protocol.h"

class WaylandFits
{
public:
	WaylandFits();

	virtual ~WaylandFits();

	class Geometry
	{
	public:
		Geometry();
		int32_t x;
		int32_t y;
		int32_t width;
		int32_t height;
	};

	class QueryRequest
	{
	public:
		QueryRequest();
		bool done;
		void* data;
	};

	QueryRequest* makeGeometryRequest(wl_surface*) const;
private:
	wfits_input*		wfits_input_;
	wfits_query*		wfits_query_;
};

#endif
