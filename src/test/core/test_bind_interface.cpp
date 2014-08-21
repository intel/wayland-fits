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

#include "harness.h"

namespace wfits {
namespace test {
namespace core {

template <typename O, const wl_interface& interface, const std::string& str_interface>
class BindInterface : public Harness
{
public:
	BindInterface()
		: Harness::Harness()
		, object_(NULL)
	{
		return;
	}

	void setup()
	{
		object_ = display().template bind<O>(str_interface, &interface);
	}

	void teardown()
	{
		FAIL_IF(object_ == NULL);
	}
private:
	O* object_;
};

#define BIND_TEST(name) \
	std::string str_iface_##name = #name; \
	typedef BindInterface<name, name##_interface, str_iface_##name> name; \
	WFITS_CORE_HARNESS_TEST_CASE(name)

namespace bind_interface {

BIND_TEST(wl_compositor)

/**
 * wl_display is no longer exposed as a global since v1.6 so disable it.
 * http://cgit.freedesktop.org/wayland/wayland/commit/?id=78d80474073b6145f6f03dd4928c3a4daf9c6b9c
 */
/* BIND_TEST(wl_display) */

BIND_TEST(wl_shm)
BIND_TEST(wl_output)
BIND_TEST(wl_seat)
BIND_TEST(wl_shell)
BIND_TEST(wl_data_device_manager)
BIND_TEST(wfits_input)
BIND_TEST(wfits_query)

} // namespace bind_interface

} // namespace core
} // namespace test
} // namespace wfits
