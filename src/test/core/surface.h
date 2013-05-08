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

#ifndef __WFITS_CORE_SURFACE_H__
#define __WFITS_CORE_SURFACE_H__

#include "compositor.h"

namespace wfits {
namespace test {
namespace core {

class Surface
{
public:
	Surface(const Compositor&);

	virtual ~Surface();

	operator wl_surface*() const { return wl_surface_; }
	const Compositor& compositor() const { return compositor_; }

	void commit() const;

private:
	static void enter(void*, wl_surface*, wl_output*);
	static void leave(void*, wl_surface*, wl_output*);

	const Compositor&	compositor_;
	wl_surface		*wl_surface_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
