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

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Hoversel)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverselResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverselPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(HoverselVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

