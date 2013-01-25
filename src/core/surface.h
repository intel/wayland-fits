#ifndef __WFITS_CORE_SURFACE_H__
#define __WFITS_CORE_SURFACE_H__

#include "compositor.h"

class Surface
{
public:
	Surface(const Compositor&);

	virtual ~Surface();

	operator wl_surface*() const { return wl_surface_; }
	const Compositor& compositor() const { return compositor_; }

	void commit() const;

private:
	static void enter(void*, wl_surface*, wl_output*);
	static void leave(void*, wl_surface*, wl_output*);

	const Compositor&	compositor_;
	wl_surface		*wl_surface_;
};

#endif
