#ifndef __WFITS_CORE_QUERY_H__
#define __WFITS_CORE_QUERY_H__

#include "display.h"
#include "surface.h"
#include "extensions/protocol/wayland-fits-client-protocol.h"

class Query
{
public:
	Query(const Display&);
	virtual ~Query();

	operator wfits_query*() const { return wfits_query_; }
	const Display& display() const { return display_; }

	class Position
	{
	public:
		Position();
		bool done;
		int32_t x;
		int32_t y;
	};

	void expectGlobalPointerPosition(int32_t, int32_t) const;
	Position getGlobalPointerPosition(unsigned = 1) const;

	class Geometry
	{
	public:
		Geometry();
		bool done;
		int32_t x;
		int32_t y;
		int32_t width;
		int32_t height;
	};

	Geometry getSurfaceGeometry(const Surface& surface) const;
private:
	const Display& display_;
	wfits_query* wfits_query_;
};

#endif
