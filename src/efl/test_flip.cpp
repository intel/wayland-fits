#include <boost/filesystem.hpp>

#include <vector>

#include "background.h"
#include "templates.h"

using boost::filesystem::path;
using std::vector;

// TODO: Once we instrument events, we can test the _interaction functions

class Flip : public EvasObject
{
public:
	Flip(EvasObject &parent)
		: EvasObject::EvasObject(elm_flip_add(parent))
	{
		return;
	}
};

class FlipGoTest : public ElmTestHarness
{
public:
	FlipGoTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FlipGoTest", "Flip Go Test")
		, control_(elm_flip_add(window_))
		, first_(window_)
		, second_(window_)
		, options_()
		, flipDone_(true)
		, nWaitForFlip_(0)
	{
		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		first_.setColor(255, 0, 0);
		second_.setColor(0, 255, 0);

		evas_object_size_hint_align_set(first_, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_size_hint_weight_set(first_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(second_, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_size_hint_weight_set(second_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		elm_object_part_content_set(control_, "front", first_);
		elm_object_part_content_set(control_, "back", second_);

		options_.push_back(ELM_FLIP_ROTATE_Y_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_Y_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_X_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_XZ_CENTER_AXIS);
		options_.push_back(ELM_FLIP_ROTATE_YZ_CENTER_AXIS);
		options_.push_back(ELM_FLIP_CUBE_LEFT);
		options_.push_back(ELM_FLIP_CUBE_RIGHT);
		options_.push_back(ELM_FLIP_CUBE_UP);
		options_.push_back(ELM_FLIP_CUBE_DOWN);
		options_.push_back(ELM_FLIP_PAGE_LEFT);
		options_.push_back(ELM_FLIP_PAGE_RIGHT);
		options_.push_back(ELM_FLIP_PAGE_UP);
		options_.push_back(ELM_FLIP_PAGE_DOWN);
		
		evas_object_smart_callback_add(control_, "animate,done", animateDone, this);
	}

	void setup()
	{
		control_.show();
		first_.show();
		second_.show();
		window_.show();

		// The front is initially visible... so kick off the flip sequence
		flipDone_ = true;
		checkFlip(EINA_TRUE);
	}

	static void animateDone(void* data, Evas_Object *obj, void* event_info)
	{
		FlipGoTest* fgt = static_cast<FlipGoTest*>(data);
		fgt->flipDone_ = true;
	}

	void checkFlip(const Eina_Bool visible)
	{
		// We would expect the signal to finish within a reasonable number of mainloop iterations.
		// But, just in case the signal/callback mechanism is broken we don't wait forever.
		FAIL_UNLESS(nWaitForFlip_ < 100);

		if (not flipDone_) {
			// we're still waiting for the "animate,done" signal
			++nWaitForFlip_;

			// just queue another callback
			queueStep(boost::bind(&FlipGoTest::checkFlip, boost::ref(*this), visible));

			return; // Don't check the flip state yet! Maybe next time!
		}

		// Reset state for next flip operation
		nWaitForFlip_ = 0;
		flipDone_ = false;
		
		// verify the flip state
		FAIL_UNLESS_EQUAL(elm_flip_front_visible_get(control_), visible);

		if (visible == EINA_FALSE) {
			// move on to the next flip option
			options_.pop_front();
		}

		if (options_.size() > 0) {
			// we still have flip options to process.
			queueStep(boost::bind(elm_flip_go, boost::ref(control_), options_.front()));
			queueStep(boost::bind(&FlipGoTest::checkFlip, boost::ref(*this), not visible));
		}
	}

private:
	Window				window_;
	EvasObject			control_;
	Background			first_, second_;
	std::deque<Elm_Flip_Mode>	options_;
	bool				flipDone_;
	unsigned			nWaitForFlip_;
};

typedef ResizeObjectTest<Flip> FlipResizeTest;
typedef PositionObjectTest<Flip> FlipPositionTest;
typedef VisibleObjectTest<Flip> FlipVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(FlipGoTest, "Flip")
WAYLAND_ELM_HARNESS_TEST_CASE(FlipResizeTest, "Flip")
WAYLAND_ELM_HARNESS_TEST_CASE(FlipPositionTest, "Flip")
WAYLAND_ELM_HARNESS_TEST_CASE(FlipVisibilityTest, "Flip")

