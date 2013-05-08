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

namespace wfits {
namespace test {
namespace efl {

class ElmTestHarness : public test::Harness
{
public:
	typedef ::boost::function<bool (void)> Condition;

	using test::Harness::queueStep;

	/**
	 * Construct the test harness.
	 **/
	ElmTestHarness();

	void run();

	void queueStep(TestStep, const std::string&);

	/*virtual*/ void yield(const unsigned time = 0.01 * 1e6) const;
	/*virtual*/ const test::Client& client() const;

	void stepUntilCondition(Condition condition);
	void assertCondition(Condition condition);
	void assertCondition(Condition condition, const std::string&);

private:
	static Eina_Bool idleStep(void*);
	static Eina_Bool doStep(void*, int, void*);

	int			eventType_; /// custom event type
	Ecore_Event_Handler*	handler_;
};

#define WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite) \
namespace egl { \
	TEST(Harness, "EFL/Egl/" suite) \
	{ \
		Application app; \
		app.setEngine(Application::ENGINE_EGL); \
		Harness().run(); \
	} \
} // namespace egl

#define WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
namespace shm { \
	TEST(Harness, "EFL/Shm/" suite) \
	{ \
		Application app; \
		app.setEngine(Application::ENGINE_SHM); \
		Harness().run(); \
	} \
} // namespace shm

#define WAYLAND_ELM_HARNESS_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_SHM_TEST_CASE(Harness, suite) \
	WAYLAND_ELM_HARNESS_EGL_TEST_CASE(Harness, suite)

} // namespace efl
} // namespace test
} // namespace wfits

#endif

