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
		Geometry geoFS = window_.getFramespaceGeometry();
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
				(geo.y + button_.getY() + button_.getHeight() / 2 + geoFS.y)
				)
		);
		
		//below are all callbacks this widget makes that the user can invoke
		addEventsTest();
		
	}
	
	void addEventsTest()
	{
		queueStep(
			boost::bind(
				&ElmTestHarness::inputKeySend,
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
				&ElmTestHarness::inputKeySend,
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
		button_.setPosition(0, 0);
		button_.show();
		window_.show();
		
		Application::yield(0.01*1e6);

		Geometry geo = getSurfaceGeometry(elm_win_wl_window_get(window_)->surface);
		Geometry geoFS = window_.getFramespaceGeometry();
		
		geo.y += geoFS.y;
		geo.x += geoFS.x;
		
		geo.y += button_.getY();
		geo.x += button_.getX();
		
		int bw(button_.getWidth());
		int bh(button_.getHeight());

		//NOTE: a button at 0,0 and 10x10 would fill pixels 0-9x0-9

		//top left (vertical)
		addInOutEventsTest(
			geo.x    , geo.y,
			geo.x    , geo.y - 1		//1px outside top
		);
		
		//top left (horizontal)
		addInOutEventsTest(
			geo.x    , geo.y,
			geo.x - 1, geo.y		//1px outside left
		);
		
		//top mid
		addInOutEventsTest(
			bw / 2 + geo.x, geo.y,
			bw / 2 + geo.x, geo.y - 1	//1px outside top
		);
		
		//top right (vertical)
		addInOutEventsTest(
			bw + geo.x - 1, geo.y,
			bw + geo.x - 1, geo.y - 1	//1px outside top
		);
		
		//top right (horizontal)
		addInOutEventsTest( 
			bw + geo.x - 1, geo.y,
 			bw + geo.x    , geo.y		//1px outside right
		);
		
		//mid left
		addInOutEventsTest(
			geo.x    , bh / 2 + geo.y,
			geo.x - 1, bh / 2 + geo.y	//1px outside left
		);
		
		//mid right
		addInOutEventsTest(
			bw + geo.x - 1, bh / 2 + geo.y,
			bw + geo.x    , bh / 2 + geo.y	//1px outside right
		);
		
		//bottom left (vertical)
		addInOutEventsTest(
			geo.x    , bh + geo.y - 1,
			geo.x    , bh + geo.y		//1px outside bottom
		);
		
		//bottom left (horizontal)
		addInOutEventsTest(
			geo.x    , bh + geo.y - 1,
			geo.x - 1, bh + geo.y - 1	//1px outside left
		);
		
		//bottom mid
		addInOutEventsTest(
			bw / 2 + geo.x, bh + geo.y - 1,
			bw / 2 + geo.x, bh + geo.y	//1px outside bottom
		);
		
		//bottom right (vertical)
		addInOutEventsTest(
			bw + geo.x - 1, bh + geo.y - 1,
			bw + geo.x - 1, bh + geo.y	//1px outside bottom
		);
		
		//bottom right (horizontal)
		addInOutEventsTest(
			bw + geo.x - 1, bh + geo.y - 1,
			bw + geo.x    , bh + geo.y - 1	//1px outside right
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

