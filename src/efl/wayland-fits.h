#ifndef __WAYLAND_EFL_WAYLAND_FITS_H__
#define __WAYLAND_EFL_WAYLAND_FITS_H__

#include <Ecore.h>
#include <Ecore_Wayland.h>
#include "extensions/protocol/wayland-fits-client-protocol.h"
#include "common/util.h"

class WaylandFits
{
public:
	WaylandFits();

	virtual ~WaylandFits();

	class QueryRequest
	{
	public:
		QueryRequest();
		bool done;
		void* data;
	};

	QueryRequest* makeGeometryRequest(wl_surface*) const;
	QueryRequest* makeGlobalPointerPositionRequest() const;

	void setGlobalPointerPosition(int32_t, int32_t) const;
	
	void inputKeySend(uint32_t, uint32_t) const;

private:
	void ensureBound() const;
	mutable wfits_input*	wfits_input_;
	mutable wfits_query*	wfits_query_;
};

#endif
