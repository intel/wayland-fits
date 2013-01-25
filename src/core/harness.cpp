
#include "harness.h"

CoreTestHarness::CoreTestHarness()
	: display_()
	, tests_()
{
	return;
}

CoreTestHarness::~CoreTestHarness()
{
	return;
}

void CoreTestHarness::queueTest(Test test)
{
	tests_.push_back(test);
}

void CoreTestHarness::run()
{
	setup();

	while (not tests_.empty())
	{
		tests_.front()(); // call test
		tests_.pop_front(); // remove test
		display().roundtrip();
	}

	teardown();
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
