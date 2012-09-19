#include <wayland-client-protocol.h>
#include "harness.h"

template <typename O>
class DataTest : public CoreTestHarness
{
public:
	virtual void setup()
	{
		queueTest(
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
		DATA_SETTER_GETTER(name) \
		void handleGlobal(uint32_t id, const std::string& iface, uint32_t version) \
		{ \
			if (iface == #name) \
			{ \
				obj_ = static_cast<name*>(wl_display_bind(*this, id, & name ## _interface)); \
			} \
		} \
	}; \
	WFITS_CORE_HARNESS_TEST_CASE(data_ ## name, "DataInterface")
	
DATA_TEST(wl_compositor)
DATA_TEST(wl_display)
DATA_TEST(wl_shm)
DATA_TEST(wl_output)
DATA_TEST(wl_seat)
DATA_TEST(wl_shell)
DATA_TEST(wl_data_device_manager)
