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

WAYLAND_ELM_HARNESS_TEST_CASE(MapResizeTest, "Map")
WAYLAND_ELM_HARNESS_TEST_CASE(MapPositionTest, "Map")
WAYLAND_ELM_HARNESS_TEST_CASE(MapVisibilityTest, "Map")

