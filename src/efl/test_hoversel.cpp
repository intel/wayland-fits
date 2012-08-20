#include "templates.h"

class Hoversel : public EvasObject
{
public:
	Hoversel(EvasObject &parent)
		: EvasObject::EvasObject(elm_hoversel_add(parent))
	{
		return;
	}
};


typedef ResizeObjectTest<Hoversel> HoverselResizeTest;
typedef PositionObjectTest<Hoversel> HoverselPositionTest;
typedef VisibleObjectTest<Hoversel> HoverselVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(HoverselResizeTest, "Hoversel")
WAYLAND_ELM_HARNESS_TEST_CASE(HoverselPositionTest, "Hoversel")
WAYLAND_ELM_HARNESS_TEST_CASE(HoverselVisibilityTest, "Hoversel")

