#include "surface.h"

Surface::Surface(const Compositor& compositor)
	: compositor_(compositor)
	, wl_surface_(
		wl_compositor_create_surface(compositor))
{
	ASSERT(wl_surface_ != NULL);

	wl_surface_set_user_data(*this, this);

	static const wl_surface_listener listener = {enter, leave};

	wl_surface_add_listener(*this, &listener, this);

	compositor.display().roundtrip();
}

/*virtual*/ Surface::~Surface()
{
	wl_surface_destroy(*this);
}

static void handleDone(void *data, wl_callback *callback, uint32_t time)
{
	bool *done = static_cast<bool*>(data);
	*done = true;
	wl_callback_destroy(callback);
}

static const struct wl_callback_listener doneListener = {handleDone};

void Surface::commit() const
{
	bool		done(false);
	wl_callback*	callback(wl_surface_frame(*this));

	wl_callback_add_listener(callback, &doneListener, &done);
	wl_surface_commit(*this);

	while (not done) {
		compositor().display().dispatch();
	}
}

/*static*/ void Surface::enter(
	void *data, wl_surface *wl_surface, wl_output *wl_output)
{
	Surface* surface = static_cast<Surface*>(data);
	ASSERT(wl_surface == *surface);
}

/*static*/ void Surface::leave(
	void *data, wl_surface *wl_surface, wl_output *wl_output)
{
	Surface* surface = static_cast<Surface*>(data);
	ASSERT(wl_surface == *surface);
}
