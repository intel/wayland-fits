#include <boost/filesystem.hpp>

#include <vector>
#include <string>

#include "templates.h"

using std::vector;
using std::string;

class Flipselector : public EvasObject
{
public:
	Flipselector(EvasObject &parent)
		: EvasObject::EvasObject(elm_flipselector_add(parent))
	{
		return;
	}
};

class FlipSelectorItemTest : public ElmTestHarness
{
public:
	FlipSelectorItemTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FlipselectorItemTest", "Flipselector Item Test")
		, control_(window_)
	{
		strings_.push_back("Nil");
		strings_.push_back("First");
		strings_.push_back("Second");
		strings_.push_back("Third");
		strings_.push_back("Fourth");
		strings_.push_back("Fifth");
		strings_.push_back("Sixth");
		strings_.push_back("Seventh");
		strings_.push_back("Eigth");
		strings_.push_back("Ninth");
		strings_.push_back("Tenth");

		return;
	}

	void setup()
	{
		vector<string>::iterator string_it;
		for (string_it = strings_.begin(); string_it != strings_.end(); string_it++)
		{
			Elm_Object_Item* item = elm_flipselector_item_append(control_, string_it->c_str(), NULL, NULL);
			BOOST_CHECK_NE(item, (Elm_Object_Item*)NULL);
			if (item != NULL)
			{
				items_.push_back(item);
			}
		}

		window_.show();
		control_.show();

		vector<Elm_Object_Item*>::reverse_iterator item_it;
		for (item_it = items_.rbegin(); item_it != items_.rend(); item_it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_flipselector_item_selected_set, boost::ref(*item_it), EINA_TRUE),
					boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), *item_it)
				)
			);
		}

	}

	void checkItem(const Elm_Object_Item* expected)
	{
		BOOST_CHECK_EQUAL(elm_flipselector_item_selected_get(expected), EINA_TRUE);
	}

private:
	Window				window_;
	Flipselector			control_;
	vector<string>			strings_;
	vector<Elm_Object_Item*>	items_;
	
};

typedef ResizeObjectTest<Flipselector> FlipSelectorResizeTest;
typedef PositionObjectTest<Flipselector> FlipSelectorPositionTest;
typedef VisibleObjectTest<Flipselector> FlipSelectorVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(FlipSelectorSuite)

		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorItemTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

