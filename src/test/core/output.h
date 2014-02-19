/*
 * Copyright © 2014 Intel Corporation
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

#ifndef __WFITS_CORE_OUTPUT_H__
#define __WFITS_CORE_OUTPUT_H__

#include "common/util.h"
#include "display.h"

namespace wfits {
namespace test {
namespace core {

class Output
{
public:
	Output(const Display&);

	virtual ~Output();

	operator wl_output*() const { return wl_output_; }
	const Display& display() const { return display_; }
	const Geometry& geometry() const { return geometry_; }

private:

	static void geometry(void*, wl_output*, int, int, int, int, int, const char*, const char*, int32_t);
	static void mode(void*, wl_output*, uint32_t, int, int, int);

	const Display&		display_;
	wl_output*		wl_output_;
	Geometry		geometry_;
};

} // namespace core
} // namespace test
} // namespace wfits

#endif
