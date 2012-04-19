#ifndef __WAYLAND_WINDOW_H__
#define __WAYLAND_WINDOW_H__

#include "display.h"

namespace wayland {

class Window
{

public:
	Window(Display& display);

private:
	Display&	display_;
	int		x_;
	int		y_;
	int		width_;
	int		height_;

	GLuint		fbo_;
	GLuint		colorrbo_;
	GLuint		program_;
	GLuint		uniformRotation_;
	GLuint		pos_;
	GLuint		col_;
	EGLSurface	eglSurface_;

	struct wl_egl_window*		native_;
	struct wl_surface*		surface_;
	struct wl_shell_surface*	shellSurface_;
	struct wl_callback*		callback_;
};

} // namespace wayland

#endif
