#include "window.h"
#include "elmtestharness.h"
#include "templates.h"
#include <vector>

#include <linux/input.h>

using std::vector;

class Button : public EvasObject
{
public:
	Button(EvasObject &parent)
		: EvasObject::EvasObject(elm_button_add(parent))
	{
		return;
	}
};

class UserButtonInteractionTest : public ElmTestHarness
{
public:
	UserButtonInteractionTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("UserButtonClickTest", "User Button Click Test")
		, button_(elm_button_add(window_))
		, clicked_(false)
		, repeated_(false)
		, pressed_(false)
		, unpressed_(false)
	{
		evas_object_resize(button_, 200, 100);
		elm_object_text_set(button_, "Test Button!");
		evas_object_smart_callback_add(button_, "clicked", onClick, this);
		evas_object_smart_callback_add(button_, "repeated", onRepeated, this);
		evas_object_smart_callback_add(button_, "pressed", onPressed, this);
		evas_object_smart_callback_add(button_, "unpressed", onUnpressed, this);
		
		elm_button_autorepeat_set(button_, EINA_TRUE);
		elm_button_autorepeat_initial_timeout_set(button_, 1.0);
		elm_button_autorepeat_gap_timeout_set(button_, 0.5);
	}

	void setup()
	{
		//place button in the center of window
		button_.setPosition(
			(window_.getWidth() / 2 - button_.getWidth() / 2),
			(window_.getHeight() / 2 - button_.getHeight() / 2)
		);
		button_.show();
		window_.show();

		Application::yield(0.01*1e6);
		Geometry geo = getSurfaceGeometry(window_.get_wl_surface());

		//Place pointer in center of button
		queueStep(
			boost::bind(
				&ElmTestHarness::setGlobalPointerPosition, boost::ref(*this),
				(geo.x + button_.getX() + button_.getWidth() / 2),
				(geo.y + button_.getY() + button_.getHeight() / 2)
				)
		);
		
		//below are all callbacks this widget makes that the user can invoke
		addEventsTest();
		
	}
	
	void addEventsTest()
	{
		queueStep(
			boost::bind(
				&ElmTestHarness::pointerKeyPress,
				boost::ref(*this), BTN_LEFT, 1
			),
			"sending button left click down"
		);
		queueStep(
			boost::bind(
			&ElmTestHarness::stepUntilCondition,
			boost::ref(*this),
			boost::lambda::bind(&UserButtonInteractionTest::pressed_, boost::ref(*this))
			),
			"checking pressed event"
		);
		queueStep(
			boost::bind(
			&ElmTestHarness::stepUntilCondition,
			boost::ref(*this),
			boost::lambda::bind(&UserButtonInteractionTest::repeated_, boost::ref(*this))
			),
			"checking repeated event"
		);
		queueStep(
			boost::bind(
				&ElmTestHarness::pointerKeyPress,
				boost::ref(*this), BTN_LEFT, 0
			),
			"sending button left click up"
		);
		queueStep(
			boost::bind(
			&ElmTestHarness::stepUntilCondition,
			boost::ref(*this),
			boost::lambda::bind(&UserButtonInteractionTest::unpressed_, boost::ref(*this))
			),
			"checking unpressed event"
		);
		queueStep(
			boost::bind(
			&ElmTestHarness::stepUntilCondition,
			boost::ref(*this),
			boost::lambda::bind(&UserButtonInteractionTest::clicked_, boost::ref(*this))
			),
			"checking clicked event"
		);
	}

	static void onClick(void* data, Evas_Object*, void*)
	{
		UserButtonInteractionTest *test = static_cast<UserButtonInteractionTest*>(data);
		test->clicked_ = true;
		std::cout << "...received click event" << std::endl;
	}
	
	static void onRepeated(void* data, Evas_Object*, void*)
	{
		UserButtonInteractionTest *test = static_cast<UserButtonInteractionTest*>(data);
		test->repeated_ = true;
		std::cout << "...received repeated event" << std::endl;
	}
	
	static void onPressed(void* data, Evas_Object*, void*)
	{
		UserButtonInteractionTest *test = static_cast<UserButtonInteractionTest*>(data);
		test->pressed_ = true;
		std::cout << "...received pressed_ event" << std::endl;
	}
	
	static void onUnpressed(void* data, Evas_Object*, void*)
	{
		UserButtonInteractionTest *test = static_cast<UserButtonInteractionTest*>(data);
		test->unpressed_ = true;
		std::cout << "...received unpressed_ event" << std::endl;
	}
	
private:
	bool		clicked_;
	bool		repeated_;
	bool		pressed_;
	bool		unpressed_;
	
	Window		window_;
	EvasObject	button_;
};


class UserButtonEventTest : public ElmTestHarness
{
public:
	
	UserButtonEventTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("UserButtonEventTest", "Button Pointer Event Test")
		, button_(elm_button_add(window_))
		, isMouseInButton_(false)
		, isMouseOutButton_(false)
	{
		evas_object_resize(button_, 200, 100);
		elm_object_text_set(button_, "Test Button!");
		evas_object_event_callback_add(button_, EVAS_CALLBACK_MOUSE_IN, onMouseInButton, this);
		evas_object_event_callback_add(button_, EVAS_CALLBACK_MOUSE_OUT, onMouseOutButton, this);
	}

	void setup()
	{
		//place button in the center of window
// 		button_.setPosition(
// 			(window_.getWidth() / 2 - button_.getWidth() / 2),
// 			(window_.getHeight() / 2 - button_.getHeight() / 2)
// 		);
// 		
		//BUG: Exposes window decoration bug
  		button_.setPosition(0, 0);
		button_.show();
		window_.show();
		
		Application::yield(0.01*1e6);
		Application::yield(1000000);
		Geometry geo = getSurfaceGeometry(elm_win_wl_window_get(window_)->surface);
		
		//upper left
		addInOutEventsTest(
			button_.getX() + geo.x,
			button_.getY() + geo.y,
			button_.getX() + geo.x,
 			button_.getY() + geo.y - 5
		);

		//upper mid
		addInOutEventsTest(
 			button_.getX() + button_.getWidth() / 2 + geo.x,
 			button_.getY() + geo.y,
 			button_.getX() + button_.getWidth() / 2 + geo.x,
 			button_.getY() + geo.y - 5
 		);
		
		//upper right
		addInOutEventsTest(
			button_.getX() + button_.getWidth() + geo.x - 1,
 			button_.getY() + geo.y,
 			button_.getX() + button_.getWidth() + geo.x,
			button_.getY() + geo.y - 5
 			
		);
		
		//mid left
		addInOutEventsTest(
			button_.getX() + geo.x,
			button_.getY() + button_.getHeight() / 2 + geo.y,
			button_.getX() + geo.x - 5,
			button_.getY() + button_.getHeight() / 2 + geo.y			
		);
		
		//mid right
		addInOutEventsTest(
			button_.getX() + button_.getWidth() + geo.x - 1,
			button_.getY() + button_.getHeight() / 2 + geo.y,
			button_.getX() + button_.getWidth() + geo.x + 5,
			button_.getY() + button_.getHeight() / 2 + geo.y				   
		);
		
		//lower left
		addInOutEventsTest(
			button_.getX() + geo.x,
			button_.getY() + button_.getHeight() + geo.y - 1,
 			button_.getX() + geo.x,
 			button_.getY() + button_.getHeight() + geo.y + 5
		);
		
		//lower mid
		addInOutEventsTest(
			button_.getX() + button_.getWidth() / 2 + geo.x,
			button_.getY() + button_.getHeight() + geo.y - 1,
			button_.getX() + button_.getWidth() / 2 + geo.x,
			button_.getY() + button_.getHeight() + geo.y + 5			
		);
		
		//lower right
		addInOutEventsTest(
			button_.getX() + button_.getWidth() + geo.x - 1,
			button_.getY() + button_.getHeight() + geo.y - 1,
 			button_.getX() + button_.getWidth() + geo.x,
 			button_.getY() + button_.getHeight() + geo.y + 5				   
		);
	}
	
	void addInOutEventsTest(int xInside, int yInside, int xOutside, int yOutside){
		//move into button
		//verify callback
		//move out of button
		//verify callback
		
		//Place pointer inside widget
		queueStep(
			boost::bind(
				&ElmTestHarness::setGlobalPointerPosition, boost::ref(*this),
				xInside,
				yInside
			)
		);
		queueStep(
			boost::bind(
				&ElmTestHarness::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&UserButtonEventTest::isMouseInButton_, boost::ref(*this))
			),
			"checking EVAS_CALLBACK_MOUSE_IN event"
		);
		
		//Place pointer outside widget
		queueStep(
			boost::bind(
				&ElmTestHarness::setGlobalPointerPosition, boost::ref(*this),
				xOutside,
				yOutside
			)
		);
		queueStep(
			boost::bind(
				&ElmTestHarness::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&UserButtonEventTest::isMouseOutButton_, boost::ref(*this))
			),
			"checking EVAS_CALLBACK_MOUSE_OUT event"
		);
	}
	
	static void onMouseInButton(void *data, Evas*, Evas_Object*, void*)
	{
		UserButtonEventTest *test = static_cast<UserButtonEventTest*>(data);

		test->isMouseInButton_ = true;
		test->isMouseOutButton_ = false;
	}

	static void onMouseOutButton(void *data, Evas*, Evas_Object*, void*)
	{
		UserButtonEventTest *test = static_cast<UserButtonEventTest*>(data);
		test->isMouseInButton_ = false;
		test->isMouseOutButton_ = true;
	}
	
	
private:
	bool		isMouseInButton_;
	bool		isMouseOutButton_;
	Window		window_;
	EvasObject 	button_;
};

typedef ResizeObjectTest<Button> ButtonResizeTest;
typedef PositionObjectTest<Button> ButtonPositionTest;
typedef VisibleObjectTest<Button> ButtonVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(UserButtonInteractionTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(UserButtonEventTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonResizeTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonPositionTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonVisibilityTest, "Button")

