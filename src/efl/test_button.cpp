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

class ButtonUserMouseClickTest : public ElmTestHarness
{
public:
	ButtonUserMouseClickTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ButtonUserMouseClickTest", "Button User Mouse Click Test")
		, button_(elm_button_add(window_))
		, clicked_(false)
		, repeated_(false)
		, pressed_(false)
		, unpressed_(false)
		, rendered_(false)
	{
		return;
	}

	void setup()
	{
		evas_object_resize(button_, 200, 100);
		elm_object_text_set(button_, "Test Button!");

		//place button in the center of window
		button_.setPosition(
			(window_.getWidth() / 2 - button_.getWidth() / 2),
			(window_.getHeight() / 2 - button_.getHeight() / 2)
		);

		elm_button_autorepeat_set(button_, EINA_TRUE);
		elm_button_autorepeat_initial_timeout_set(button_, 1.0);
		elm_button_autorepeat_gap_timeout_set(button_, 0.5);

		evas_event_callback_add(
			evas_object_evas_get(window_),
			EVAS_CALLBACK_RENDER_POST,
			onPostRender, this
		);

		evas_object_smart_callback_add(button_, "clicked", onClick, this);
		evas_object_smart_callback_add(button_, "repeated", onRepeated, this);
		evas_object_smart_callback_add(button_, "pressed", onPressed, this);
		evas_object_smart_callback_add(button_, "unpressed", onUnpressed, this);

		button_.show();
		window_.show();

		queueStep(boost::bind(&ButtonUserMouseClickTest::test, boost::ref(*this)));
	}

	void test()
	{
		YIELD_UNTIL(rendered_);

		Geometry framespace(window_.getFramespaceGeometry());
		Geometry geometry(getSurfaceGeometry(window_.get_wl_surface()));

		//Place pointer in center of button
		setGlobalPointerPosition(
			geometry.x + button_.getX() + button_.getWidth() / 2,
			geometry.y + button_.getY() + button_.getHeight() / 2 + framespace.y
		);

		ASSERT(not pressed_);
		ASSERT(not repeated_);

		inputKeySend(BTN_LEFT, 1);

		YIELD_UNTIL(pressed_ and repeated_)

		ASSERT(not unpressed_);
		ASSERT(not clicked_);

		inputKeySend(BTN_LEFT, 0);

		YIELD_UNTIL(unpressed_ and clicked_)
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		ButtonUserMouseClickTest *test = static_cast<ButtonUserMouseClickTest*>(data);
		test->rendered_ = true;
		std::cout << "...received post render event" << std::endl;
	}

	static void onClick(void* data, Evas_Object*, void*)
	{
		ButtonUserMouseClickTest *test = static_cast<ButtonUserMouseClickTest*>(data);
		test->clicked_ = true;
		std::cout << "...received click event" << std::endl;
	}
	
	static void onRepeated(void* data, Evas_Object*, void*)
	{
		ButtonUserMouseClickTest *test = static_cast<ButtonUserMouseClickTest*>(data);
		test->repeated_ = true;
		std::cout << "...received repeated event" << std::endl;
	}
	
	static void onPressed(void* data, Evas_Object*, void*)
	{
		ButtonUserMouseClickTest *test = static_cast<ButtonUserMouseClickTest*>(data);
		test->pressed_ = true;
		std::cout << "...received pressed event" << std::endl;
	}
	
	static void onUnpressed(void* data, Evas_Object*, void*)
	{
		ButtonUserMouseClickTest *test = static_cast<ButtonUserMouseClickTest*>(data);
		test->unpressed_ = true;
		std::cout << "...received unpressed event" << std::endl;
	}
	
private:
	Window		window_;
	EvasObject	button_;
	bool		clicked_;
	bool		repeated_;
	bool		pressed_;
	bool		unpressed_;
	bool		rendered_;
};


class ButtonUserMouseInOutTest : public ElmTestHarness
{
public:
	
	ButtonUserMouseInOutTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ButtonUserMouseInOutTest", "Button Mouse In/Out Test")
		, button_(elm_button_add(window_))
		, mouseIn_(false)
		, rendered_(false)
	{
		return;
	}

	void setup()
	{
		evas_object_resize(button_, 200, 100);
		elm_object_text_set(button_, "Test Button!");
		button_.setPosition(0, 0);

		evas_event_callback_add(
			evas_object_evas_get(window_),
			EVAS_CALLBACK_RENDER_POST,
			onPostRender, this
		);

		evas_object_event_callback_add(button_, EVAS_CALLBACK_MOUSE_IN, onMouseIn, this);
		evas_object_event_callback_add(button_, EVAS_CALLBACK_MOUSE_OUT, onMouseOut, this);

		button_.show();
		window_.show();

		queueStep(boost::bind(&ButtonUserMouseInOutTest::test, boost::ref(*this)));
	}

	void test()
	{
		YIELD_UNTIL(rendered_);

		int bw(button_.getWidth());
		int bh(button_.getHeight());
		Geometry geo(getSurfaceGeometry(elm_win_wl_window_get(window_)->surface));
		Geometry fg(window_.getFramespaceGeometry());

		geo.y += fg.y + button_.getY();
		geo.x += fg.x + button_.getX();

		//NOTE: a button at 0,0 and 10x10 would fill pixels 0-9x0-9

		//top left (vertical)
		testMouseInOut(
			geo.x    , geo.y,
			geo.x    , geo.y - 1		//1px outside top
		);

		//top left (horizontal)
		testMouseInOut(
			geo.x    , geo.y,
			geo.x - 1, geo.y		//1px outside left
		);

		//top mid
		testMouseInOut(
			bw / 2 + geo.x, geo.y,
			bw / 2 + geo.x, geo.y - 1	//1px outside top
		);

		//top right (vertical)
		testMouseInOut(
			bw + geo.x - 1, geo.y,
			bw + geo.x - 1, geo.y - 1	//1px outside top
		);

		//top right (horizontal)
		testMouseInOut(
			bw + geo.x - 1, geo.y,
			bw + geo.x    , geo.y		//1px outside right
		);

		//mid left
		testMouseInOut(
			geo.x    , bh / 2 + geo.y,
			geo.x - 1, bh / 2 + geo.y	//1px outside left
		);

		//mid right
		testMouseInOut(
			bw + geo.x - 1, bh / 2 + geo.y,
			bw + geo.x    , bh / 2 + geo.y	//1px outside right
		);

		//bottom left (vertical)
		testMouseInOut(
			geo.x    , bh + geo.y - 1,
			geo.x    , bh + geo.y		//1px outside bottom
		);

		//bottom left (horizontal)
		testMouseInOut(
			geo.x    , bh + geo.y - 1,
			geo.x - 1, bh + geo.y - 1	//1px outside left
		);

		//bottom mid
		testMouseInOut(
			bw / 2 + geo.x, bh + geo.y - 1,
			bw / 2 + geo.x, bh + geo.y	//1px outside bottom
		);

		//bottom right (vertical)
		testMouseInOut(
			bw + geo.x - 1, bh + geo.y - 1,
			bw + geo.x - 1, bh + geo.y	//1px outside bottom
		);

		//bottom right (horizontal)
		testMouseInOut(
			bw + geo.x - 1, bh + geo.y - 1,
			bw + geo.x    , bh + geo.y - 1	//1px outside right
		);
	}

	void testMouseInOut(int xInside, int yInside, int xOutside, int yOutside)
	{
		ASSERT(not mouseIn_);

		//Place pointer inside widget
		setGlobalPointerPosition(xInside, yInside);

		YIELD_UNTIL(mouseIn_);

		setGlobalPointerPosition(xOutside, yOutside);

		YIELD_UNTIL(not mouseIn_);
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		ButtonUserMouseInOutTest *test = static_cast<ButtonUserMouseInOutTest*>(data);
		test->rendered_ = true;
		std::cout << "...received post render event" << std::endl;
	}

	static void onMouseIn(void *data, Evas*, Evas_Object*, void*)
	{
		ButtonUserMouseInOutTest *test = static_cast<ButtonUserMouseInOutTest*>(data);
		test->mouseIn_ = true;
		std::cout << "...received mouse in event" << std::endl;
	}

	static void onMouseOut(void *data, Evas*, Evas_Object*, void*)
	{
		ButtonUserMouseInOutTest *test = static_cast<ButtonUserMouseInOutTest*>(data);
		test->mouseIn_ = false;
		std::cout << "...received mouse out event" << std::endl;
	}

private:
	Window		window_;
	EvasObject 	button_;
	bool		mouseIn_;
	bool		rendered_;
};

typedef ResizeObjectTest<Button> ButtonResizeTest;
typedef PositionObjectTest<Button> ButtonPositionTest;
typedef VisibleObjectTest<Button> ButtonVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(ButtonUserMouseClickTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonUserMouseInOutTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonResizeTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonPositionTest, "Button")
WAYLAND_ELM_HARNESS_TEST_CASE(ButtonVisibilityTest, "Button")

