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

#ifndef __WFITS_EFL_WINDOW_H__
#define __WFITS_EFL_WINDOW_H__

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

