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

#ifndef __WFITS_CORE_DATA_DEVICE_H__
#define __WFITS_CORE_DATA_DEVICE_H__

#include <wayland-client.h>

#include "test/tools.h"
#include "data_offer.h"
#include "data_target.h"

namespace wfits {
namespace test {
namespace core {

class DataDevice
{
public:
	DataDevice(const class DataDeviceManager&, const class Seat&);
	~DataDevice();

	operator wl_data_device*() const { return device_; }
	DataOffer* offer() const { return offer_; }
	DataTarget* target() const { return target_; }

private:
	void offer(wl_data_offer *);
	void enter(wl_surface *, int32_t, int32_t, uint32_t);
	void leave();
	void motion(int32_t, int32_t, uint32_t);
	void drop();
	void selection();

	static void offer(void *data, wl_data_device *dd, wl_data_offer *o)
	{
		DataDevice *dev(static_cast<DataDevice*>(data));
		ASSERT(dd == *dev);
		dev->offer(o);
	}

	static void enter(void *data, wl_data_device *dd, uint32_t serial,
		wl_surface *surface, wl_fixed_t x, wl_fixed_t y, wl_data_offer *o)
	{
		DataDevice *dev(static_cast<DataDevice*>(data));
		ASSERT(dd == *dev);
		ASSERT(dev->offer_ != NULL);
		ASSERT(o == *dev->offer_);
		dev->enter(
			surface, wl_fixed_to_int(x),
			wl_fixed_to_int(y), serial
		);
	}

	static void leave(void *data, wl_data_device *dd)
	{
		DataDevice *dev(static_cast<DataDevice*>(data));
		ASSERT(dd == *dev);
		ASSERT(dev->offer_ != NULL);
		ASSERT(dev->target_ != NULL);
		dev->leave();
	}

	static void motion(void *data, wl_data_device *dd, uint32_t time,
		wl_fixed_t x, wl_fixed_t y)
	{
		DataDevice *dev(static_cast<DataDevice*>(data));
		ASSERT(dd == *dev);
		ASSERT(dev->offer_ != NULL);
		ASSERT(dev->target_ != NULL);
		dev->motion(wl_fixed_to_int(x), wl_fixed_to_int(y), time);
	}

	static void drop(void *data, wl_data_device *dd)
	{
		DataDevice *dev(static_cast<DataDevice*>(data));
		ASSERT(dd == *dev);
		ASSERT(dev->offer_ != NULL);
		ASSERT(dev->target_ != NULL);
		dev->drop();
	}

	static void selection(void *data, wl_data_device *dd, wl_data_offer *o)
	{
		DataDevice *dev(static_cast<DataDevice*>(data));
		ASSERT(dd == *dev);
		ASSERT(dev->offer_ != NULL);
		ASSERT(o == *dev->offer_);
		dev->selection();
	}

	wl_data_device	*device_;
	DataOffer	*offer_;
	DataTarget	*target_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
