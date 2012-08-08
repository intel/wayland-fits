#include <vector>
#include <string>

#include "templates.h"

class List : public EvasObject
{
public:
	List(EvasObject &parent)
		: EvasObject::EvasObject(elm_list_add(parent))
	{

	}
};

//TODO: more rigorous tests

typedef ResizeObjectTest<List> ListResizeTest;
typedef PositionObjectTest<List> ListPositionTest;
typedef VisibleObjectTest<List> ListVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(List)

		WAYLAND_ELM_HARNESS_TEST_CASE(ListResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ListPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ListVisibilityTest)

	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

