#ifndef __WFITS_CORE_DISPLAY_H__
#define __WFITS_CORE_DISPLAY_H__

#include <wayland-client.h>
#include "common/test.h"

class Display
{
public:
	Display()
		: display_(wl_display_connect(0))
	{
		FAIL_IF(display_ == NULL);
	}

	virtual ~Display()
	{
		wl_display_disconnect(*this);
	}

	operator wl_display*() { return display_; }

private:
	wl_display* display_;
};

#endif
