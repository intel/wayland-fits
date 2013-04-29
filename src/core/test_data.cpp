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

#include <wayland-client-protocol.h>
#include "harness.h"

template <typename O>
class DataTest : public CoreTestHarness
{
public:
	virtual void setup()
	{
		queueStep(
			boost::bind(&DataTest<O>::test, boost::ref(*this))
		);
	}
	
	void test()
	{
		FAIL_IF_EQUAL(obj_, NULL);

		int a = random();
		int *r;
		for (unsigned i(0); i < 10000; ++i)
		{
			setter(&a);
			r = static_cast<int*>(getter());
			FAIL_UNLESS_EQUAL(r, &a);
			FAIL_UNLESS_EQUAL(*r, a);
			a = random();
		}

		std::string *d, p("This is my data");
		setter(&p);
		d = static_cast<std::string*>(getter());
		FAIL_UNLESS_EQUAL(d, &p);
		FAIL_UNLESS_EQUAL(*d, p);
	}
	
	virtual void* getter() = 0;
	virtual void setter(void*) = 0;
	
protected:
	O* obj_;
};

#define DATA_SETTER_GETTER(name) \
	void* getter() { return name ## _get_user_data(obj_); } \
	void setter(void* d) { name ## _set_user_data(obj_, d); }

#define DATA_TEST(name) \
	class data_ ## name : public DataTest<name> \
	{ \
	public: \
		data_ ## name() \
			: DataTest<name>::DataTest() \
		{ \
			obj_ = display().bind<name>(#name, & name ## _interface); \
		} \
		DATA_SETTER_GETTER(name) \
	}; \
	WFITS_CORE_HARNESS_TEST_CASE(data_ ## name, "DataInterface")
	
DATA_TEST(wl_compositor)
DATA_TEST(wl_display)
DATA_TEST(wl_shm)
DATA_TEST(wl_output)
DATA_TEST(wl_seat)
DATA_TEST(wl_shell)
DATA_TEST(wl_data_device_manager)
