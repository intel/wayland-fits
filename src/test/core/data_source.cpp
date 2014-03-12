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

#include "surface.h"
#include "data_device_manager.h"
#include "data_device.h"
#include "data_source.h"

namespace wfits {
namespace test {
namespace core {


DataSource::DataSource(const DataDeviceManager& ddm, const std::string& type, const char *data, uint32_t len)
	: type_(type)
	, data_(strndup(data, len))
	, len_(len)
	, source_(ddm.createDataSource())
	, sent_(false)
{
	ASSERT(source_ != NULL);

	static const wl_data_source_listener listener = {
		target, send, cancelled
	};

	wl_data_source_add_listener(source_, &listener, this);
	wl_data_source_offer(*this, type.c_str());
	wl_data_source_set_user_data(*this, this);

	ASSERT(wl_data_source_get_user_data(*this) == this);
}

DataSource::~DataSource()
{
	wl_data_source_destroy(source_);
}

void DataSource::startDrag(uint32_t serial, const DataDevice& dd, const Surface& s)
{
	sent_ = false;
	wl_data_device_start_drag(
		dd, *this, s,
		NULL, serial
	);
}

void DataSource::target(const std::string& type)
{
	return;
}

void DataSource::send(const std::string& type, const int32_t fd)
{
	ASSERT(write(fd, data_, len_) >= 0);

	close(fd);

	sent_ = true;
}

void DataSource::cancelled()
{
	return;
}

/*static*/
void DataSource::target(void *data, wl_data_source *ds, const char *type)
{
	DataSource *dataSource(static_cast<DataSource*>(data));
	ASSERT(ds == dataSource->source_);
	ASSERT(type == dataSource->type_);
	dataSource->target(std::string(type));
}

/*static*/
void DataSource::send(void *data, wl_data_source *ds, const char *type, int32_t fd)
{
	DataSource *dataSource(static_cast<DataSource*>(data));
	ASSERT(ds == dataSource->source_);
	ASSERT(type == dataSource->type_);
	dataSource->send(std::string(type), fd);
}

/*static*/
void DataSource::cancelled(void *data, wl_data_source *ds)
{
	DataSource *dataSource(static_cast<DataSource*>(data));
	ASSERT(ds == dataSource->source_);
	dataSource->cancelled();
}


namespace wrapper {
	TEST(DataSourceConstruct)
	{
		std::string data("data");
		Display display;
		DataDeviceManager mgr(display);
		DataSource source(mgr, "type", data.c_str(), data.size());
	}
} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
