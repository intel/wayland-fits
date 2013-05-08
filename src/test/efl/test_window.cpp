/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <vector>

#include "window.h"
#include "elmtestharness.h"

namespace wfits {
namespace test {
namespace efl {

using std::vector;

class WindowIconifyTest : public ElmTestHarness
{
public:
	WindowIconifyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIconifyTest", "Window Iconify/Minimize Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::iconify, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkIconified, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::iconify, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkIconified, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
};

class WindowStickyTest : public ElmTestHarness
{
public:
	WindowStickyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowStickyTest", "Window Sticky Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::sticky, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkSticky, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::sticky, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkSticky, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
};

class WindowWithdrawnTest : public ElmTestHarness
{
public:
	WindowWithdrawnTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowWithdrawnTest", "Window Withdrawn Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&Window::withdrawn, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::checkWithdrawn, boost::ref(window_), EINA_TRUE));
		queueStep(boost::bind(&Window::withdrawn, boost::ref(window_), EINA_FALSE));
		queueStep(boost::bind(&Window::checkWithdrawn, boost::ref(window_), EINA_FALSE));
	}

private:
	Window	window_;
};

class WindowRotationTest : public ElmTestHarness
{
public:
	WindowRotationTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowRotationTest", "Window Rotation Test")
	{
		degrees_.push_back(90);
		degrees_.push_back(180);
		degrees_.push_back(270);
		degrees_.push_back(360);
		degrees_.push_back(0);
		degrees_.push_back(90);
		degrees_.push_back(0);
		degrees_.push_back(180);
		degrees_.push_back(0);
		degrees_.push_back(270);
		degrees_.push_back(0);
		degrees_.push_back(360);
	}

	void setup()
	{
		window_.show();

		foreach (const int degree, degrees_) {
			queueStep(boost::bind(&Window::rotate, boost::ref(window_), degree));
			queueStep(boost::bind(&Window::checkRotation, boost::ref(window_), degree));
		}
	}

private:
	Window		window_;
	vector<int>	degrees_;
};

class WindowAlphaTest : public ElmTestHarness
{
public:
	WindowAlphaTest()
		: ElmTestHarness::ElmTestHarness()
		, window_(elm_win_add(NULL, elm_config_engine_get(), ELM_WIN_BASIC))
		, rendered_(false)
	{
		return;
	}

	void setup()
	{
		elm_win_title_set(window_, "Alpha Test");
		window_.setSize(800, 600);
		window_.show();

		queueStep(boost::bind(&WindowAlphaTest::test, boost::ref(*this)));
	}

	void test()
	{
		#define YIELD_UNTIL_RENDERED \
		do { \
			rendered_ = false; \
			evas_event_callback_add( \
				evas_object_evas_get(window_), \
				EVAS_CALLBACK_RENDER_POST, \
				onPostRender, this \
			); \
			std::cout << "...checking for post render" << std::endl; \
			YIELD_UNTIL(rendered_); \
		} while (0)

		YIELD_UNTIL_RENDERED;

		for (unsigned i(0); i < 5; ++i) {
			std::cout << "...setting alpha true" << std::endl;
			elm_win_alpha_set(window_, EINA_TRUE);
			YIELD_UNTIL_RENDERED;
			std::cout << "...checking alpha prop" << std::endl;
			YIELD_UNTIL(elm_win_alpha_get(window_) == EINA_TRUE);
			std::cout << "...setting alpha false" << std::endl;
			elm_win_alpha_set(window_, EINA_FALSE);
			YIELD_UNTIL_RENDERED;
			std::cout << "...checking alpha prop" << std::endl;
			YIELD_UNTIL(elm_win_alpha_get(window_) == EINA_FALSE);
		}
		#undef YIELD_UNTIL_RENDERED
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		WindowAlphaTest *test = static_cast<WindowAlphaTest*>(data);
		test->rendered_ = true;
	}
private:
	EvasObject window_;
	bool rendered_;
};

class WindowIndicatorTest : public ElmTestHarness
{
public:
	WindowIndicatorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIndicatorTest", "Window Indicator Test")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(elm_win_indicator_mode_set, boost::ref(window_), ELM_WIN_INDICATOR_SHOW));
		queueStep(boost::bind(&WindowIndicatorTest::checkIndicator, boost::ref(*this), ELM_WIN_INDICATOR_SHOW));
		queueStep(boost::bind(elm_win_indicator_mode_set, boost::ref(window_), ELM_WIN_INDICATOR_HIDE));
		queueStep(boost::bind(&WindowIndicatorTest::checkIndicator, boost::ref(*this), ELM_WIN_INDICATOR_HIDE));
		queueStep(boost::bind(elm_win_indicator_mode_set, boost::ref(window_), ELM_WIN_INDICATOR_SHOW));
		queueStep(boost::bind(&WindowIndicatorTest::checkIndicator, boost::ref(*this), ELM_WIN_INDICATOR_SHOW));
	}

	void checkIndicator(const Elm_Win_Indicator_Mode expected)
	{
		FAIL_UNLESS_EQUAL(elm_win_indicator_mode_get(window_), expected);
	}

private:
	Window	window_;
};

class WindowIndicatorOpacityTest : public ElmTestHarness
{
public:
	WindowIndicatorOpacityTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIndicatorOpacityTest", "Window Indicator Opacity Test")
	{
		modes_.push_back(ELM_WIN_INDICATOR_OPAQUE);
		modes_.push_back(ELM_WIN_INDICATOR_TRANSLUCENT);
		modes_.push_back(ELM_WIN_INDICATOR_TRANSPARENT);
	}

	void setup()
	{
		window_.show();

		typedef vector<Elm_Win_Indicator_Opacity_Mode>::const_iterator CIterator;
		const CIterator endIt(modes_.end());
		for (CIterator it(modes_.begin()); it != endIt; ++it) {
			queueStep(boost::bind(elm_win_indicator_opacity_set, boost::ref(window_), *it));
			queueStep(boost::bind(&WindowIndicatorOpacityTest::checkOpacity, boost::ref(*this), *it));
			queueStep(boost::bind(elm_win_indicator_opacity_set, boost::ref(window_), ELM_WIN_INDICATOR_OPAQUE));
			queueStep(boost::bind(&WindowIndicatorOpacityTest::checkOpacity, boost::ref(*this), ELM_WIN_INDICATOR_OPAQUE));
		}
	}

	void checkOpacity(const Elm_Win_Indicator_Opacity_Mode expected)
	{
		FAIL_UNLESS_EQUAL(elm_win_indicator_opacity_get(window_), expected);
	}

private:
	Window					window_;
	vector<Elm_Win_Indicator_Opacity_Mode>	modes_;
};

class EcoreWlWindowTest : public ElmTestHarness
{
public:
	EcoreWlWindowTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EcoreWlWindowTest", "EcoreWlWindowTest")
	{
		return;
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&EcoreWlWindowTest::check, boost::ref(*this)));
	}

	void check()
	{
		ASSERT(elm_win_wl_window_get(window_) != NULL);
	}

private:
	Window	window_;
};

class WindowOutputTest : public ElmTestHarness
{
public:
	void setup()
	{
		Evas *evas;

		rendered_ = false;
		window_ = elm_win_util_standard_add("WindowOutputTest", "Window Output Test");
		evas = evas_object_evas_get(window_);

		evas_event_callback_add(evas, EVAS_CALLBACK_RENDER_POST, onPostRender, this);

		evas_object_resize(window_, 67, 39);
		evas_object_show(window_);

		queueStep(boost::bind(&WindowOutputTest::test, boost::ref(*this)));
	}

	void test()
	{
		YIELD_UNTIL(rendered_);

		Evas *evas;
		Ecore_Wl_Window *wlwin;
		Geometry server, window, viewport, framespace;
		int width, height;
		const int cwidth = 67;	// control width
		const int cheight = 39;	// control height

		evas = evas_object_evas_get(window_);
		wlwin = elm_win_wl_window_get(window_);
		server = getSurfaceGeometry(wlwin->surface);
		evas_output_size_get(evas, &width, &height);
		evas_output_viewport_get(evas, &viewport.x, &viewport.y, &viewport.width, &viewport.height);
		evas_output_framespace_get(evas, &framespace.x, &framespace.y, &framespace.width, &framespace.height);
		evas_object_geometry_get(window_, &window.x, &window.y, &window.width, &window.height);

		FAIL_UNLESS_EQUAL(width, cwidth + framespace.width);
		FAIL_UNLESS_EQUAL(height, cheight + framespace.height);
		FAIL_UNLESS_EQUAL(width, server.width);
		FAIL_UNLESS_EQUAL(height, server.height);
		FAIL_UNLESS_EQUAL(viewport.width, server.width);
		FAIL_UNLESS_EQUAL(viewport.height, server.height);
		FAIL_UNLESS_EQUAL(window.width, cwidth);
		FAIL_UNLESS_EQUAL(window.height, cheight);
		FAIL_UNLESS_EQUAL(window.width + framespace.width, server.width);
		FAIL_UNLESS_EQUAL(window.height + framespace.height, server.height);
	}

	void teardown()
	{
		evas_object_del(window_);
	}

	static void onPostRender(void *data, Evas *e, void *info)
	{
		evas_event_callback_del(e, EVAS_CALLBACK_RENDER_POST, onPostRender);

		WindowOutputTest *test = static_cast<WindowOutputTest*>(data);
		test->rendered_ = true;
		std::cout << "...got post render event" << std::endl;
	}

private:
	Evas_Object	*window_;
	bool		rendered_;
};

WFITS_EFL_HARNESS_TEST_CASE(WindowIconifyTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowStickyTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowWithdrawnTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowRotationTest)
WFITS_EFL_HARNESS_TEST_CASE(EcoreWlWindowTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowOutputTest)

//TODO: These three below need work still
WFITS_EFL_HARNESS_TEST_CASE(WindowAlphaTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowIndicatorTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowIndicatorOpacityTest)

} // namespace efl
} // namespace test
} // namespace wfitsv
