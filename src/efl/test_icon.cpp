#include <vector>
#include <string>

#include "templates.h"

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

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		vector<string>::iterator it;
		for (it = icons_.begin(); it != icons_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(&IconStandardTest::setStandard, boost::ref(*this), *it),
					boost::bind(&IconStandardTest::checkStandard, boost::ref(*this), *it)
				)
			);
		}
	}

	void setStandard(const string& icon_name)
	{
		BOOST_CHECK_EQUAL(elm_icon_standard_set(control_, icon_name.c_str()), EINA_TRUE);
	}

	void checkStandard(const string& expected)
	{
		const char* actual_raw = elm_icon_standard_get(control_);
		string actual(actual_raw == NULL ? "" : actual_raw);

		BOOST_CHECK_EQUAL(actual, expected);
	}

private:
	Window			window_;
	Icon			control_;
	vector<string>		icons_;
};


typedef ResizeObjectTest<Icon> IconResizeTest;
typedef PositionObjectTest<Icon> IconPositionTest;
typedef VisibleObjectTest<Icon> IconVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Icon)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(IconResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(IconPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(IconVisibilityTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(IconStandardTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

