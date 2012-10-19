
#include "harness.h"

CoreTestHarness::CoreTestHarness()
	: Display::Display()
	, registry_(0)
{
	registry_ = wl_display_get_registry(*this);

	static const struct wl_registry_listener listener = {
		&CoreTestHarness::globalListener
	};

	wl_registry_add_listener(registry_, &listener, this);
	FAIL_IF_EQUAL(registry_, NULL);
}

CoreTestHarness::~CoreTestHarness()
{
	wl_registry_destroy(*this);
}

void CoreTestHarness::queueTest(Test test)
{
	tests_.push_back(test);
}

void CoreTestHarness::run()
{
	// trigger client global announcements
	wl_display_dispatch(*this);

	setup();

	while (not tests_.empty())
	{
		tests_.front()(); // call test
		tests_.pop_front(); // remove test
	}

	teardown();
}

/*static*/
void CoreTestHarness::globalListener(
	void *data, struct wl_registry *registry, uint32_t id, const char* iface, uint32_t version)
{
	CoreTestHarness* harness = static_cast<CoreTestHarness*>(data);
	FAIL_UNLESS_EQUAL(registry, *harness);
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
