/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <Elementary.h>
#include <Ecore_Wayland.h>

#include "window.h"
#include "application.h"

namespace wfits {
namespace test {
namespace efl {

Window::Window(const std::string& name, const std::string& title, const unsigned width, const unsigned height)
	: EvasObject::EvasObject(
		elm_win_util_standard_add(name.c_str(), title.c_str())
	)
{
	setSize(width, height);
}

wl_surface* Window::get_wl_surface()
{
#if ECORE_VERSION_MAJOR == 1 && ECORE_VERSION_MINOR == 7
	return elm_win_wl_window_get(*this)->surface;
#else // ECORE 1.8
	return ecore_wl_window_surface_get(elm_win_wl_window_get(*this));
#endif
}

void Window::iconify(const Eina_Bool iconify)
{
	elm_win_iconified_set(*this, iconify);
}

Eina_Bool Window::isIconified() const
{
	return elm_win_iconified_get(*this);
}

void Window::maximize(const Eina_Bool maximize)
{
	elm_win_maximized_set(*this, maximize);
}

Eina_Bool Window::isMaximized() const
{
	return elm_win_maximized_get(*this);
}

void Window::fullscreen(const Eina_Bool fullscreen)
{
	elm_win_fullscreen_set(*this, fullscreen);
}

Eina_Bool Window::isFullscreen() const
{
	return elm_win_fullscreen_get(*this);
}

void Window::sticky(const Eina_Bool sticky)
{
	elm_win_sticky_set(*this, sticky);
}

Eina_Bool Window::isSticky() const
{
	return elm_win_sticky_get(*this);
}

void Window::withdrawn(const Eina_Bool withdraw)
{
	elm_win_withdrawn_set(*this, withdraw);
}

Eina_Bool Window::isWithdrawn() const
{
	return elm_win_withdrawn_get(*this);
}

void Window::rotate(const int degrees)
{
	elm_win_rotation_set(*this, degrees);
}

int Window::getRotation() const
{
	return elm_win_rotation_get(*this);
}

void Window::checkIconified(const Eina_Bool isIconified) const
{
	FAIL_UNLESS_EQUAL(this->isIconified(), isIconified);
}

void Window::checkMaximized(const Eina_Bool isMaximized) const
{
	FAIL_UNLESS_EQUAL(this->isMaximized(), isMaximized);
}

void Window::checkFullscreen(const Eina_Bool isFullscreen) const
{
	FAIL_UNLESS_EQUAL(this->isFullscreen(), isFullscreen);
}

void Window::checkSticky(const Eina_Bool isSticky) const
{
	FAIL_UNLESS_EQUAL(this->isSticky(), isSticky);
}

void Window::checkWithdrawn(const Eina_Bool isWithdrawn) const
{
	FAIL_UNLESS_EQUAL(this->isWithdrawn(), isWithdrawn);
}

void Window::checkRotation(const int expected) const
{
	FAIL_UNLESS_EQUAL(this->getRotation(), expected);
}

} // namespace efl
} // namespace test
} // namespace wfits
