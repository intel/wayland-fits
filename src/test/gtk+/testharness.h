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

#ifndef __WFITS_GTK_GTKTESTHARNESS_H__
#define __WFITS_GTK_GTKTESTHARNESS_H__

#include <gtk/gtk.h>
#include <gdk/gdkwayland.h>

#include "test/tools.h"
#include "test/harness.h"

namespace wfits {
namespace test {
namespace gtk {

class GtkTestHarness : public test::Harness
{
public:
	GtkTestHarness();

	void run();

	/*virtual*/ void yield(const unsigned time = 0.01 * 1e6) const;
	/*virtual*/ const Client& client() const;

private:
	static gboolean idleStep(gpointer);
};

#define WFITS_GTK_HARNESS_TEST_CASE(Harness) \
TEST(Harness) \
{ \
	Harness().run(); \
}

} // namespace gtk
} // namespace test
} // namespace wfits

#endif

