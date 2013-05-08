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

using std::vector;
using std::string;

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
public:
	IconStandardTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("IconStandardTest", "Icon Standard Test")
		, control_(window_)
	{
		icons_.push_back("home");
		icons_.push_back("close");
		icons_.push_back("apps");
		icons_.push_back("arrow_up");
		icons_.push_back("arrow_down");
		icons_.push_back("arrow_left");
		icons_.push_back("arrow_right");
		icons_.push_back("chat");
		icons_.push_back("clock");
		icons_.push_back("delete");
		icons_.push_back("edit");
		icons_.push_back("refresh");
		icons_.push_back("folder");
		icons_.push_back("file");
		icons_.push_back("menu/home");
		icons_.push_back("menu/close");
		icons_.push_back("menu/apps");
		icons_.push_back("menu/arrow_up");
		icons_.push_back("menu/arrow_down");
		icons_.push_back("menu/arrow_left");
		icons_.push_back("menu/arrow_right");
		icons_.push_back("menu/chat");
		icons_.push_back("menu/clock");
		icons_.push_back("menu/delete");
		icons_.push_back("menu/edit");
		icons_.push_back("menu/refresh");
		icons_.push_back("menu/folder");
		icons_.push_back("menu/file");
		icons_.push_back("media_player/forward");
		icons_.push_back("media_player/info");
		icons_.push_back("media_player/next");
		icons_.push_back("media_player/pause");
		icons_.push_back("media_player/play");
		icons_.push_back("media_player/prev");
		icons_.push_back("media_player/rewind");
		icons_.push_back("media_player/stop");

		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		elm_icon_standard_set(control_, "home");
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (const std::string& name, icons_) {
			queueStep(boost::bind(&IconStandardTest::setStandard, boost::ref(*this), boost::cref(name)));
			queueStep(boost::bind(&IconStandardTest::checkStandard, boost::ref(*this), boost::cref(name)));
		}
	}

	void setStandard(const string& icon_name)
	{
		FAIL_UNLESS_EQUAL(elm_icon_standard_set(control_, icon_name.c_str()), EINA_TRUE);
	}

	void checkStandard(const string& expected)
	{
		const char* actual_raw = elm_icon_standard_get(control_);
		string actual(actual_raw == NULL ? "" : actual_raw);

		FAIL_UNLESS_EQUAL(actual, expected);
	}

private:
	Window		window_;
	Icon		control_;
	vector<string>	icons_;
};

typedef ResizeObjectTest<Icon> IconResizeTest;
typedef PositionObjectTest<Icon> IconPositionTest;
typedef VisibleObjectTest<Icon> IconVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(IconResizeTest, "Icon")
WAYLAND_ELM_HARNESS_TEST_CASE(IconPositionTest, "Icon")
WAYLAND_ELM_HARNESS_TEST_CASE(IconVisibilityTest, "Icon")
WAYLAND_ELM_HARNESS_TEST_CASE(IconStandardTest, "Icon")

} // namespace efl
} // namespace test
} // namespace wfits
