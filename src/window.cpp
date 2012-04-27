#include <cassert>
#include <wayland-client.h>
#include <wayland-egl.h>

#include "window.h"

namespace wayland {

Window::Window(Display& display)
	: display_(display)
{
	EGLBoolean result;

	surface_ = wl_compositor_create_surface(display_.compositor());
	shellSurface_ = wl_shell_get_shell_surface(display_.shell(), surface_);
	native_ = wl_egl_window_create(surface_, width_, height_);
	eglSurface_ = eglCreateWindowSurface(display_.display(), display_.config(), native_, NULL);

	wl_shell_surface_set_toplevel(shellSurface_);

	assert(
		eglMakeCurrent(display_.display(), eglSurface_, eglSurface_, &display_.context())
		== EGL_TRUE
	);
}

} // namespace wayland
