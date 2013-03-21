#include "window.h"
#include "elmtestharness.h"

class WindowResizeTest : public ElmTestHarness
{
public:
	WindowResizeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowResizeTest", "Window Resize Test")
		, resizeDone_(false)
	{
		return;
	}

	void setup()
	{
		window_.show();

		// This will flush out the window titlebar animation.
		// However, this is highly dependent on the default animation
		// and if that changes, then this may not work.  But currently
		// 20 yields seems to do the trick.
		for (unsigned i(0); i < 20; ++i)
			queueStep(boost::bind(&Application::yield, 0.001*1e6));

		addResizeTest(-10, -10);
		addResizeTest(-1, 10);
		addResizeTest(10, -1);
		addResizeTest(0, 0);

		for (int w(1); w <= 400; w += 89)
			for (int h(3); h <= 400; h += 91)
				addResizeTest(w, h);

		addResizeTest(3000, 3000);

		evas_object_event_callback_add(window_, EVAS_CALLBACK_RESIZE, &onResize, this);
	}

	void addResizeTest(int width, int height)
	{
		queueStep(
			boost::lambda::bind(
				&WindowResizeTest::resizeDone_,
				boost::ref(*this)
			) = false
		);
		queueStep(
			boost::bind(
				&Window::setSize,
				boost::ref(window_),
				width,
				height
			),
			boost::str(
				boost::format("resizing to %1% x %2%") % width % height
			)
		);
		queueStep(
			boost::bind(
				&WindowResizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowResizeTest::resizeDone_, boost::ref(*this))
			),
			"checking resize event"
		);
		queueStep(
			boost::bind(
				&WindowResizeTest::assertCondition,
				boost::ref(*this),
				boost::lambda::bind(&Window::getWidth, boost::ref(window_)) == std::max(1, width)
				and boost::lambda::bind(&Window::getHeight, boost::ref(window_)) == std::max(1, height)
			),
			"checking client size"
		);
		queueStep(
			boost::bind(
				&WindowResizeTest::stepUntilCondition,
				boost::ref(*this),
				boost::lambda::bind(&WindowResizeTest::serverSizeIsEqual, boost::ref(*this))
			),
			"checking server size"
		);
	}

	static void onResize(void *data, Evas*, Evas_Object*, void*)
	{
		WindowResizeTest *test = static_cast<WindowResizeTest*>(data);
		test->resizeDone_ = true;
	}

	bool serverSizeIsEqual()
	{
		Geometry geometry(getSurfaceGeometry(window_.get_wl_surface()));

		return window_.getWidth() == geometry.width
			and window_.getHeight() == geometry.height;
	}

private:
	Window	window_;
	bool	resizeDone_;
};

WAYLAND_ELM_HARNESS_TEST_CASE(WindowResizeTest, "Window")
