/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "harness.h"
#include "compositor.h"
#include "pointer.h"
#include "seat.h"
#include "surface.h"
#include "shell.h"
#include "shell_surface.h"
#include "shm.h"
#include "data_device_manager.h"
#include "data_offer.h"
#include "data_target.h"
#include "data_device.h"
#include "data_source.h"

namespace wfits {
namespace test {
namespace core {
namespace input {

class ShmSurface : public Surface
{
public:
	ShmSurface(const Compositor& compositor, const Shell& shell, const SharedMemory& shm)
		: Surface::Surface(compositor)
		, shellSurface_(shell, *this)
		, buffer_(shm, 50, 50)
	{
		wl_surface_attach(*this, buffer_, 0, 0);
		wl_surface_damage(*this, 0, 0, buffer_.width(), buffer_.height());
		commit();
	}

private:
	ShellSurface		shellSurface_;
	SharedMemoryBuffer	buffer_;
};

class DnDTarget : public ShmSurface, public DataTarget
{
public:
	DnDTarget(const Compositor& compositor, const Shell& shell,
		const SharedMemory& shm, const std::string& type
	)
		: ShmSurface::ShmSurface(compositor, shell, shm)
		, DataTarget::DataTarget(type)
	{
		// DataDevice relies on wl_surface_data_get(...) to get to the
		// underlying DataTarget interface (i.e. cast void*->DataTarget*).
		// To avoid data corruption and undesired behavior we must cast
		// 'this' down to the DataTarget base class first before storing
		// it as a void* in the wl_surface user data.  This is because
		// our DataTarget base may be stored at a different address.
		// http://stackoverflow.com/questions/5445105/conversion-from-void-to-the-pointer-of-the-base-class
		wl_surface_set_user_data(*this, static_cast<DataTarget*>(this));
	}
};

class DnDSource : public ShmSurface, public DataSource, public DataTarget
{
public:
	DnDSource(const Compositor& compositor, const Shell& shell,
		const SharedMemory& shm, const DataDeviceManager& ddm,
		const std::string& type, const std::string& message
	)
		: ShmSurface::ShmSurface(compositor, shell, shm)
		, DataSource::DataSource(ddm, type, message.c_str(), message.size())
		, DataTarget::DataTarget(type)
	{
		// DataDevice relies on wl_surface_data_get(...) to get to the
		// underlying DataTarget interface (i.e. cast void*->DataTarget*).
		// To avoid data corruption and undesired behavior we must cast
		// 'this' down to the DataTarget base class first before storing
		// it as a void* in the wl_surface user data.  This is because
		// our DataTarget base may be stored at a different address.
		// http://stackoverflow.com/questions/5445105/conversion-from-void-to-the-pointer-of-the-base-class
		wl_surface_set_user_data(*this, static_cast<DataTarget*>(this));
	}
};

class SimpleDragAndDropTest : public Harness
{
public:
	SimpleDragAndDropTest()
		: Harness::Harness()
		, compositor_(display())
		, shell_(display())
		, seat_(display())
		, ddm_(display())
		, dd_(ddm_, seat_)
		, pointer_(seat_)
		, shm_(display())
		, source_(compositor_, shell_, shm_, ddm_, "text/plain", "Test Drag-N-Drop!!")
		, target_(compositor_, shell_, shm_, "text/plain")
		, event_()
	{
		pointer_.bind(
			Pointer::OnButtonCallback(
				&SimpleDragAndDropTest::onButton, boost::ref(*this),
				_1, _2
			)
		);

		queueStep(boost::bind(&SimpleDragAndDropTest::test, boost::ref(*this)));
	}

	void test()
	{
		const Geometry gs(getSurfaceGeometry(source_));
		const Geometry gt(getSurfaceGeometry(target_));

		std::cout << "...source geometry: " << gs << std::endl;
		std::cout << "...target geometry: " << gt << std::endl;

		std::cout << "...moving pointer to source" << std::endl;
		setGlobalPointerPosition(gs.x + 2, gs.y + 3);

		std::cout << "...checking source has pointer focus" << std::endl;
		YIELD_UNTIL(pointer_.hasFocus(source_));

		std::cout << "...pressing mouse left button" << std::endl;
		inputKeySend(BTN_LEFT, 1);

		std::cout << "...checking for button press event" << std::endl;
		YIELD_UNTIL(event_.state == WL_POINTER_BUTTON_STATE_PRESSED);

		// A drag operation removes focus from the pointer surface
		// until the drop finishes.
		std::cout << "...checking NULL has pointer focus" << std::endl;
		YIELD_UNTIL(pointer_.hasFocus(NULL));

		// drag will be presented to source initially
		std::cout << "...checking data offered to source" << std::endl;
		YIELD_UNTIL(source_.offer() != NULL);

		std::cout << "...checking offer position" << std::endl;
		Position p(source_.offer()->position());
		FAIL_UNLESS_EQUAL(p.x, 2);
		FAIL_UNLESS_EQUAL(p.y, 3);

		std::cout << "...moving pointer to target" << std::endl;
		setGlobalPointerPosition(gt.x + 5, gt.y + 4);

		std::cout << "...checking offer leave source" << std::endl;
		YIELD_UNTIL(source_.offer() == NULL);

		std::cout << "...checking data offered to target" << std::endl;
		YIELD_UNTIL(target_.offer() != NULL);

		std::cout << "...checking offer position" << std::endl;
		p = target_.offer()->position();
		FAIL_UNLESS_EQUAL(p.x, 5);
		FAIL_UNLESS_EQUAL(p.y, 4);

		// trigger data device "motion"
		std::cout << "...moving pointer around target" << std::endl;
		setGlobalPointerPosition(gt.x + 6, gt.y + 5);

		std::cout << "...checking offer position" << std::endl;
		YIELD_UNTIL(
			target_.offer()->position().x == 6
			and target_.offer()->position().y == 5
		);

		std::cout << "...releasing mouse left button" << std::endl;
		inputKeySend(BTN_LEFT, 0);
		YIELD_UNTIL(pointer_.hasFocus(target_));

		std::cout << "...checking for drop sent" << std::endl;
		YIELD_UNTIL(source_.sent());

		std::cout << "...checking target received drop" << std::endl;
		std::string data(target_.readDropData());
		FAIL_UNLESS_EQUAL(data, "Test Drag-N-Drop!!");

		std::cout << "...checking offer leave target" << std::endl;
		YIELD_UNTIL(target_.offer() == NULL);
	}

private:
	void onButton(const Pointer& pointer, const ButtonEvent& event)
	{
		if (event.state == WL_POINTER_BUTTON_STATE_PRESSED) {
			ASSERT(pointer_.hasFocus(source_));
			source_.startDrag(event.serial, dd_, source_);
		}
		event_ = event;
	}

	Compositor		compositor_;
	Shell			shell_;
	Seat			seat_;
	DataDeviceManager	ddm_;
	DataDevice		dd_;
	Pointer			pointer_;
	SharedMemory		shm_;
	DnDSource		source_;
	DnDTarget		target_;
	ButtonEvent		event_;
};

WFITS_CORE_HARNESS_TEST_CASE(SimpleDragAndDropTest);

} // namespace input
} // namespace core
} // namespace test
} // namespace wfits
