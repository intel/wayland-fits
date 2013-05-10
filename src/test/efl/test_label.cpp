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

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

static const std::string lipsum(
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer nunc sapien, "
"bibendum non pharetra sed, volutpat et sapien. Mauris adipiscing arcu nec mauris "
"blandit nec imperdiet massa scelerisque. Donec interdum malesuada sapien eleifend "
"vestibulum. Vestibulum posuere elit et purus rhoncus luctus. Mauris justo "
"tellus, elementum at iaculis et, tempor vitae arcu. Sed at urna sit amet orci "
"facilisis adipiscing. Nunc cursus dictum nibh, sit amet sollicitudin velit "
"lobortis id. Phasellus ipsum justo, volutpat eu convallis gravida, malesuada "
"id lorem. Mauris ligula elit, aliquet sit amet tincidunt ac, aliquet ut est. "
"Curabitur cursus sollicitudin mauris sed malesuada. Nunc imperdiet ornare "
"magna, ac egestas leo rutrum sit amet. Nulla vel erat lacus. In at purus "
"lorem. Nam ac nisi ac dui sollicitudin adipiscing ut et urna. Vestibulum "
"bibendum, sem nec placerat pharetra, diam nunc interdum ligula, eu dignissim "
"augue sem nec dui. Etiam eu magna sapien. "
"Maecenas nulla turpis, aliquam non pretium ultricies, ullamcorper nec lacus. "
"Quisque non ligula felis, vitae vehicula lectus. Sed rutrum tellus vel lacus "
"volutpat fermentum. Fusce pretium luctus diam vitae lacinia. Sed orci enim, "
"bibendum nec cursus non, lobortis at purus. Sed sed erat nulla. Duis in ligula "
"vel nisi tincidunt condimentum quis ac dui. Quisque lacinia magna quis erat "
"ullamcorper pellentesque. Nunc id libero massa. Donec placerat erat ac massa "
"dignissim eu consequat libero suscipit. "
"Aliquam laoreet, tellus at molestie gravida, enim nisl feugiat orci, sit amet "
"auctor neque enim sit amet mi. Integer volutpat dictum tincidunt. Etiam "
"ultrices gravida metus ut ornare. Duis iaculis, risus non interdum egestas, "
"odio risus mattis lacus, eu scelerisque metus ligula a magna. Mauris semper "
"pretium rutrum. Etiam porta, massa sit amet sodales ullamcorper, ligula dolor "
"vehicula purus, non auctor ante lacus eu nisi. Nam mi dolor, hendrerit et "
"pharetra sed, aliquet a nisi. Vivamus luctus elementum urna, at vestibulum "
"turpis mattis non. Nam quis odio tortor. Aliquam nec erat elit. Donec dapibus "
"nunc pretium lacus pharetra a sagittis nisi vehicula. Duis dolor lacus, ornare "
"sed gravida eu, scelerisque et elit. Proin non tellus in ante placerat luctus. "
"Vivamus sit amet tortor augue, nec ultricies felis.");

class Label : public EvasObject
{
public:
	Label(EvasObject &parent)
		: EvasObject::EvasObject(elm_label_add(parent))
	{
		elm_object_text_set(*this, "Label");
	}
};

class LabelWrapTest : public ElmTestHarness
{
public:
	LabelWrapTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("LabelWrapTest", "Label Wrap Test")
		, control_(window_)
	{
		control_.setSize(350, 250);
		control_.setPosition(25, 25);

		elm_object_text_set(control_, lipsum.c_str());

		wraps_.push_back(ELM_WRAP_CHAR);
		wraps_.push_back(ELM_WRAP_WORD);
		wraps_.push_back(ELM_WRAP_CHAR);
		wraps_.push_back(ELM_WRAP_MIXED);
		wraps_.push_back(ELM_WRAP_CHAR);
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Elm_Wrap_Type wrap, wraps_) {
			queueStep(boost::bind(elm_label_line_wrap_set, boost::ref(control_), wrap));
			queueStep(boost::bind(&LabelWrapTest::checkWrap, boost::ref(*this), wrap));
		}
	}

	void checkWrap(const Elm_Wrap_Type wrap)
	{
		FAIL_UNLESS_EQUAL(elm_label_line_wrap_get(control_), wrap);
	}


private:
	Window				window_;
	Label				control_;
	std::vector<Elm_Wrap_Type>	wraps_;
};

class LabelWrapWidthTest : public ElmTestHarness
{
public:
	LabelWrapWidthTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("LabelWrapWidthTest", "Label Wrap Width Test")
		, control_(window_)
	{
		control_.setSize(350, 250);
		control_.setPosition(25, 25);

		elm_object_text_set(control_, lipsum.c_str());

		elm_label_line_wrap_set(control_, ELM_WRAP_MIXED);

		widths_.push_back(15);
		widths_.push_back(31);
		widths_.push_back(63);
		widths_.push_back(79);
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Evas_Coord width, widths_) {
			queueStep(boost::bind(elm_label_wrap_width_set, boost::ref(control_), width));
			queueStep(boost::bind(&LabelWrapWidthTest::checkWrapWidth, boost::ref(*this), width));
		}

	}

	void checkWrapWidth(const Evas_Coord width)
	{
		FAIL_UNLESS_EQUAL(elm_label_wrap_width_get(control_), width);
	}

private:
	Window			window_;
	Label			control_;
	std::vector<Evas_Coord>	widths_;
};

typedef ResizeObjectTest<Label> LabelResizeTest;
typedef PositionObjectTest<Label> LabelPositionTest;
typedef VisibleObjectTest<Label> LabelVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(LabelResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(LabelPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(LabelVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(LabelWrapTest)
WFITS_EFL_HARNESS_TEST_CASE(LabelWrapWidthTest)

} // namespace efl
} // namespace test
} // namespace wfits
