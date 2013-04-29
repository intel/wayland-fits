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

#ifndef __WFITS_CORE_QUERY_H__
#define __WFITS_CORE_QUERY_H__

#include "display.h"
#include "surface.h"
#include "extensions/protocol/wayland-fits-client-protocol.h"

class Query
{
public:
	Query(const Display&);
	virtual ~Query();

	operator wfits_query*() const { return wfits_query_; }
	const Display& display() const { return display_; }

	class Position
	{
	public:
		Position();
		bool done;
		int32_t x;
		int32_t y;
	};

	void expectGlobalPointerPosition(int32_t, int32_t) const;
	Position getGlobalPointerPosition(unsigned = 1) const;

	class Geometry
	{
	public:
		Geometry();
		bool done;
		int32_t x;
		int32_t y;
		int32_t width;
		int32_t height;
	};

	Geometry getSurfaceGeometry(const Surface& surface) const;
private:
	const Display& display_;
	wfits_query* wfits_query_;
};

#endif
