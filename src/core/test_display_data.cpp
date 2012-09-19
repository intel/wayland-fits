#include "harness.h"

class SetDataTest : public CoreTestHarness
{
public:
	void setup()
	{
		queueTest(
			boost::bind(&SetDataTest::test, boost::ref(*this))
		);
	}
	
	void test()
	{
		int a = random();
		int *r;
		for (unsigned i(0); i < 1000; ++i)
		{
			wl_display_set_user_data(*this, &a);
			r = static_cast<int*>(wl_display_get_user_data(*this));
			FAIL_UNLESS_EQUAL(r, &a);
			FAIL_UNLESS_EQUAL(*r, a);
			a = random();
		}

		std::string *d, p("This is my data");
		wl_display_set_user_data(*this, &p);
		d = static_cast<std::string*>(wl_display_get_user_data(*this));
		FAIL_UNLESS_EQUAL(d, &p);
		FAIL_UNLESS_EQUAL(*d, p);
		std::cout << "SANITY" << std::endl;
	}
};

WFITS_CORE_HARNESS_TEST_CASE(SetDataTest, "Display");