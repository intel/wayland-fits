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

#include "window.h"
#include "elmtestharness.h"

namespace wfits {
namespace test {
namespace efl {

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
	}

	void test()
	{
		TEST_LOG("iconifying window");
		window_.iconify(EINA_TRUE);

		TEST_LOG("checking window iconify attributes");
		window_.checkIconified(EINA_TRUE);

		TEST_LOG("un-iconifying window");
		window_.iconify(EINA_FALSE);

		TEST_LOG("checking window un-iconify attributes");
		window_.checkIconified(EINA_FALSE);
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
	}

	void test()
	{
		TEST_LOG("setting window sticky");
		window_.sticky(EINA_TRUE);

		TEST_LOG("checking window sticky attributes");
		window_.checkSticky(EINA_TRUE);

		TEST_LOG("setting window un-sticky");
		window_.sticky(EINA_FALSE);

		TEST_LOG("checking window un-sticky attributes");
		window_.checkSticky(EINA_FALSE);
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
	}

	void test()
	{
		//FIXME: doing this too quickly can crash
		// we need to ensure we've rendered once
		TEST_LOG("setting window withdrawn");
		window_.withdrawn(EINA_TRUE);

		TEST_LOG("checking window withdrawn attributes");
		window_.checkWithdrawn(EINA_TRUE);

		TEST_LOG("setting window un-withdrawn");
		window_.withdrawn(EINA_FALSE);

		TEST_LOG("checking window un-withdrawn attributes");
		window_.checkWithdrawn(EINA_FALSE);
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
		return;
	}

	void setup()
	{
		window_.setSize(450, 150);
		window_.show();
	}

	void test()
	{
		testRotate(90);
		testRotate(180);
		testRotate(270);
		testRotate(360);
		testRotate(0);
		testRotate(90);
		testRotate(0);
		testRotate(180);
		testRotate(0);
		testRotate(270);
		testRotate(0);
		testRotate(360);
	}

	void testRotate(int degrees)
	{
		TEST_LOG("rotating window to " << degrees << " degrees");
		window_.rotate(degrees);

		TEST_LOG("checking window rotation attributes == " << degrees << " degrees");
		window_.checkRotation(degrees);

		//TODO: validate window orientation with the server side
	}

private:
	Window		window_;
};

class WindowAlphaTest : public ElmTestHarness
{
public:
	WindowAlphaTest()
		: ElmTestHarness::ElmTestHarness()
		, window_(elm_win_add(NULL, elm_config_engine_get(), ELM_WIN_BASIC))
	{
		return;
	}

	void setup()
	{
		elm_win_title_set(window_, "Alpha Test");
		window_.setSize(400, 300);
		window_.show();
	}

	void test()
	{
		for (unsigned i(0); i < 5; ++i)
		{
			TEST_LOG("setting alpha true");
			synchronized(
				[this]() {
					elm_win_alpha_set(window_, EINA_TRUE);
				}
			);

			TEST_LOG("checking alpha attribute == true");
			YIELD_UNTIL(
				Application::synchronizedResult(
					[this]()->Eina_Bool {
						return elm_win_alpha_get(window_);
					}
				)  == EINA_TRUE
			);

			TEST_LOG("setting alpha false");
			synchronized(
				[this]() {
					elm_win_alpha_set(window_, EINA_FALSE);
				}
			);

			TEST_LOG("checking alpha attribute == false");
			YIELD_UNTIL(
				Application::synchronizedResult(
					[this]()->Eina_Bool {
						return elm_win_alpha_get(window_);
					}
				)  == EINA_FALSE
			);
		}
	}

private:
	EvasObject window_;
};

static std::string toString(Elm_Win_Indicator_Mode mode)
{
	switch(mode)
	{
		case ELM_WIN_INDICATOR_SHOW:
			return "SHOW";
		case ELM_WIN_INDICATOR_HIDE:
			return "HIDE";
		default:
			return "UNKNOWN";
	}
}

class WindowIndicatorModeTest : public ElmTestHarness
{
public:
	WindowIndicatorModeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIndicatorModeTest", "Window Indicator Mode Test")
	{
		return;
	}

	void setup()
	{
		window_.show();
	}

	void test()
	{
		testIndicatorMode(ELM_WIN_INDICATOR_SHOW);
		testIndicatorMode(ELM_WIN_INDICATOR_HIDE);
		testIndicatorMode(ELM_WIN_INDICATOR_SHOW);
	}

	void testIndicatorMode(Elm_Win_Indicator_Mode mode)
	{
		TEST_LOG("setting window indicator mode = " << toString(mode));
		synchronized(
			[this, &mode]() {
				elm_win_indicator_mode_set(window_, mode);
			}
		);

		TEST_LOG("checking window indicator mode attribute == " << toString(mode));
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Elm_Win_Indicator_Mode {
					return elm_win_indicator_mode_get(window_);
				}
			), mode
		);
	}

private:
	Window window_;
};

static std::string toString(Elm_Win_Indicator_Opacity_Mode opacity)
{
	switch(opacity)
	{
		case ELM_WIN_INDICATOR_OPAQUE:
			return "OPAQUE";
		case ELM_WIN_INDICATOR_TRANSLUCENT:
			return "TRANSLUCENT";
		default:
			return "UNKNOWN";
	}
}

class WindowIndicatorOpacityTest : public ElmTestHarness
{
public:
	WindowIndicatorOpacityTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("WindowIndicatorOpacityTest", "Window Indicator Opacity Test")
	{
		return;
	}

	void setup()
	{
		window_.show();
	}

	void test()
	{
		testIndicatorOpacity(ELM_WIN_INDICATOR_OPAQUE);
		testIndicatorOpacity(ELM_WIN_INDICATOR_TRANSLUCENT);
		testIndicatorOpacity(ELM_WIN_INDICATOR_OPAQUE);
	}

	void testIndicatorOpacity(Elm_Win_Indicator_Opacity_Mode opacity)
	{
		TEST_LOG("setting window indicator opacity = " << toString(opacity));
		synchronized(
			[this, &opacity]() {
				elm_win_indicator_opacity_set(window_, opacity);
			}
		);

		TEST_LOG("checking window indicator opacity attribute == " << toString(opacity));
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this]()->Elm_Win_Indicator_Opacity_Mode {
					return elm_win_indicator_opacity_get(window_);
				}
			), opacity
		);
	}

private:
	Window window_;
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
	}

	void test()
	{
		TEST_LOG("checking wl_window != NULL")
		ASSERT(
			Application::synchronizedResult(
				[this]()->Ecore_Wl_Window* {
					return elm_win_wl_window_get(window_);
				}
			) != NULL
		);
	}

private:
	Window	window_;
};

class WindowOutputTest : public ElmTestHarness
{
public:
	WindowOutputTest()
		: ElmTestHarness::ElmTestHarness()
		, window_(NULL)
	{
		return;
	}

	virtual ~WindowOutputTest()
	{
		if (window_ != NULL)
		{
			evas_object_del(window_);
		}
	}

	void setup()
	{
		window_ = elm_win_util_standard_add("WindowOutputTest", "Window Output Test");

		evas_object_resize(window_, 67, 39);
		evas_object_show(window_);
	}

	void test()
	{
		Evas		*evas;
		Ecore_Wl_Window	*wlwin;
		wl_surface	*surface;
		Geometry	server, window, viewport, framespace;
		int		width, height;
		const int	cwidth = 67;	// control width
		const int	cheight = 39;	// control height

		synchronized(
			[this, &evas, &wlwin, &surface]() {
				evas = evas_object_evas_get(window_);
				wlwin = elm_win_wl_window_get(window_);
				#if ECORE_VERSION_MAJOR == 1 && ECORE_VERSION_MINOR == 7
				surface = wlwin->surface;
				#else
				surface = ecore_wl_window_surface_get(wlwin);
				#endif
			}
		);

		server = getSurfaceGeometry(surface);

		synchronized(
			[this, &evas, &width, &height, &window, &viewport, &framespace]() {
				evas_output_size_get(evas, &width, &height);
				evas_output_viewport_get(
					evas, &viewport.x, &viewport.y,
					&viewport.width, &viewport.height
				);
				evas_output_framespace_get(
					evas, &framespace.x, &framespace.y,
					&framespace.width, &framespace.height
				);
				evas_object_geometry_get(
					window_, &window.x, &window.y,
					&window.width, &window.height
				);
			}
		);

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

private:
	Evas_Object	*window_;
};

// NOTE: These tests are DISABLED for now since the wayland protocol
// does not currently support iconify and sticky windows
// WFITS_EFL_HARNESS_TEST_CASE(WindowIconifyTest)
// WFITS_EFL_HARNESS_TEST_CASE(WindowStickyTest)


WFITS_EFL_HARNESS_TEST_CASE(WindowWithdrawnTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowRotationTest)
WFITS_EFL_HARNESS_TEST_CASE(EcoreWlWindowTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowOutputTest)

//TODO: These three below need work still
WFITS_EFL_HARNESS_TEST_CASE(WindowAlphaTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowIndicatorModeTest)
WFITS_EFL_HARNESS_TEST_CASE(WindowIndicatorOpacityTest)

} // namespace efl
} // namespace test
} // namespace wfitsv
