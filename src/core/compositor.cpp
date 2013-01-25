#include "compositor.h"

Compositor::Compositor(const Display& display)
	: display_(display)
	, wl_compositor_(
		display.bind<wl_compositor>(
			"wl_compositor", &wl_compositor_interface))
{
	ASSERT(wl_compositor_ != NULL);

	wl_compositor_set_user_data(*this, this);
}

/*virtual*/ Compositor::~Compositor()
{
	wl_compositor_destroy(*this);
}

TEST(Compositor, "Core/Wrapper")
{
	Display display;
	Compositor compositor(display);

	FAIL_UNLESS_EQUAL(&compositor.display(), &display);
	FAIL_IF((wl_compositor*)compositor == NULL);
	FAIL_UNLESS_EQUAL(wl_compositor_get_user_data(compositor), &compositor);
}
