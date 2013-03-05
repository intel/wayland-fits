#ifndef __WAYLAND_EFL_WINDOW_H__
#define __WAYLAND_EFL_WINDOW_H__

#include <string>

#include "evasobject.h"

struct wl_surface;

class Window : public EvasObject
{
public:
	Window(const std::string& = "name", const std::string& = "title", const unsigned = 400, const unsigned = 300);
	
	void iconify(const Eina_Bool iconify);
	void maximize(const Eina_Bool maximize);
	void fullscreen(const Eina_Bool fullscreen);
	void sticky(const Eina_Bool sticky);
	void withdrawn(const Eina_Bool withdraw);
	void rotate(const int degrees);

	const Eina_Bool isIconified();
	const Eina_Bool isMaximized();
	const Eina_Bool isFullscreen();
	const Eina_Bool isSticky();
	const Eina_Bool isWithdrawn();
	const int getRotation();

	void checkIconified(const Eina_Bool isIconified);
	void checkMaximized(const Eina_Bool isMaximized);
	void checkFullscreen(const Eina_Bool isFullscreen);
	void checkSticky(const Eina_Bool isSticky);
	void checkWithdrawn(const Eina_Bool isWithdrawn);
	void checkRotation(const int expected);

	wl_surface* get_wl_surface();
	
};

#endif

