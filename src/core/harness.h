#ifndef __WFITS_CORE_HARNESS_H__
#define __WFITS_CORE_HARNESS_H__

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <deque>
#include <string>

#include "display.h"

class CoreTestHarness
{
public:
	typedef boost::function<void()>	Test;
	typedef std::deque<Test>	Tests;

	CoreTestHarness();

	virtual ~CoreTestHarness();
	
	void queueTest(Test);
	void run();

	/**
	 * Optionally override this to do any special
	 * setup before processing Tests.
	 * i.e. you can queue all your tests from here.
	 **/
	virtual void setup() { };
	
	/**
	 * Optionally override teardown to do any special
	 * cleanup before the test exits.
	 **/
	virtual void teardown() { };

	const Display& display() const { return display_; }

private:
	Display	display_;
	Tests	tests_;
};

#define WFITS_CORE_HARNESS_TEST_CASE(HarnessClass, suite) \
\
TEST(HarnessClass, "Core/" suite) \
{ \
	HarnessClass().run(); \
}

#endif
