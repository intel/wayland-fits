#include <boost/bind.hpp>
#include "harness.h"

CoreTestHarness::CoreTestHarness()
	: Display::Display()
	, listener_(0)
{
	listener_ = wl_display_add_global_listener(
		*this, &CoreTestHarness::globalListener, this);
	FAIL_IF_EQUAL(listener_, NULL);
}

CoreTestHarness::~CoreTestHarness()
{
	wl_display_remove_global_listener(*this, listener_);
}

void CoreTestHarness::queueTest(Test test)
{
	tests_.push_back(test);
}

void CoreTestHarness::run()
{
	// trigger client global announcements
	wl_display_iterate(*this, WL_DISPLAY_READABLE);

	setup();

	while (not tests_.empty())
	{
		tests_.front()(); // call test
		tests_.pop_front(); // remove test
	}

	teardown();
}

/*static*/
void CoreTestHarness::globalListener(wl_display* display, uint32_t id, const char* iface, uint32_t version, void* data)
{
	CoreTestHarness* harness = static_cast<CoreTestHarness*>(data);
	FAIL_UNLESS_EQUAL(display, *harness);
	harness->handleGlobal(id, std::string(iface), version);
}

class SimpleTest : public CoreTestHarness
{
public:
	SimpleTest() :
		CoreTestHarness::CoreTestHarness()
		, tested(0)
	{
		return;
	}

	void handleGlobal(uint32_t, const std::string&, uint32_t)
	{
		return;
	}
	
	void setup()
	{
		queueTest(boost::bind(&SimpleTest::test, boost::ref(*this)));
	}
	
	void test()
	{
		if (++tested < 10)
		{
			queueTest(boost::bind(&SimpleTest::test, boost::ref(*this)));
		}
	}
	
	void teardown()
	{
		FAIL_UNLESS_EQUAL(tested, 10);
	}
	
	unsigned tested;
};

WFITS_CORE_HARNESS_TEST_CASE(SimpleTest, "Harness")
