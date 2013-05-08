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

#ifndef __WFITS_CORE_SHELLSURFACE_H__
#define __WFITS_CORE_SHELLSURFACE_H__

#include "shell.h"
#include "surface.h"

namespace wfits {
namespace test {
namespace core {

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

} // namespace core
} // namespace test
} // namespace wfits

#endif
