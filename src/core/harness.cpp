
#include "harness.h"

CoreTestHarness::CoreTestHarness()
	: TestHarness::TestHarness()
	, display_()
{
	return;
}

CoreTestHarness::~CoreTestHarness()
{
	return;
}

void CoreTestHarness::runStep(CoreTestHarness::TestStep step) const
{
	step();
	display().yield();
}

void CoreTestHarness::queueStep(TestStep step)
{
	TestHarness::queueStep(
		boost::bind(&CoreTestHarness::runStep, boost::ref(*this), step));
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
		queueStep(boost::bind(&SimpleTest::test, boost::ref(*this)));
	}

	void test()
	{
		if (++tested < 10)
		{
			queueStep(boost::bind(&SimpleTest::test, boost::ref(*this)));
		}
	}

	void teardown()
	{
		FAIL_UNLESS_EQUAL(tested, 10);
	}

	unsigned tested;
};

WFITS_CORE_HARNESS_TEST_CASE(SimpleTest, "Harness")
