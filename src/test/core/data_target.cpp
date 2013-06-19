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
#include "data_target.h"

namespace wfits {
namespace test {
namespace core {

DataTarget::DataTarget(const std::string& t)
	: type_(t)
	, fd_(-1)
	, offer_(NULL)
{
	return;
}

void DataTarget::offer(const DataOffer& offer)
{
	if (offer.type() == type_) {
		offer_ = &offer;
		offer.accept();
	}
}

void DataTarget::drop(const DataOffer& offer)
{
	if (offer.type() == type_) {
		ASSERT(offer_ == &offer);
		fd_ = offer.receive();
	}
}

const char * DataTarget::readDropData()
{
	ASSERT(fd_ != -1);

	unsigned len;
	char buffer[4096];

	len = read(fd_, buffer, sizeof buffer);

	ASSERT(len > 0);

	close(fd_);
	fd_ = -1;

	return strndup(buffer, len);
}

} // namespace core
} // namespace test
} // namespace wfits
