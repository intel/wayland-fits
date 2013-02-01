#include <vector>

#include "templates.h"

class Panel : public EvasObject
{
public:
	Panel(EvasObject &parent)
		: EvasObject::EvasObject(elm_panel_add(parent))
	{
		return;
	}
};

class PanelOrientTest : public ElmTestHarness
{
public:
	PanelOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PanelOrientTest", "Panel Orientation Test")
		, control_(window_)
		, content_(elm_label_add(window_))
	{
		elm_object_text_set(content_, "Panel");
		elm_object_content_set(control_, content_);

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		orients_.push_back(ELM_PANEL_ORIENT_TOP);
		orients_.push_back(ELM_PANEL_ORIENT_BOTTOM);
		orients_.push_back(ELM_PANEL_ORIENT_TOP);
		orients_.push_back(ELM_PANEL_ORIENT_LEFT);
		orients_.push_back(ELM_PANEL_ORIENT_TOP);
		orients_.push_back(ELM_PANEL_ORIENT_RIGHT);
		orients_.push_back(ELM_PANEL_ORIENT_TOP);
	}

	void setup()
	{
		window_.show();
		content_.show();
		control_.show();

		foreach (const Elm_Panel_Orient orient, orients_) {
			queueStep(boost::bind(elm_panel_orient_set, boost::ref(control_), orient));
			queueStep(boost::bind(&PanelOrientTest::checkOrient, boost::ref(*this), orient));
		}
	}

	void checkOrient(const Elm_Panel_Orient expected)
	{
		control_.show();
		FAIL_UNLESS_EQUAL(elm_panel_orient_get(control_), expected);
		Application::yield();
	}

private:
	Window				window_;
	Panel				control_;
	EvasObject			content_;
	std::vector<Elm_Panel_Orient>	orients_;
};

// TODO - PanelToggleTest - needs callbacks
class PanelToggleTest : public ElmTestHarness
{
public:
	PanelToggleTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PanelToggleTest", "Panel Toggle Test")
		, control_(window_)
		, content_(elm_label_add(window_))
	{
		elm_object_text_set(content_, "Panel");
		elm_object_content_set(control_, content_);

		control_.setSize(200, 100);
		control_.setPosition(50, 10);
	}

	void setup()
	{
		window_.show();
		content_.show();
		control_.show();

		queueStep(boost::bind(elm_panel_toggle, boost::ref(control_)));
		queueStep(boost::bind(&EvasObject::checkVisible, boost::ref(control_), EINA_FALSE));

		queueStep(boost::bind(&PanelToggleTest::yield, boost::ref(*this)));
		queueStep(boost::bind(&PanelToggleTest::yield, boost::ref(*this)));

		queueStep(boost::bind(elm_panel_toggle, boost::ref(control_)));
		queueStep(boost::bind(&EvasObject::checkVisible, boost::ref(control_), EINA_TRUE));
	}

	void yield(void)
	{
		Application::yield();
	}

private:
	Window				window_;
	Panel				control_;
	EvasObject			content_;
};

typedef ResizeObjectTest<Panel> PanelResizeTest;
typedef PositionObjectTest<Panel> PanelPositionTest;
typedef VisibleObjectTest<Panel> PanelVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(PanelResizeTest, "Panel")
WAYLAND_ELM_HARNESS_TEST_CASE(PanelPositionTest, "Panel")
WAYLAND_ELM_HARNESS_TEST_CASE(PanelVisibilityTest, "Panel")
WAYLAND_ELM_HARNESS_TEST_CASE(PanelOrientTest, "Panel")
//WAYLAND_ELM_HARNESS_TEST_CASE(PanelToggleTest, "Panel")

