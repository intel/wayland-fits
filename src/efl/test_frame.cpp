#include "templates.h"

class Frame : public EvasObject
{
public:
	Frame(EvasObject &parent)
		: EvasObject::EvasObject(elm_frame_add(parent))
	{
		return;
	}
};

class FrameGoTest : public ElmTestHarness
{
public:
	FrameGoTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FrameGoTest", "Frame Go Test")
		, control_(window_)
	{
		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		BOOST_CHECK_EQUAL(elm_frame_collapse_get(control_), EINA_FALSE);
		
		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_frame_collapse_go, boost::ref(control_), EINA_TRUE),
				boost::bind(&FrameGoTest::checkCollapse, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_frame_collapse_go, boost::ref(control_), EINA_FALSE),
				boost::bind(&FrameGoTest::checkCollapse, boost::ref(*this), EINA_FALSE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_frame_collapse_go, boost::ref(control_), EINA_TRUE),
				boost::bind(&FrameGoTest::checkCollapse, boost::ref(*this), EINA_TRUE)
			)
		);
	}

	void checkCollapse(const Eina_Bool expected)
	{
		BOOST_CHECK_EQUAL(elm_frame_collapse_get(control_), expected);
	}

private:
	Window	window_;
	Frame	control_;
};

typedef ResizeObjectTest<Frame> FrameResizeTest;
typedef PositionObjectTest<Frame> FramePositionTest;
typedef VisibleObjectTest<Frame> FrameVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Frame)

		WAYLAND_ELM_HARNESS_TEST_CASE(FrameGoTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FrameResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FramePositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FrameVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
