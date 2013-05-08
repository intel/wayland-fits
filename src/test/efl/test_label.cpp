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

// From "A Tale of Two Cities" by Dickens, Ch 5 (Public Domain, WikiSource)
static const char* dickens =
	"A large cask of wine had been dropped and broken, in the street. The accident "
	"had happened in getting it out of a cart; the cask had tumbled out with a run, "
	"the hoops had burst, and it lay on the stones just outside the door of the "
	"wine-shop, shattered like a walnut-shell. "
	"All the people within reach had suspended their business, or their idleness, to "
	"run to the spot and drink the wine. The rough, irregular stones of the street, "
	"pointing every way, and designed, one might have thought, expressly to lame all "
	"living creatures that approached them, had dammed it into little pools; these "
	"were surrounded, each by its own jostling group or crowd, according to its "
	"size. Some men kneeled down, made scoops of their two hands joined, and sipped, "
	"or tried to help women, who bent over their shoulders, to sip, before the wine "
	"had all run out between their fingers. Others, men and women, dipped in the "
	"puddles with little mugs of mutilated earthenware, or even with handkerchiefs "
	"from women’s heads, which were squeezed dry into infants’ mouths; others made "
	"small mud-embankments, to stem the wine as it ran; others, directed by "
	"lookers-on up at high windows, darted here and there, to cut off little streams "
	"of wine that started away in new directions; others devoted themselves to the "
	"sodden and lee-dyed pieces of the cask, licking, and even champing the moister "
	"wine-rotted fragments with eager relish. There was no drainage to carry off the "
	"wine, and not only did it all get taken up, but so much mud got taken up along "
	"with it, that there might have been a scavenger in the street, if anybody "
	"acquainted with it could have believed in such a miraculous presence.";

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

		elm_object_text_set(control_, dickens);

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

		elm_object_text_set(control_, dickens);

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

WAYLAND_ELM_HARNESS_TEST_CASE(LabelResizeTest, "Label")
WAYLAND_ELM_HARNESS_TEST_CASE(LabelPositionTest, "Label")
WAYLAND_ELM_HARNESS_TEST_CASE(LabelVisibilityTest, "Label")
WAYLAND_ELM_HARNESS_TEST_CASE(LabelWrapTest, "Label")
WAYLAND_ELM_HARNESS_TEST_CASE(LabelWrapWidthTest, "Label")

} // namespace efl
} // namespace test
} // namespace wfits
