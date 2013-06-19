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

#ifndef __WFITS_CORE_DATA_SOURCE_H__
#define __WFITS_CORE_DATA_SOURCE_H__

#include <wayland-client.h>
#include <string>

#include "test/tools.h"

namespace wfits {
namespace test {
namespace core {

class DataSource
{
public:
	DataSource(const class DataDeviceManager&, const std::string&, const char *, uint32_t);
	~DataSource();

	void startDrag(uint32_t, const class DataDevice&, const class Surface&);
	bool sent() const { return sent_; }
	operator wl_data_source*() const { return source_; }

private:
	void target(const std::string&);
	void send(const std::string&, const int32_t);
	void cancelled();

	static void target(void *data, wl_data_source *ds, const char *type)
	{
		DataSource *dataSource(static_cast<DataSource*>(data));
		ASSERT(ds == dataSource->source_);
		ASSERT(type == dataSource->type_);
		dataSource->target(std::string(type));
	}

	static void send(void *data, wl_data_source *ds, const char *type, int32_t fd)
	{
		DataSource *dataSource(static_cast<DataSource*>(data));
		ASSERT(ds == dataSource->source_);
		ASSERT(type == dataSource->type_);
		dataSource->send(std::string(type), fd);
	}

	static void cancelled(void *data, wl_data_source *ds)
	{
		DataSource *dataSource(static_cast<DataSource*>(data));
		ASSERT(ds == dataSource->source_);
		dataSource->cancelled();
	}

	const std::string	type_;
	const char		*data_;
	uint32_t		len_;
	wl_data_source		*source_;
	bool			sent_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
