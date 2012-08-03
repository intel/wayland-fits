#include "templates.h"

class Inwin : public EvasObject
{
public:
	Inwin(EvasObject &parent)
		: EvasObject::EvasObject(elm_win_inwin_add(parent))
	{
	}
};

//TODO: more rigorous tests

typedef ResizeObjectTest<Inwin> InwinResizeTest;
typedef PositionObjectTest<Inwin> InwinPositionTest;
typedef VisibleObjectTest<Inwin> InwinVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Inwin)

		WAYLAND_ELM_HARNESS_TEST_CASE(InwinResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(InwinPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(InwinVisibilityTest)

	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

