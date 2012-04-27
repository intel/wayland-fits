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
	redraw(0);
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

void Window::redraw(const uint32_t time)
{
	const int end(width_ * height_);
	const int offset(time >> 4);
	uint32_t* p(static_cast<uint32_t*>(shmData_));

	for (int i(0); i < end; ++i) {
		p[i] = (i + offset) * 0x0080401;
	}

	wl_surface_attach(surface_, buffer_, 0, 0);
	wl_surface_damage(surface_, 0, 0, width_, height_);

	callback_ = wl_surface_frame(surface_);
	wl_callback_add_listener(callback_, &frameListener_, this);
}

/*static*/
const wl_callback_listener Window::frameListener_ = {
	Window::redrawcb
};

/*static*/
void Window::redrawcb(void* data, wl_callback* callback, uint32_t time)
{
	Window* window(static_cast<Window*>(data));
	window->redraw(time);

	if (callback)
		wl_callback_destroy(callback);
}


} // namespace wayland
