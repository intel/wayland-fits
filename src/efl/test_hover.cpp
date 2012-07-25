#include "templates.h"

class Hover : public EvasObject
{
public:
	Hover(EvasObject &parent)
		: EvasObject::EvasObject(elm_hover_add(parent))
	{
		return;
	}
};


typedef ResizeObjectTest<Hover> HoverResizeTest;
typedef PositionObjectTest<Hover> HoverPositionTest;
typedef VisibleObjectTest<Hover> HoverVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Hover)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

