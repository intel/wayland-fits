#include "window.h"
#include "elmtestharness.h"

class WindowMaximizeTest : public ElmTestHarness
{
public:
	WindowMaximizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowMaximizeTest", "Window Maximize Test")
		, geometry_()
		, isMaximized_(false)
		, configureDone_(false)
		, resizeDone_(false)
	{
		return;
	}

	void setup()
	{
		window_.show();
		Application::yield(0.01*1e6);
		geometry_ = getSurfaceGeometry(window_.get_wl_surface());
		std::cout << "...initial server geometry is: "
			<< geometry_.x << " "
			<< geometry_.y << " "
			<< geometry_.width << " "
			<< geometry_.height << std::endl;

		evas_object_smart_callback_add(window_, "maximized", onMaximize, this);
		evas_object_smart_callback_add(window_, "unmaximized", onUnMaximize, this);
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);
		ecore_event_handler_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, onConfigure, this);

		for(unsigned i(0); i < 5; ++i)
			addMaximizeTest();
	}

	void addMaximizeTest()
	{
		// Maximize
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::isMaximized_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::configureDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::resizeDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::bind(
				&Window::maximize, boost::ref(window_),
				EINA_TRUE
			),
			"maximizing window"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::isMaximized_, boost::ref(*this))
			),
			"checking maximized event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::configureDone_, boost::ref(*this))
			),
			"checking configure event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::resizeDone_, boost::ref(*this))
			),
			"checking resize event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&Window::getWidth, boost::ref(window_)) > geometry_.width
				and boost::lambda::bind(&Window::getHeight, boost::ref(window_)) > geometry_.height
			),
			"checking client size > initial server size"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::serverSizeIsEqual, boost::ref(*this))
			),
			"checking client size == server size"
		);

		// UnMaximize
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::configureDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::lambda::bind(
				&WindowMaximizeTest::resizeDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::bind(
				&Window::maximize, boost::ref(window_),
				EINA_FALSE
			),
			"unmaximizing window"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::configureDone_, boost::ref(*this))
			),
			"checking configure event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				not boost::lambda::bind(&WindowMaximizeTest::isMaximized_, boost::ref(*this))
			),
			"checking unmaximized event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::resizeDone_, boost::ref(*this))
			),
			"checking resize event"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&Window::getWidth, boost::ref(window_)) == geometry_.width
				and boost::lambda::bind(&Window::getHeight, boost::ref(window_)) == geometry_.height
			),
			"checking client size == initial server size"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowMaximizeTest::serverSizeIsEqual, boost::ref(*this))
			),
			"checking client size == server size"
		);
		queueStep(
			boost::bind(
				&WindowMaximizeTest::serverGeometryIsInitial,
				boost::ref(*this)
			),
			"checking server geometry == initial server geometry"
		);
	}

	bool serverSizeIsEqual()
	{
		Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return window_.getWidth() == g.width
			and window_.getHeight() == g.height;
	}

	bool serverGeometryIsInitial()
	{
		Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return geometry_.x == g.x
			and geometry_.y == g.y
			and geometry_.width == g.width
			and geometry_.height == g.height;
	}

	static Eina_Bool onConfigure(void *data, int type, void *event)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->configureDone_ = true;
		Ecore_Wl_Event_Window_Configure *ev = static_cast<Ecore_Wl_Event_Window_Configure *>(event);

		std::cout << "...got configure event: "
			<< ev->x << " "
			<< ev->y << " "
			<< ev->w << " "
			<< ev->h << std::endl;
		return ECORE_CALLBACK_PASS_ON;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->resizeDone_ = true;
	}

	static void onMaximize(void* data, Evas_Object *obj, void* event_info)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = EINA_TRUE;
	}

	static void onUnMaximize(void *data, Evas_Object*, void*)
	{
		WindowMaximizeTest *test = static_cast<WindowMaximizeTest*>(data);
		test->isMaximized_ = EINA_FALSE;
	}

private:
	Window		window_;
	Geometry	geometry_;
	bool		isMaximized_;
	bool		configureDone_;
	bool		resizeDone_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(WindowMaximizeTest, "Window")
