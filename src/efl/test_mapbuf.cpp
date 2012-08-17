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

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Mapbuf)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(MapbufResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(MapbufPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(MapbufVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

