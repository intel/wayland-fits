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

#include "testharness.h"

namespace wfits {
namespace test {
namespace gtk {

GtkTestHarness::GtkTestHarness()
	: test::Harness::Harness()
{
	gtk_init(
		&TheGlobalSuite::instance().argc,
		&TheGlobalSuite::instance().argv
	);

	GdkDisplay* display(gdk_display_get_default());

	ASSERT(std::string(gdk_display_get_name(display)) == "Wayland");
}

void GtkTestHarness::run()
{
	setup();

	g_idle_add(idleStep, this);

	gtk_main();

	teardown();

	ASSERT(not haveStep());
}

const test::Client& GtkTestHarness::client() const
{
	static const test::Client c(
		gdk_wayland_display_get_wl_display(
			gdk_display_get_default()
		)
	);
	return c;
}

void GtkTestHarness::yield(const unsigned time, bool strict) const
{
	gtk_main_iteration_do(gtk_false());
	usleep(time);
}

/*static*/
gboolean GtkTestHarness::idleStep(gpointer data)
{
	GtkTestHarness* harness = static_cast<GtkTestHarness*>(data);

	if (harness->haveStep()) {
		harness->runNextStep();
		harness->yield();
		return gtk_true();
	}

	gtk_main_quit();

	return gtk_false();
}

class SimpleHarnessTest : public GtkTestHarness
{
public:
	SimpleHarnessTest()
		: GtkTestHarness::GtkTestHarness()
		, nsetup_(0)
		, nsteps_(0)
		, nteardown_(0)
	{
		return;
	}

	~SimpleHarnessTest()
	{
		ASSERT(1 == nsetup_);
		ASSERT(50 == nsteps_);
		ASSERT(1 == nteardown_);
	}

	void setup()
	{
		++nsetup_;
		for (unsigned i(0); i < 50; ++i) {
			queueStep(boost::bind(&SimpleHarnessTest::step, boost::ref(*this)));
		}
	}

	void step()
	{
		++nsteps_;
	}

	void teardown()
	{
		++nteardown_;
	}

private:
	unsigned nsetup_;
	unsigned nsteps_;
	unsigned nteardown_;
};

class PointerInterfaceTest : public GtkTestHarness
{
public:
	PointerInterfaceTest()
		: GtkTestHarness::GtkTestHarness()
	{
		return;
	}

	void setup()
	{
		queueStep(boost::bind(&PointerInterfaceTest::test, boost::ref(*this)));
	}

	void test()
	{
		for (int x(0), y(0); x < 300; x += 51, y += 37) {
			setGlobalPointerPosition(x, y);
			Position p(getGlobalPointerPosition());
			ASSERT(p.x == x and p.y == y);
			expectGlobalPointerPosition(x, y);

			p.x += 1;
			p.y += 1;
			setGlobalPointerPosition(p);
			Position np(getGlobalPointerPosition());
			ASSERT(np.x == p.x and np.y == p.y);
			expectGlobalPointerPosition(p);
		}
	}
};

WFITS_GTK_HARNESS_TEST_CASE(SimpleHarnessTest)
WFITS_GTK_HARNESS_TEST_CASE(PointerInterfaceTest)

} // namespace gtk
} // namespace test
} // namespace wfits
