#ifndef __WFITS_CORE_SHELLSURFACE_H__
#define __WFITS_CORE_SHELLSURFACE_H__

#include "shell.h"
#include "surface.h"

class ShellSurface
{
public:
	ShellSurface(const Shell&, const Surface&);

	virtual ~ShellSurface();

	operator wl_shell_surface*() const { return wl_shell_surface_; }
	const Shell& shell() const { return shell_; }
	const Surface& surface() const { return surface_; }

private:
	static void ping(
		void*, wl_shell_surface*, uint32_t);
	static void configure(
		void*, wl_shell_surface*, uint32_t, int32_t, int32_t);
	static void popupDone(
		void*, wl_shell_surface*);

	const Shell&		shell_;
	const Surface&		surface_;
	wl_shell_surface 	*wl_shell_surface_;
};

#endif
