#include <Elementary.h>

#include "window.h"

Window::Window(const std::string& name, const std::string& title, const unsigned width, const unsigned height)
	: EvasObject::EvasObject(
		elm_win_util_standard_add(name.c_str(), title.c_str())
	)
{
	setSize(width, height);
}


void Window::iconify(Eina_Bool iconify)
{
	elm_win_iconified_set(*this, iconify);
}

Eina_Bool Window::isIconified()
{
	return elm_win_iconified_get(*this);
}

void Window::maximize(Eina_Bool maximize)
{
	elm_win_maximized_set(*this, maximize);
}

Eina_Bool Window::isMaximized()
{
	return elm_win_maximized_get(*this);
}

void Window::fullscreen(Eina_Bool fullscreen)
{
	elm_win_fullscreen_set(*this, fullscreen);
}

Eina_Bool Window::isFullscreen()
{
	return elm_win_fullscreen_get(*this);
}

void Window::sticky(Eina_Bool sticky)
{
	elm_win_sticky_set(*this, sticky);
}

Eina_Bool Window::isSticky()
{
	return elm_win_sticky_get(*this);
}

