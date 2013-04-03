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

	Eina_Bool isIconified() const;
	Eina_Bool isMaximized() const;
	Eina_Bool isFullscreen() const;
	Eina_Bool isSticky() const;
	Eina_Bool isWithdrawn() const;

	int getRotation() const;
	
	void checkIconified(const Eina_Bool isIconified) const;
	void checkMaximized(const Eina_Bool isMaximized) const;
	void checkFullscreen(const Eina_Bool isFullscreen) const;
	void checkSticky(const Eina_Bool isSticky) const;
	void checkWithdrawn(const Eina_Bool isWithdrawn) const;
	void checkRotation(const int expected) const;

	wl_surface* get_wl_surface();
};

#endif

