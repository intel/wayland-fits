#include "harness.h"

class ShmDataTest : public CoreTestHarness
{
public:
	void handleGlobal(uint32_t id, const std::string& iface, uint32_t version)
	{
		if (iface == "wl_shm")
		{
			shm_ = static_cast<wl_shm*>(wl_display_bind(*this, id, &wl_shm_interface));
			FAIL_IF_EQUAL(shm_, NULL);
		}
	}

	void setup()
	{
		queueTest(
			boost::bind(&ShmDataTest::test, boost::ref(*this))
		);
	}
	
	void test()
	{
		int a = random();
		int *r;
		for (unsigned i(0); i < 1000; ++i)
		{
			wl_shm_set_user_data(shm_, &a);
			r = static_cast<int*>(wl_shm_get_user_data(shm_));
			FAIL_UNLESS_EQUAL(r, &a);
			FAIL_UNLESS_EQUAL(*r, a);
			a = random();
		}

		std::string *d, p("This is my data");
		wl_shm_set_user_data(shm_, &p);
		d = static_cast<std::string*>(wl_shm_get_user_data(shm_));
		FAIL_UNLESS_EQUAL(d, &p);
		FAIL_UNLESS_EQUAL(*d, p);
	}
private:
	wl_shm* shm_;
};

WFITS_CORE_HARNESS_TEST_CASE(ShmDataTest, "Shm");