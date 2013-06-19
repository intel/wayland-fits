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

#ifndef __WFITS_CORE_DATA_OFFER_H__
#define __WFITS_CORE_DATA_OFFER_H__

#include <wayland-client.h>
#include <string>
#include "common/util.h"

namespace wfits {
namespace test {
namespace core {

class DataOffer
{
public:
	DataOffer(wl_data_offer *);
	~DataOffer();

	void accept() const;
	int32_t receive() const;

	const std::string& type() const { return type_; }
	const uint32_t& serial() const { return serial_; }
	const Position& position() const { return position_; }
	operator wl_data_offer*() const { return offer_; }

private:
	friend class DataDevice;

	static void offer(void *data, wl_data_offer *o, const char *t)
	{
		DataOffer* dof(static_cast<DataOffer*>(data));
		ASSERT(o == *dof);
		dof->type_ = std::string(strdup(t));
	}

	void setSerial(const uint32_t);
	void setPosition(const int32_t, const int32_t);

	wl_data_offer	*offer_;
	std::string	type_;
	uint32_t	serial_;
	Position	position_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
