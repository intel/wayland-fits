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
		object_.show();
		window_.show();
		queueStep(boost::bind(&ResizeObjectTest<T>::test, boost::ref(*this)));
	}

	void test()
	{
		evas_object_event_callback_add(object_, EVAS_CALLBACK_RESIZE, onResize, this);
		foreach (const int w, widths_) {
			foreach (const int h, heights_) {
				resized_ = false;
				std::cout << "...resizing object to " << w << "x" << h << std::endl;
				object_.setSize(w, h);
				std::cout << "...checking for resize event" << std::endl;
				YIELD_UNTIL(resized_);
				std::cout << "...checking object size prop" << std::endl;
				object_.checkSize(w, h);
			}
		}
	}

	static void onResize(void *data, Evas *, Evas_Object *, void *)
	{
		ResizeObjectTest<T> *o = static_cast<ResizeObjectTest<T>*>(data);
		o->resized_ = true;
		std::cout << "...got resize event" << std::endl;
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

		object_.setSize(50, 50);
	}

	void setup()
	{
		object_.show();
		window_.show();
		queueStep(boost::bind(&PositionObjectTest<T>::test, boost::ref(*this)));
	}

	void test()
	{
		evas_object_event_callback_add(object_, EVAS_CALLBACK_MOVE, onMove, this);
		foreach (const int x, xs_) {
			foreach (const int y, ys_) {
				moved_ = false;
				std::cout << "...moving object to " << x << "," << y << std::endl;
				object_.setPosition(x, y);
				std::cout << "...checking for move event" << std::endl;
				YIELD_UNTIL(moved_);
				std::cout << "...checking object position prop" << std::endl;
				object_.checkPosition(x, y);
			}
		}
	}

	static void onMove(void *data, Evas *, Evas_Object *, void *)
	{
		PositionObjectTest<T> *o = static_cast<PositionObjectTest<T>*>(data);
		o->moved_ = true;
		std::cout << "...got move event" << std::endl;
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
		object_.setSize(50, 50);
		object_.setPosition(100, 100);
	}

	void setup()
	{
		window_.show();

		queueStep(boost::bind(&VisibleObjectTest<T>::test, boost::ref(*this)));
	}

	void test()
	{
		evas_object_event_callback_add(object_, EVAS_CALLBACK_SHOW, onShow, this);
		evas_object_event_callback_add(object_, EVAS_CALLBACK_HIDE, onHide, this);

		ASSERT(object_.isVisible() == EINA_FALSE);

		visible_ = false;
		std::cout << "...showing object" << std::endl;
		object_.show();
		std::cout << "...checking for show event" << std::endl;
		YIELD_UNTIL(visible_);
		std::cout << "...checking object visibility prop" << std::endl;
		object_.checkVisible(EINA_TRUE);

		ASSERT(visible_);
		std::cout << "...hiding object" << std::endl;
		object_.hide();
		std::cout << "...checking for hide event" << std::endl;
		YIELD_UNTIL(not visible_);
		std::cout << "...checking object visibility prop" << std::endl;
		object_.checkVisible(EINA_FALSE);

		ASSERT(not visible_);
		std::cout << "...showing object" << std::endl;
		object_.show();
		std::cout << "...checking for show event" << std::endl;
		YIELD_UNTIL(visible_);
		std::cout << "...checking object visibility prop" << std::endl;
		object_.checkVisible(EINA_TRUE);
	}

	static void onShow(void *data, Evas *, Evas_Object *, void *)
	{
		VisibleObjectTest<T> *o = static_cast<VisibleObjectTest<T>*>(data);
		o->visible_ = true;
		std::cout << "...got show event" << std::endl;
	}

	static void onHide(void *data, Evas *, Evas_Object *, void *)
	{
		VisibleObjectTest<T> *o = static_cast<VisibleObjectTest<T>*>(data);
		o->visible_ = false;
		std::cout << "...got hide event" << std::endl;
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

