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

#include <boost/bind.hpp>
#include <wayland-cursor.h>
#include "harness.h"

/** This is as defined in wayland/cursor/wayland-cursor.c **/
struct wl_cursor_theme {
	unsigned int cursor_count;
	struct wl_cursor **cursors;
	struct wl_shm *shm;
	struct shm_pool *pool;
	char *name;
	int size;
};

class ThemeLoadTest : public CoreTestHarness
{
public:
	ThemeLoadTest()
		: CoreTestHarness::CoreTestHarness()
		, shm_(display().bind<wl_shm>("wl_shm", &wl_shm_interface))
	{
		ASSERT(shm_ != NULL);
	}

	void setup()
	{
		queueStep(
			boost::bind(&ThemeLoadTest::test, boost::ref(*this), "default", "default")
		);
		queueStep(
			boost::bind(&ThemeLoadTest::test, boost::ref(*this), "this_theme_does_not_exist", "default")
		);
	}

	void test(const std::string load, const std::string expect)
	{
		wl_cursor_theme* theme(
			wl_cursor_theme_load(load.c_str(), 32, shm_)
		);
		FAIL_IF_EQUAL(theme, NULL);
		FAIL_UNLESS(expect == std::string(theme->name));

		wl_cursor_theme_destroy(theme);
	}

private:
	wl_shm*	shm_;
};

WFITS_CORE_HARNESS_TEST_CASE(ThemeLoadTest, "Cursor");

