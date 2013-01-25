#include "shell_surface.h"

ShellSurface::ShellSurface(const Shell& shell, const Surface& surface)
	: shell_(shell)
	, surface_(surface)
	, wl_shell_surface_(
		wl_shell_get_shell_surface(shell, surface))
{
	ASSERT(wl_shell_surface_ != NULL);

	wl_shell_surface_set_user_data(*this, this);
	
	static const wl_shell_surface_listener listener = {
		ping, configure, popupDone};

	wl_shell_surface_add_listener(*this, &listener, this);

	wl_shell_surface_set_toplevel(*this);

	shell.display().roundtrip();
}

ShellSurface::~ShellSurface()
{
	wl_shell_surface_destroy(*this);
}

/*static*/ void ShellSurface::ping(
	void *data, wl_shell_surface *wl_shell_surface, uint32_t serial)
{
	ShellSurface* shellSurface = static_cast<ShellSurface*>(data);
	ASSERT(wl_shell_surface == *shellSurface);

	wl_shell_surface_pong(wl_shell_surface, serial);
}

/*static*/ void ShellSurface::configure(
	void *data, wl_shell_surface *wl_shell_surface, uint32_t edges,
	int32_t width, int32_t height)
{
	ShellSurface* shellSurface = static_cast<ShellSurface*>(data);
	ASSERT(wl_shell_surface == *shellSurface);
}

/*static*/ void ShellSurface::popupDone(
	void *data, wl_shell_surface *wl_shell_surface)
{
	ShellSurface* shellSurface = static_cast<ShellSurface*>(data);
	ASSERT(wl_shell_surface == *shellSurface);
}

TEST(ShellSurface, "Core/Wrapper")
{
	Display display;
	Compositor compositor(display);
	Shell shell(display);
	Surface surface(compositor);
	ShellSurface ss(shell, surface);

	FAIL_UNLESS_EQUAL(&ss.shell(), &shell);
	FAIL_UNLESS_EQUAL(&ss.surface(), &surface);
	FAIL_IF((wl_shell_surface*)ss == NULL);
	FAIL_UNLESS_EQUAL(wl_shell_surface_get_user_data(ss), &ss);
}

