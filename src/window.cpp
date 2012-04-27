#include <cassert>
#include <wayland-client.h>

#include "window.h"

namespace wayland {

Window::Window(Display& display, int width, int height)
	: display_(display)
	, width_(width)
	, height_(height)
	, surface_(wl_compositor_create_surface(display_.compositor()))
	, shellSurface_(wl_shell_get_shell_surface(display_.shell(), surface_))
	, shmData_(0)
	, buffer_(
		display_.createShmBuffer(
			  width_
			, height_
			, WL_SHM_FORMAT_XRGB8888
			, &shmData_
		)
	  )
	, callback_(0)
{
	wl_shell_surface_set_toplevel(shellSurface_);
}

Window::~Window()
{
	if (callback_) {
		wl_callback_destroy(callback_);
	}

	wl_buffer_destroy(buffer_);
	wl_shell_surface_destroy(shellSurface_);
	wl_surface_destroy(surface_);
}

} // namespace wayland
