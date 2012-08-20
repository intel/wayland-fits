#include "templates.h"

class Mapbuf : public EvasObject
{
public:
	Mapbuf(EvasObject &parent)
		: EvasObject::EvasObject(elm_mapbuf_add(parent))
	{
	}
};

//TODO: more rigorous tests

typedef ResizeObjectTest<Mapbuf> MapbufResizeTest;
typedef PositionObjectTest<Mapbuf> MapbufPositionTest;
typedef VisibleObjectTest<Mapbuf> MapbufVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(MapbufResizeTest, "Mapbuf")
WAYLAND_ELM_HARNESS_TEST_CASE(MapbufPositionTest, "Mapbuf")
WAYLAND_ELM_HARNESS_TEST_CASE(MapbufVisibilityTest, "Mapbuf")

