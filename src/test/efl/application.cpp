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

#include <Elementary.h>

#include "application.h"

namespace wfits {
namespace test {
namespace efl {

Application::Application()
{
	elm_init(
		TheGlobalSuite::instance().argc,
		TheGlobalSuite::instance().argv
	);

	// Eina's default setting doesn't abort on ERR messages so this
	// forces tests to abort if they emit any ERR or CRITICAL messages.
	eina_log_abort_on_critical_set(EINA_TRUE);
	eina_log_abort_on_critical_level_set(EINA_LOG_LEVEL_ERR);

	setEngine(ENGINE_SHM);
}

/* virtual */
Application::~Application()
{
	elm_shutdown();
}

/* static */
void Application::setEngine(const Engine& engine)
{
	std::string strEngine;
	switch (engine) {
		case ENGINE_EGL:
			strEngine = "wayland_egl";
			break;
		case ENGINE_SHM:
		default:
			strEngine = "wayland_shm";
	}

	elm_config_preferred_engine_set(strEngine.c_str()); // override's ELM_ENGINE user environment setting
	ASSERT(strEngine == std::string(elm_config_preferred_engine_get()));

	elm_config_engine_set(strEngine.c_str());
	ASSERT(strEngine == std::string(elm_config_engine_get()));
}

/*static*/
void Application::yield(const unsigned time)
{
	ecore_main_loop_iterate();
	usleep(time);
}

} // namespace efl
} // namespace test
} // namespace wfits
