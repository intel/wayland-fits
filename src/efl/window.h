#ifndef __WAYLAND_EFL_WINDOW_H__
#define __WAYLAND_EFL_WINDOW_H__

#include <string>

#include "evasobject.h"

class Window : public EvasObject
{
public:
	Window(const std::string& = "name", const std::string& = "title", const unsigned = 400, const unsigned = 300);
	
	void iconify(Eina_Bool iconify);
	void maximize(Eina_Bool maximize);
	void fullscreen(Eina_Bool fullscreen);
	void sticky(Eina_Bool sticky);
	void withdrawn(Eina_Bool withdraw);

	Eina_Bool isIconified();
	Eina_Bool isMaximized();
	Eina_Bool isFullscreen();
	Eina_Bool isSticky();
	Eina_Bool isWithdrawn();
};

#endif

