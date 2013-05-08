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

#ifndef __WFITS_TEST_CLIENT_H__
#define __WFITS_TEST_CLIENT_H__

#include "common/util.h"
#include "extensions/protocol/wayland-fits-client-protocol.h"

namespace wfits {
namespace test {

class Client
{
public:
	Client(wl_display*);

	virtual ~Client();

	class QueryRequest
	{
	public:
		QueryRequest();
		bool done;
		void* data;
	};

	/** QueryRequest::data will be a Geometry object **/
	QueryRequest* makeGeometryRequest(wl_surface*) const;

	/** QueryRequest::data will be a Position object **/
	QueryRequest* makePointerPositionRequest() const;

	void movePointerTo(int32_t, int32_t) const;
	void movePointerTo(const Position&) const;

	void sendKey(uint32_t, uint32_t) const;

private:
	static void bind_wfits(void *, wl_registry *, uint32_t, const char *, uint32_t);

	wfits_input*	wfits_input_;
	wfits_query*	wfits_query_;
};

} // namespace test
} // namespace wfits

#endif
