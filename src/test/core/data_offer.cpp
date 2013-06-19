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

#include <fcntl.h>

#include "test/tools.h"
#include "data_offer.h"

namespace wfits {
namespace test {
namespace core {

DataOffer::DataOffer(wl_data_offer* o)
	: offer_(o)
	, type_("")
	, serial_(0)
	, position_()
{
	ASSERT(offer_ != NULL);

	static const wl_data_offer_listener listener = {offer};

	wl_data_offer_add_listener(*this, &listener, this);
	wl_data_offer_set_user_data(*this, this);

	ASSERT(wl_data_offer_get_user_data(*this) == this);
}

DataOffer::~DataOffer()
{
	wl_data_offer_destroy(offer_);
}

void DataOffer::accept() const
{
	ASSERT(type_.size() > 0);
	ASSERT(serial_ > 0);
	ASSERT(position_.x >= 0);
	ASSERT(position_.y >= 0);

	wl_data_offer_accept(*this, serial_, type_.c_str());
}

int32_t DataOffer::receive() const
{
	ASSERT(type_.size() > 0);
	ASSERT(serial_ > 0);
	ASSERT(position_.x >= 0);
	ASSERT(position_.y >= 0);

	int p[2];

	if (pipe2(p, O_CLOEXEC) == -1) {
		return -1;
	}

	wl_data_offer_receive(*this, type_.c_str(), p[1]);
	close(p[1]);

	return p[0];
}

void DataOffer::setSerial(const uint32_t s)
{
	serial_ = s;
}

void DataOffer::setPosition(const int32_t x, const int32_t y)
{
	position_.x = x;
	position_.y = y;
}

} // namespace core
} // namespace test
} // namespace wfits
