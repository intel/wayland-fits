#include "window.h"
#include "elmtestharness.h"

class WindowFullscreenTest : public ElmTestHarness
{
public:
	WindowFullscreenTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowFullscreenTest", "Window Fullscreen Test")
		, initialGeometry_()
		, fullscreenDone_(EINA_FALSE)
		, unfullscreenDone_(EINA_FALSE)
		, configureDone_(EINA_FALSE)
		, resizeDone_(EINA_FALSE)
	{
		return;
	}

	void setup()
	{
		window_.show();

		Application::yield(0.01*1e6);
		initialGeometry_ = getSurfaceGeometry(window_.get_wl_surface());
		std::cout << "...initial server geometry is: "
			<< initialGeometry_.x << ","
			<< initialGeometry_.y << " "
			<< initialGeometry_.width << "x"
			<< initialGeometry_.height << std::endl;

		evas_object_smart_callback_add(window_, "fullscreen", onFullscreen, this);
		evas_object_smart_callback_add(window_, "unfullscreen", onUnFullscreen, this);
		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);
		ecore_event_handler_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, onConfigure, this);

		for(unsigned i(0); i < 5; ++i)
			addFullscreenTest();
	}

	void addFullscreenTest()
	{
		// Set Window Fullscreen
		queueStep(
			boost::lambda::bind(
				&WindowFullscreenTest::fullscreenDone_,
				boost::ref(*this)
			) = EINA_FALSE
		);
		queueStep(
			boost::lambda::bind(
				&WindowFullscreenTest::configureDone_,
				boost::ref(*this)
			) = EINA_FALSE
		);
		queueStep(
			boost::lambda::bind(
				&WindowFullscreenTest::resizeDone_,
				boost::ref(*this)
			) = EINA_FALSE
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::isInitialGeometry, boost::ref(*this))
			),
			"checking client geometry == initial server geometry"
		);
		queueStep(
			boost::bind(
				&Window::fullscreen, boost::ref(window_),
				EINA_TRUE
			),
			"set fullscreen"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::fullscreenDone_, boost::ref(*this)) == EINA_TRUE
			),
			"checking fullscreen event"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::configureDone_, boost::ref(*this)) == EINA_TRUE
			),
			"checking configure event"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::resizeDone_, boost::ref(*this)) == EINA_TRUE
			),
			"checking resize event"
		);
		queueStep(
			boost::bind(
				&Window::checkFullscreen, boost::ref(window_),
				EINA_TRUE
			),
			"checking client is fullscreen"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::isScreenGeometry, boost::ref(*this))
			),
			"checking client geometry == server geometry == screen geometry"
		);

		// Unfullscreen/Restore Window
		queueStep(
			boost::lambda::bind(
				&WindowFullscreenTest::unfullscreenDone_,
				boost::ref(*this)
			) = EINA_FALSE
		);
		queueStep(
			boost::lambda::bind(
				&WindowFullscreenTest::configureDone_,
				boost::ref(*this)
			) = EINA_FALSE
		);
		queueStep(
			boost::lambda::bind(
				&WindowFullscreenTest::resizeDone_,
				boost::ref(*this)
			) = EINA_FALSE
		);
		queueStep(
			boost::bind(
				&Window::fullscreen, boost::ref(window_),
				EINA_FALSE
			),
			"set unfullscreen"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::configureDone_, boost::ref(*this)) == EINA_TRUE
			),
			"checking configure event"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::unfullscreenDone_, boost::ref(*this)) == EINA_TRUE
			),
			"checking unfullscreen event"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::resizeDone_, boost::ref(*this)) == EINA_TRUE
			),
			"checking resize event"
		);
			queueStep(
			boost::bind(
				&Window::checkFullscreen, boost::ref(window_),
				EINA_FALSE
			),
			"checking client is not fullscreen"
		);
		queueStep(
			boost::bind(
				&WindowFullscreenTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowFullscreenTest::isInitialGeometry, boost::ref(*this))
			),
			"checking client geometry == server geometry == initial server geometry"
		);
	}

	bool isInitialGeometry()
	{
		const Geometry sg(getSurfaceGeometry(window_.get_wl_surface()));
		const Geometry fg(window_.getFramespaceGeometry());
	
		return sg.x == initialGeometry_.x
			and sg.y == initialGeometry_.y
			and sg.width == initialGeometry_.width
			and sg.height == initialGeometry_.height
			and window_.getWidth() + fg.width == initialGeometry_.width
			and window_.getHeight() + fg.height == initialGeometry_.height
		;
		// NOTE: server does not support client side positioning
	}

	bool isScreenGeometry()
	{
		int sx, sy, sw, sh;
		elm_win_screen_size_get(window_, &sx, &sy, &sw, &sh);
		const Geometry g(getSurfaceGeometry(window_.get_wl_surface()));

		return g.x == sx
			and g.y == sy
			and g.width == sw
			and g.height == sh
			and window_.getX() == sx
			and window_.getY() == sy
			and window_.getWidth() == sw
			and window_.getHeight() == sh
		;
	}

	static Eina_Bool onConfigure(void *data, int type, void *event)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		test->configureDone_ = true;
		Ecore_Wl_Event_Window_Configure *ev = static_cast<Ecore_Wl_Event_Window_Configure *>(event);

		std::cout << "...got configure event: "
			<< ev->x << ","
			<< ev->y << " "
			<< ev->w << "x"
			<< ev->h << std::endl;
		return ECORE_CALLBACK_PASS_ON;
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		std::cout << "...got resize event" << std::endl;
		test->resizeDone_ = true;
	}

	static void onFullscreen(void* data, Evas_Object *obj, void* event_info)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		std::cout << "...got fullscreen event" << std::endl;
		test->fullscreenDone_ = EINA_TRUE;
	}

	static void onUnFullscreen(void *data, Evas_Object*, void*)
	{
		WindowFullscreenTest *test = static_cast<WindowFullscreenTest*>(data);
		std::cout << "...got unfullscreen event" << std::endl;
		test->unfullscreenDone_ = EINA_TRUE;
	}
	
private:
	Window		window_;
	Geometry	initialGeometry_;
	Eina_Bool	fullscreenDone_;
	Eina_Bool	unfullscreenDone_;
	Eina_Bool	configureDone_;
	Eina_Bool	resizeDone_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(WindowFullscreenTest, "Window")
