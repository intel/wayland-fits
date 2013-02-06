#ifndef __WFITS_COMMON_HARNESS_H__
#define __WFITS_COMMON_HARNESS_H__

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <deque>

class TestHarness
{
public:
	typedef boost::function<void()>	TestStep;
	typedef std::deque<TestStep>	TestSteps;

	TestHarness();

	virtual ~TestHarness();

	virtual void queueStep(TestStep);

	virtual void run();
	virtual void setup() { };
	virtual void teardown() { };

	void runNextStep();
	bool haveStep() const;

protected:
	TestSteps	steps_;
};

#define HARNESS_TEST(HarnessClass, suite) \
\
TEST(HarnessClass, suite) \
{ \
	HarnessClass().run(); \
}

#endif
