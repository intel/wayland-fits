#include "harness.h"

TestHarness::TestHarness()
	: steps_()
{
	return;
}

TestHarness::~TestHarness()
{
	return;
}

void TestHarness::queueStep(TestStep step)
{
	steps_.push_back(step);
}

void TestHarness::run()
{
	setup();

	while (haveStep())
		runNextStep();

	teardown();
}

bool TestHarness::haveStep() const
{
	return not steps_.empty();
}

void TestHarness::runNextStep()
{
	TestStep step(steps_.front());
	steps_.pop_front();
	step();
}
