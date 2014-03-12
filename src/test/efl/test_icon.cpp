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

#include <vector>
#include <string>

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class Icon : public EvasObject
{
public:
	Icon(EvasObject &parent)
		: EvasObject::EvasObject(elm_icon_add(parent))
	{
		return;
	}
};

class IconStandardTest : public ElmTestHarness
{
	typedef std::vector<std::string> Icons;
public:
	IconStandardTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("IconStandardTest", "Icon Standard Test")
		, control_(window_)
		, icons_({
			"home", "close", "apps", "arrow_up", "arrow_down"
			, "arrow_left", "arrow_right", "chat", "clock", "delete"
			, "edit", "refresh", "folder", "file", "menu/home"
			, "menu/close", "menu/apps", "menu/arrow_up", "menu/arrow_down"
			, "menu/arrow_left", "menu/arrow_right", "menu/chat", "menu/clock"
			, "menu/delete", "menu/edit", "menu/refresh", "menu/folder"
			, "menu/file", "media_player/forward", "media_player/info"
			, "media_player/next", "media_player/pause", "media_player/play"
			, "media_player/prev", "media_player/rewind", "media_player/stop"
		  })
	{
		return;
	}

	void setup()
	{
		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		elm_icon_standard_set(control_, "home");

		window_.show();
		control_.show();
	}

	void test()
	{
		foreach (const std::string& name, icons_) {
			TEST_LOG("setting icon = " << name);
			Eina_Bool result(EINA_FALSE);
			synchronized(
				[this, &result, &name]() {
					result = elm_icon_standard_set(control_, name.c_str());
				}
			);
			FAIL_UNLESS_EQUAL(result, EINA_TRUE);

			TEST_LOG("checking icon attribute == " << name);
			const char *actual_raw(NULL);
			synchronized(
				[this, &actual_raw]() {
					actual_raw = elm_icon_standard_get(control_);
				}
			);
			const std::string actual(actual_raw == NULL ? "" : actual_raw);
			FAIL_UNLESS_EQUAL(actual, name);
		}
	}

private:
	Window	window_;
	Icon	control_;
	Icons	icons_;
};

typedef ResizeObjectTest<Icon> IconResizeTest;
typedef PositionObjectTest<Icon> IconPositionTest;
typedef VisibleObjectTest<Icon> IconVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(IconResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(IconPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(IconVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(IconStandardTest)

} // namespace efl
} // namespace test
} // namespace wfits
