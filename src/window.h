#ifndef __WAYLAND_WINDOW_H__
#define __WAYLAND_WINDOW_H__

#include "display.h"

namespace wayland {

class Window
{

public:
	Window(Display&, int, int);

	virtual ~Window();
private:
	Display&	display_;
	int		width_;
	int		height_;

	struct wl_surface*		surface_;
	struct wl_shell_surface*	shellSurface_;
	void*				shmData_;
	struct wl_buffer*		buffer_;
	struct wl_callback*		callback_;
};

} // namespace wayland

#endif
