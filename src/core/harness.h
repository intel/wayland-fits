#ifndef __WFITS_CORE_HARNESS_H__
#define __WFITS_CORE_HARNESS_H__

#include "common/harness.h"
#include "display.h"

class CoreTestHarness : public TestHarness
{
public:
	CoreTestHarness();

	virtual ~CoreTestHarness();
	
	void queueStep(TestStep);

	const Display& display() const { return display_; }

private:
	void runStep(TestStep step) const;

	Display	display_;
};

#define WFITS_CORE_HARNESS_TEST_CASE(HarnessClass, suite) \
	HARNESS_TEST(HarnessClass, "Core/" suite)

#endif
