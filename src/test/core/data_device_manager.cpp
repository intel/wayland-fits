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

#include "test/tools.h"
#include "display.h"
#include "data_device_manager.h"

namespace wfits {
namespace test {
namespace core {

DataDeviceManager::DataDeviceManager(const Display& display)
	: manager_(
		display.bind<wl_data_device_manager>(
			"wl_data_device_manager",
			&wl_data_device_manager_interface))
{
	ASSERT(manager_ != NULL);

	wl_data_device_manager_set_user_data(*this, this);

	ASSERT(wl_data_device_manager_get_user_data(*this) == this);
}

DataDeviceManager::~DataDeviceManager()
{
	wl_data_device_manager_destroy(manager_);
}

wl_data_source* DataDeviceManager::createDataSource() const
{
	return wl_data_device_manager_create_data_source(*this);
}

namespace wrapper {
	TEST(DataDeviceManagerConstruct)
	{
		Display display;
		DataDeviceManager mgr(display);
	}
} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
