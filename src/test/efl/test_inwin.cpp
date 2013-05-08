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

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

class Inwin : public EvasObject
{
public:
	Inwin(EvasObject &parent)
		: EvasObject::EvasObject(elm_win_inwin_add(parent))
	{
		return;
	}
};

//TODO: more rigorous tests

// NOTE: inwin's cannot be resized explicitly

typedef PositionObjectTest<Inwin> InwinPositionTest;
typedef VisibleObjectTest<Inwin> InwinVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(InwinPositionTest, "Inwin")
WAYLAND_ELM_HARNESS_TEST_CASE(InwinVisibilityTest, "Inwin")

} // namespace efl
} // namespace test
} // namespace wfits
