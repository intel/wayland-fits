#include "templates.h"

class Map : public EvasObject
{
public:
	Map(EvasObject &parent)
		: EvasObject::EvasObject(elm_map_add(parent))
	{
		return;
	}
};

//TODO: more rigorous tests

typedef ResizeObjectTest<Map> MapResizeTest;
typedef PositionObjectTest<Map> MapPositionTest;
typedef VisibleObjectTest<Map> MapVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Map)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(MapResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(MapPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(MapVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

