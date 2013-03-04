#include "templates.h"

class Inwin : public EvasObject
{
public:
	Inwin(EvasObject &parent)
		: EvasObject::EvasObject(elm_win_inwin_add(parent))
	{
		return;
	}
};

//TODO: more rigorous tests

// NOTE: inwin's cannot be resized explicitly

typedef PositionObjectTest<Inwin> InwinPositionTest;
typedef VisibleObjectTest<Inwin> InwinVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(InwinPositionTest, "Inwin")
WAYLAND_ELM_HARNESS_TEST_CASE(InwinVisibilityTest, "Inwin")

