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

#ifndef __WESTON_WFITS_INPUT_H__
#define __WESTON_WFITS_INPUT_H__

#include <set>
#include "weston-wfits.h"

namespace wfits {
namespace weston {

class InputEmulator;

class InputInterface
{
	typedef std::set<uint32_t> ActiveKeys;
public:
	static void init();
	static void destroy();

	static void keySend(const uint32_t key, const uint32_t state);
	static void movePointer(const int32_t x, const int32_t y);

private:
	static void initEmulator(void *);

	static void bind(struct wl_client *, void *, uint32_t, uint32_t);
	static void unbind(struct wl_resource *);
	static void keySend(struct wl_client *, struct wl_resource *,
		uint32_t, uint32_t);
	static void movePointer(struct wl_client *, struct wl_resource *,
		int32_t, int32_t);

	static const struct wfits_input_interface implementation;

	static InputEmulator *emulator_;
	static bool initialized_;
};

} // namespace weston
} // namespace wfits

#endif
