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

#include "seat.h"
#include "data_device_manager.h"
#include "data_device.h"

namespace wfits {
namespace test {
namespace core {

DataDevice::DataDevice(const DataDeviceManager& mgr, const Seat& seat)
	: device_(wl_data_device_manager_get_data_device(mgr, seat))
	, offer_(NULL)
	, target_(NULL)
{
	ASSERT(device_ != NULL);

	static const wl_data_device_listener listener = {
		offer, enter, leave, motion, drop, selection
	};

	wl_data_device_add_listener(*this, &listener, this);
	wl_data_device_set_user_data(*this, this);

	ASSERT(wl_data_device_get_user_data(*this) == this);
}

DataDevice::~DataDevice()
{
	if (offer_ != NULL) {
		delete offer_;
	}
	wl_data_device_destroy(device_);
}

void DataDevice::offer(wl_data_offer *o)
{
	ASSERT(offer_ == NULL);

	offer_ = new DataOffer(o);
}

void DataDevice::enter(wl_surface *surface, int32_t x, int32_t y, uint32_t serial)
{
	ASSERT(target_ == NULL);

	offer_->setSerial(serial);
	offer_->setPosition(x, y);

	target_ = static_cast<DataTarget*>(wl_surface_get_user_data(surface));
	target_->offer(*offer_);
}

void DataDevice::leave()
{
	delete offer_;

	offer_ = NULL;
	target_ = NULL;
}

void DataDevice::motion(int32_t x, int32_t y, uint32_t time)
{
	offer_->setPosition(x, y);
	target_->offer(*offer_);
}

void DataDevice::drop()
{
	target_->drop(*offer_);
}

void DataDevice::selection()
{
	return;
}

namespace wrapper {
	TEST(DataDeviceConstruct)
	{
		Display display;
		Seat seat(display);
		DataDeviceManager mgr(display);
		DataDevice device(mgr, seat);
	}
} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
