#include "templates.h"

class Button : public EvasObject
{
public:
	Button(EvasObject &parent)
		: EvasObject::EvasObject(elm_button_add(parent))
	{
		return;
	}
};

typedef ResizeObjectTest<Button> ButtonResizeTest;
typedef PositionObjectTest<Button> ButtonPositionTest;
typedef VisibleObjectTest<Button> ButtonVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Button)

		WAYLAND_ELM_HARNESS_TEST_CASE(ButtonResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ButtonPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ButtonVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
