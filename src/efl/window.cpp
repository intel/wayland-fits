#include <Elementary.h>
#include <Ecore_Wayland.h>

#include "window.h"
#include "application.h"

Window::Window(const std::string& name, const std::string& title, const unsigned width, const unsigned height)
	: EvasObject::EvasObject(
		elm_win_util_standard_add(name.c_str(), title.c_str())
	)
{
	setSize(width, height);
}

wl_surface* Window::get_wl_surface()
{
	return elm_win_wl_window_get(*this)->surface;
}

void Window::iconify(const Eina_Bool iconify)
{
	elm_win_iconified_set(*this, iconify);
}

const Eina_Bool Window::isIconified()
{
	return elm_win_iconified_get(*this);
}

void Window::maximize(const Eina_Bool maximize)
{
	elm_win_maximized_set(*this, maximize);
}

const Eina_Bool Window::isMaximized()
{
	return elm_win_maximized_get(*this);
}

void Window::fullscreen(const Eina_Bool fullscreen)
{
	elm_win_fullscreen_set(*this, fullscreen);
}

const Eina_Bool Window::isFullscreen()
{
	return elm_win_fullscreen_get(*this);
}

void Window::sticky(const Eina_Bool sticky)
{
	elm_win_sticky_set(*this, sticky);
}

const Eina_Bool Window::isSticky()
{
	return elm_win_sticky_get(*this);
}

void Window::withdrawn(const Eina_Bool withdraw)
{
	elm_win_withdrawn_set(*this, withdraw);
}

const Eina_Bool Window::isWithdrawn()
{
	return elm_win_withdrawn_get(*this);
}

void Window::rotate(const int degrees)
{
	elm_win_rotation_set(*this, degrees);
}

const int Window::getRotation()
{
	return elm_win_rotation_get(*this);
}

void Window::checkIconified(const Eina_Bool isIconified)
{
	FAIL_UNLESS_EQUAL(this->isIconified(), isIconified);
}

void Window::checkMaximized(const Eina_Bool isMaximized)
{
	FAIL_UNLESS_EQUAL(this->isMaximized(), isMaximized);
}

void Window::checkFullscreen(const Eina_Bool isFullscreen)
{
	FAIL_UNLESS_EQUAL(this->isFullscreen(), isFullscreen);
}

void Window::checkSticky(const Eina_Bool isSticky)
{
	FAIL_UNLESS_EQUAL(this->isSticky(), isSticky);
}

void Window::checkWithdrawn(const Eina_Bool isWithdrawn)
{
	FAIL_UNLESS_EQUAL(this->isWithdrawn(), isWithdrawn);
}

void Window::checkRotation(const int expected)
{
	FAIL_UNLESS_EQUAL(this->getRotation(), expected);
}
