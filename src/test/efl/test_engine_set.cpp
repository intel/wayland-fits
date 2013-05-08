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
#include "test/tools.h"

namespace wfits {
namespace test {
namespace efl {

namespace shm {
	TEST(SetEngineTest, "EFL/Shm")
	{
		const std::string engine("wayland_shm");

		elm_init(
			TheGlobalTestSuite::instance().argc,
			TheGlobalTestSuite::instance().argv
		);

		elm_config_preferred_engine_set(engine.c_str()); // override's ELM_ENGINE user environment setting
		elm_config_engine_set(engine.c_str());
		FAIL_UNLESS_EQUAL(engine, std::string(elm_config_preferred_engine_get()));
		FAIL_UNLESS_EQUAL(engine, std::string(elm_config_engine_get()));

		elm_shutdown();
	}
} // namespace shm

namespace egl {
	TEST(SetEngineTest, "EFL/Egl")
	{
		const std::string engine("wayland_egl");

		elm_init(
			TheGlobalTestSuite::instance().argc,
			TheGlobalTestSuite::instance().argv
		);

		elm_config_preferred_engine_set(engine.c_str()); // override's ELM_ENGINE user environment setting
		elm_config_engine_set(engine.c_str());
		FAIL_UNLESS_EQUAL(engine, std::string(elm_config_preferred_engine_get()));
		FAIL_UNLESS_EQUAL(engine, std::string(elm_config_engine_get()));

		elm_shutdown();
	}
} // namespace egl

} // namespace efl
} // namespace test
} // namespace wfits
