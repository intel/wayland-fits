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

WAYLAND_ELM_HARNESS_TEST_CASE(ButtonResizeTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonPositionTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonVisibilityTest, "Button")

