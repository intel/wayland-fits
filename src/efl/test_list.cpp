#include <vector>
#include <string>

#include "templates.h"

class List : public EvasObject
{
public:
	List(EvasObject &parent)
		: EvasObject::EvasObject(elm_list_add(parent))
	{
		return;
	}
};

//TODO: more rigorous tests

typedef ResizeObjectTest<List> ListResizeTest;
typedef PositionObjectTest<List> ListPositionTest;
typedef VisibleObjectTest<List> ListVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(ListResizeTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListPositionTest, "List")
WAYLAND_ELM_HARNESS_TEST_CASE(ListVisibilityTest, "List")

