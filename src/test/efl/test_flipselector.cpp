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

#include <boost/filesystem.hpp>

#include <vector>
#include <string>

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

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
		, strings_()
		, items_()
		, pivot_(NULL)
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
	}

	void setup()
	{
		pivot_ = elm_flipselector_item_append(
			control_, "PIVOT", NULL, NULL
		);
		
		FAIL_IF_EQUAL(pivot_, static_cast<Elm_Object_Item*>(NULL));

		foreach (const std::string& label, strings_) {
			Elm_Object_Item* item = elm_flipselector_item_append(control_, label.c_str(), NULL, NULL);
			FAIL_IF_EQUAL(item, static_cast<Elm_Object_Item*>(NULL));
			if (item != NULL) {
				items_.push_back(item);
			}
		}

		window_.show();
		control_.show();

		control_.setSize(200, 200);
		control_.setPosition(50, 10);
	}

	void test()
	{
		foreach_reverse (Elm_Object_Item* item, items_) {
			synchronized(boost::bind(elm_flipselector_item_selected_set, item, EINA_TRUE));
			synchronized(boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, EINA_TRUE));

			synchronized(boost::bind(elm_flipselector_item_selected_set, pivot_, EINA_TRUE));
			synchronized(boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, EINA_FALSE));

			synchronized(boost::bind(elm_flipselector_item_selected_set, item, EINA_TRUE));
			synchronized(boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, EINA_TRUE));
		}
	}

	void checkItem(const Elm_Object_Item* item, Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_flipselector_item_selected_get(item), expected);
		FAIL_UNLESS_EQUAL(elm_flipselector_item_selected_get(pivot_), not expected);
	}

private:
	Window		window_;
	Flipselector	control_;
	vector<string>	strings_;
	vector<Elm_Object_Item*> items_;
	Elm_Object_Item* pivot_;
};

typedef ResizeObjectTest<Flipselector> FlipSelectorResizeTest;
typedef PositionObjectTest<Flipselector> FlipSelectorPositionTest;
typedef VisibleObjectTest<Flipselector> FlipSelectorVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(FlipSelectorItemTest)
WFITS_EFL_HARNESS_TEST_CASE(FlipSelectorResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(FlipSelectorPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(FlipSelectorVisibilityTest)

} // namespace efl
} // namespace test
} // namespace wfits
