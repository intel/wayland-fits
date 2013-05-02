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

#ifndef __WFITS_EFL_ELMTESTHARNESS_H__
#define __WFITS_EFL_ELMTESTHARNESS_H__

#include <Ecore.h>
#include <Ecore_Wayland.h>
#include <Elementary.h>
#include "test/harness.h"
#include "application.h"

class ElmTestHarness : public TestHarness
{
public:
	typedef boost::function<bool (void)> Condition;

	/**
	 * Construct the test harness.
	 **/
	ElmTestHarness();

	void run();

	using TestHarness::queueStep;

	void queueStep(TestHarness::TestStep, const std::string&);

	Geometry getSurfaceGeometry(wl_surface*);

	void setGlobalPointerPosition(int32_t, int32_t) const;
	void setGlobalPointerPosition(const Position&) const;
	Position getGlobalPointerPosition() const;
	void expectGlobalPointerPosition(int32_t, int32_t) const;
	void expectGlobalPointerPosition(const Position&) const;

	void inputKeySend(int32_t, int32_t) const;

	void stepUntilCondition(Condition condition);
	void assertCondition(Condition condition);
	void assertCondition(Condition condition, const std::string&);

private:
	static Eina_Bool idleSetup(void*);
	static Eina_Bool doSetup(void*, int, void*);

	static Eina_Bool idleStep(void*);
	static Eina_Bool doStep(void*, int, void*);

	static Eina_Bool idleTeardown(void*);
	static Eina_Bool doTeardown(void*, int, void*);

	int			eventType_; /// custom event type
	Ecore_Event_Handler*	handler_;
};

#define WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite) \
TEST(Egl##Harness, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_EGL); \
	Harness().run(); \
}

#define WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
TEST(Shm##Harness, "EFL/" suite) \
{ \
	Application app; \
	app.setEngine(Application::ENGINE_SHM); \
	Harness().run(); \
}

#define WAYLAND_ELM_HARNESS_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite)


#endif

