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

#ifndef __WFITS_EFL_TEMPLATES_H__
#define __WFITS_EFL_TEMPLATES_H__

#include <boost/bind.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <vector>

#include "elmtestharness.h"
#include "evasobject.h"
#include "window.h"

namespace wfits {
namespace test {
namespace efl {

template <typename T>
class ResizeObjectTest : public ElmTestHarness
{
	BOOST_STATIC_ASSERT( (boost::is_base_of<EvasObject, T>::value) );

public:
	ResizeObjectTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ResizeObjectTest", "Resize Test")
		, object_(window_)
		, resized_(false)
	{
		widths_.push_back(300);
		widths_.push_back(200);
		widths_.push_back(101);

		heights_.push_back(200);
		heights_.push_back(100);
		heights_.push_back(51);
	}

	void setup()
	{
		evas_object_event_callback_add(object_, EVAS_CALLBACK_RESIZE, onResize, this);

		object_.show();
		window_.show();
	}

	void test()
	{
		foreach (const int w, widths_) {
			foreach (const int h, heights_) {
				resized_ = false;
				TEST_LOG("resizing object to " << w << "x" << h);
				object_.setSize(w, h);
				TEST_LOG("checking for resize event");
				YIELD_UNTIL(resized_);
				TEST_LOG("checking object size prop");
				object_.checkSize(w, h);
			}
		}
	}

	static void onResize(void *data, Evas *, Evas_Object *, void *)
	{
		ResizeObjectTest<T> *o = static_cast<ResizeObjectTest<T>*>(data);
		o->resized_ = true;
		TEST_LOG("got resize event");
	}

private:
	Window			window_;
	T			object_;
	bool			resized_;
	std::vector<int>	widths_;
	std::vector<int>	heights_;
};

template <typename T>
class PositionObjectTest : public ElmTestHarness
{
	BOOST_STATIC_ASSERT( (boost::is_base_of<EvasObject, T>::value) );

public:
	PositionObjectTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("PositionObjectTest", "Position Test")
		, object_(window_)
		, moved_(false)
	{
		xs_.push_back(50);
		xs_.push_back(100);
		xs_.push_back(151);

		ys_.push_back(10);
		ys_.push_back(50);
		ys_.push_back(101);
	}

	void setup()
	{
		object_.setSize(50, 50);

		evas_object_event_callback_add(object_, EVAS_CALLBACK_MOVE, onMove, this);

		object_.show();
		window_.show();
	}

	void test()
	{
		foreach (const int x, xs_) {
			foreach (const int y, ys_) {
				moved_ = false;
				TEST_LOG("moving object to " << x << "," << y);
				object_.setPosition(x, y);
				TEST_LOG("checking for move event");
				YIELD_UNTIL(moved_);
				TEST_LOG("checking object position prop");
				object_.checkPosition(x, y);
			}
		}
	}

	static void onMove(void *data, Evas *, Evas_Object *, void *)
	{
		PositionObjectTest<T> *o = static_cast<PositionObjectTest<T>*>(data);
		o->moved_ = true;
		TEST_LOG("got move event");
	}

private:
	Window			window_;
	T			object_;
	bool			moved_;
	std::vector<int>	xs_;
	std::vector<int>	ys_;
};


template <typename T>
class VisibleObjectTest : public ElmTestHarness
{
	BOOST_STATIC_ASSERT( (boost::is_base_of<EvasObject, T>::value) );

public:
	VisibleObjectTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("VisibleObjectTest", "Visibility Test")
		, object_(window_)
		, visible_(false)
	{
		return;
	}

	void setup()
	{
		object_.setSize(50, 50);
		object_.setPosition(100, 100);

		evas_object_event_callback_add(object_, EVAS_CALLBACK_SHOW, onShow, this);
		evas_object_event_callback_add(object_, EVAS_CALLBACK_HIDE, onHide, this);

		window_.show();
	}

	void test()
	{
		ASSERT(object_.isVisible() == EINA_FALSE);

		visible_ = false;
		TEST_LOG("showing object");
		object_.show();
		TEST_LOG("checking for show event");
		YIELD_UNTIL(visible_);
		TEST_LOG("checking object visibility prop");
		object_.checkVisible(EINA_TRUE);

		ASSERT(visible_);
		TEST_LOG("hiding object");
		object_.hide();
		TEST_LOG("checking for hide event");
		YIELD_UNTIL(not visible_);
		TEST_LOG("checking object visibility prop");
		object_.checkVisible(EINA_FALSE);

		ASSERT(not visible_);
		TEST_LOG("showing object");
		object_.show();
		TEST_LOG("checking for show event");
		YIELD_UNTIL(visible_);
		TEST_LOG("checking object visibility prop");
		object_.checkVisible(EINA_TRUE);
	}

	static void onShow(void *data, Evas *, Evas_Object *, void *)
	{
		VisibleObjectTest<T> *o = static_cast<VisibleObjectTest<T>*>(data);
		o->visible_ = true;
		TEST_LOG("got show event");
	}

	static void onHide(void *data, Evas *, Evas_Object *, void *)
	{
		VisibleObjectTest<T> *o = static_cast<VisibleObjectTest<T>*>(data);
		o->visible_ = false;
		TEST_LOG("got hide event");
	}

private:
	Window	window_;
	T	object_;
	bool	visible_;
};

/* TODO: evasobject methods for color set/get/check
template <typename T, int Tr1, int Tg1, int Tb1, int Ta1, int Tr2, int Tg2, int Tb2, int Ta2>
class ColorObjectTest : public ElmTestHarness
{
	BOOST_STATIC_ASSERT( (boost::is_base_of<EvasObject, T>::value) );

public:
	ColorObjectTest()
			: ElmTestHarness::ElmTestHarness()
			, window_("ColorObjectTest", "Color Test")
			, object_(window_)
	{
		return;
	}

	void setup()
	{
		queueCallback(
			ModifyCheckCallback(
				boost::bind(&T::setColor, boost::ref(object_), Tr1, Tg1, Tb1, Ta1),
				boost::bind(&T::checkColor, boost::ref(object_), Tr1, Tg1, Tb1, Ta1)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&T::setColor, boost::ref(object_), Tr2, Tg2, Tb2, Ta2),
				boost::bind(&T::checkColor, boost::ref(object_), Tr2, Tg2, Tb2, Ta2)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&T::setColor, boost::ref(object_), Tr1, Tg1, Tb1, Ta1),
				boost::bind(&T::checkColor, boost::ref(object_), Tr1, Tg1, Tb1, Ta1)
			)
		);
	}

private:
	Window	window_;
	T	object_;
};
*/

/* TODO: evasobject methods for raise/lower set/get/check
template <typename T>
class RaiseLowerObjectTest : public ElmTestHarness
{
	BOOST_STATIC_ASSERT( (boost::is_base_of<EvasObject, T>::value) );

public:
	RaiseLowerObjectTest()
			: ElmTestHarness::ElmTestHarness()
			, window_("RaiseLowerObjectTest", "RaiseLower Test")
			, object_(window_)
	{
		return;
	}

	void setup()
	{
		queueCallback(
			ModifyCheckCallback(
				boost::bind(&T::raise, boost::ref(object_)),
				boost::bind(&T::checkRisen, boost::ref(object_), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&T::lower, boost::ref(object_)),
				boost::bind(&T::checkLowered, boost::ref(object_), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&T::raise, boost::ref(object_)),
				boost::bind(&T::checkRisen, boost::ref(object_), EINA_TRUE)
			)
		);
	}

private:
	Window	window_;
	T	object_;
};
*/

} // namespace efl
} // namespace test
} // namespace wfits

#endif

