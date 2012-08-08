#include "templates.h"

class Label : public EvasObject
{
public:
	Label(EvasObject &parent)
		: EvasObject::EvasObject(elm_label_add(parent))
	{
		elm_object_text_set(*this, "Label");
	}
};

//TODO: more rigorous tests

typedef ResizeObjectTest<Label> LabelResizeTest;
typedef PositionObjectTest<Label> LabelPositionTest;
typedef VisibleObjectTest<Label> LabelVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Label)

		WAYLAND_ELM_HARNESS_TEST_CASE(LabelResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(LabelPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(LabelVisibilityTest)

	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

