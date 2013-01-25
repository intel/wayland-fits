#ifndef __WFITS_CORE_COMPOSITOR_H__
#define __WFITS_CORE_COMPOSITOR_H__

#include "display.h"

class Compositor
{
public:
	Compositor(const Display&);

	virtual ~Compositor();

	operator wl_compositor*() const { return wl_compositor_; }
	const Display& display() const { return display_; }

private:
	const Display&	display_;
	wl_compositor	*wl_compositor_;
};

#endif
