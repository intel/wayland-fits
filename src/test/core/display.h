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

#ifndef __WFITS_CORE_DISPLAY_H__
#define __WFITS_CORE_DISPLAY_H__

#include <map>
#include <wayland-client.h>
#include "test/tools.h"

namespace wfits {
namespace test {
namespace core {

class Display
{
	typedef std::pair<uint32_t, uint32_t> GlobalInfo;
	typedef std::map<std::string, GlobalInfo> Globals;
public:
	Display();

	virtual ~Display();

	template <typename T>
	T* bind(
		const std::string& interface,
		const wl_interface *wl_interface) const
	{
		const Globals::const_iterator match(globals_.find(interface));

		ASSERT(match != globals_.end());

		return static_cast<T*>(
			wl_registry_bind(
				wl_registry_, match->second.first, wl_interface,
				match->second.second));
	}

	void roundtrip() const;
	void dispatch() const;
	void yield(const unsigned = 0.001 * 1e6) const;

	operator wl_display*() const { return wl_display_; }
	operator wl_registry*() const { return wl_registry_; }

private:
	static void global(
		void*, wl_registry*, uint32_t, const char*, uint32_t);

	wl_display	*wl_display_;
	wl_registry	*wl_registry_;
	Globals		globals_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
