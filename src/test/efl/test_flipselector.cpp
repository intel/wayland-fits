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
	}

	void setup()
	{
		Elm_Object_Item* pivot(
			elm_flipselector_item_append(
				control_, "PIVOT", NULL, NULL
			)
		);
		
		FAIL_IF_EQUAL(pivot, static_cast<Elm_Object_Item*>(NULL));
		
		vector<Elm_Object_Item*> items;

		foreach (const std::string& label, strings_) {
			Elm_Object_Item* item = elm_flipselector_item_append(control_, label.c_str(), NULL, NULL);
			FAIL_IF_EQUAL(item, static_cast<Elm_Object_Item*>(NULL));
			if (item != NULL) {
				items.push_back(item);
			}
		}

		window_.show();
		control_.show();

		control_.setSize(200, 200);
		control_.setPosition(50, 10);

		foreach_reverse (Elm_Object_Item* item, items) {
			queueStep(boost::bind(elm_flipselector_item_selected_set, item, EINA_TRUE));
			queueStep(boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, pivot, EINA_TRUE));

			queueStep(boost::bind(elm_flipselector_item_selected_set, pivot, EINA_TRUE));
			queueStep(boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, pivot, EINA_FALSE));

			queueStep(boost::bind(elm_flipselector_item_selected_set, item, EINA_TRUE));
			queueStep(boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, pivot, EINA_TRUE));
		}

	}

	void checkItem(const Elm_Object_Item* item, const Elm_Object_Item* pivot, Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_flipselector_item_selected_get(item), expected);
		FAIL_UNLESS_EQUAL(elm_flipselector_item_selected_get(pivot), not expected);
	}

private:
	Window		window_;
	Flipselector	control_;
	vector<string>	strings_;
};

typedef ResizeObjectTest<Flipselector> FlipSelectorResizeTest;
typedef PositionObjectTest<Flipselector> FlipSelectorPositionTest;
typedef VisibleObjectTest<Flipselector> FlipSelectorVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorItemTest, "FlipSelector")
WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorResizeTest, "FlipSelector")
WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorPositionTest, "FlipSelector")
WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorVisibilityTest, "FlipSelector")

