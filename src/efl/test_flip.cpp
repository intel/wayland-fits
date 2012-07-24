#include <boost/filesystem.hpp>

#include <vector>

#include "background.h"
#include "templates.h"

using boost::filesystem::path;
using std::vector;

// TODO: Once we instrument events, we can test the _interaction functions

class Flip : public EvasObject
{
public:
	Flip(EvasObject &parent)
		: EvasObject::EvasObject(elm_flip_add(parent))
	{
		return;
	}
};

class FlipGoTest : public ElmTestHarness
{
public:

	FlipGoTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FlipGoTest", "Flip Go Test")
		, control_(elm_flip_add(window_))
		, first_(window_)
		, second_(window_)
	{
		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		evas_object_size_hint_align_set(first_, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_size_hint_weight_set(first_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(second_, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_size_hint_weight_set(second_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		elm_object_part_content_set(control_, "front", first_);
		elm_object_part_content_set(control_, "back", second_);
		
		options_.push_back(ELM_FLIP_ROTATE_Y_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_X_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_XZ_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_YZ_CENTER_AXIS);
		options_.push_back(ELM_FLIP_CUBE_LEFT);
		options_.push_back(ELM_FLIP_CUBE_RIGHT);
		options_.push_back(ELM_FLIP_CUBE_UP);
		options_.push_back(ELM_FLIP_CUBE_DOWN);
		options_.push_back(ELM_FLIP_PAGE_LEFT);
		options_.push_back(ELM_FLIP_PAGE_RIGHT);
		options_.push_back(ELM_FLIP_PAGE_UP);
		options_.push_back(ELM_FLIP_PAGE_DOWN);

		return;
	}

	void setup()
	{
		control_.show();
		first_.show();
		second_.show();
		window_.show();

		vector<Elm_Flip_Mode>::iterator it;
		for (it = options_.begin(); it != options_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_flip_go, boost::ref(control_), *it),
					boost::bind(&FlipGoTest::checkFront, boost::ref(*this), EINA_TRUE)
				)
			);

			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_flip_go, boost::ref(control_), *it),
					boost::bind(&FlipGoTest::checkFront, boost::ref(*this), EINA_FALSE)
				)
			);
		}
	}

	void checkFront(const Eina_Bool visible)
	{
		BOOST_CHECK_EQUAL(elm_flip_front_visible_get(control_), visible);
	}

private:
	Window			window_;
	EvasObject		control_;
	Background		first_, second_;
	vector<Elm_Flip_Mode>	options_;
};

typedef ResizeObjectTest<Flip> FlipResizeTest;
typedef PositionObjectTest<Flip> FlipPositionTest;
typedef VisibleObjectTest<Flip> FlipVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(FlipSuite)

		WAYLAND_ELM_HARNESS_TEST_CASE(FlipGoTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
