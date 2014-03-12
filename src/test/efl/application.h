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

#ifndef __WFITS_EFL_APPLICATION_H__
#define __WFITS_EFL_APPLICATION_H__

#include <Ecore.h>
#include <Ecore_Wayland.h>
#include <Elementary.h>
#include "common/util.h"
#include "test/tools.h"
#include "test/client.h"

namespace wfits {
namespace test {
namespace efl {

class Client : public test::Client
{
public:
	Client(wl_display*);

	/*virtual*/ void synchronized(std::function<void()>) const;

protected:
	/*virtual*/ void doYield(const unsigned time) const;
};

class Application
{
public:
	enum Engine {
		engineBegin = 0,
		ENGINE_SHM = engineBegin,	/// wayland_shm
		ENGINE_EGL,			/// wayland_egl
		engineLast
	};

	/**
	 * Initialize's Elm and set's the default engine to ENGINE_SHM
	 **/
	Application();

	virtual ~Application();

	static const Client& client();

	static void run();
	static void exit();

	/**
	 * This will have an effect on objects created after
	 * calling this.
	 **/
	static void setEngine(const Engine&);

	/**
	 * sleep for specified microseconds.
	 **/
	static void yield(const unsigned time = 0.001 * 1e6, bool strict = false);

	template <typename F>
	static inline typename std::result_of<F()>::type
	synchronizedResult(F&& f)
	{
		if (boost::this_thread::get_id() != mainThreadId_) {
			yield();
			ecore_thread_main_loop_begin();
		}

		typename std::result_of<F()>::type result = f();

		if (boost::this_thread::get_id() != mainThreadId_) {
			ecore_thread_main_loop_end();
		}

		return result;
	}

	static inline void synchronized(std::function<void()> f)
	{
		if (boost::this_thread::get_id() != mainThreadId_)
		{
			yield();
			ecore_main_loop_thread_safe_call_sync(
				&synchronizedMain, &f
			);
		}
		else
		{
			f();
		}
	}

private:
	static inline void* synchronizedMain(void *data)
	{
		ASSERT(boost::this_thread::get_id() == mainThreadId_);

		typedef std::function<void()> Func;
		Func* f = static_cast<Func*>(data);
		(*f)();
		return NULL;
	}

	static boost::thread::id	mainThreadId_;
	static Client*			client_;
};

} // namespace efl
} // namespace test
} // namespace wfits

#endif
