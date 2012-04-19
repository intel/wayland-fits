// #include <sys/epoll.h>
#include <iostream>
#include <cstdlib>

#include <wayland-client.h>

#include "display.h"

namespace wayland {

/*static*/
int Display::evtMaskUpdate(uint32_t mask, void *data)
{
	Display* display = static_cast<Display*>(data);
	display->mask_ = mask;

	return 0;
}

/*static*/
void Display::handleGlobal(wl_display* wldisplay, uint32_t id, const char* interface, uint32_t version, void* data)
{
	Display* display = static_cast<Display*>(data);
	if ("wl_compositor" == interface)
	{
		display->compositor_ = static_cast<wl_compositor*>(
			wl_display_bind(
				wldisplay, id, &wl_compositor_interface
			)
		);
	}
	else if ("wl_shell" == interface)
	{
		display->shell_ = static_cast<wl_shell*>(
			wl_display_bind(
				wldisplay, id, &wl_shell_interface
			)
		);
	}
}

Display::Display()
	: display_(wl_display_connect(0))
{
	if (not display_)
	{
		std::cerr << "Failed to create display!" << std::endl;
		exit(1);
	}

	wl_display_add_global_listener(display_, handleGlobal, this);
	wl_display_iterate(display_, WL_DISPLAY_READABLE);
	if (initEGL() < 0)
	{
		std::cerr << "Failed to initialize egl!" << std::endl;
		exit(1);
	}
}

bool Display::initEGL()
{
	EGLint major, minor, n;

	static const EGLint argb_cfg_attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PIXMAP_BIT,
		EGL_RED_SIZE, 1,
		EGL_GREEN_SIZE, 1,
		EGL_BLUE_SIZE, 1,
		EGL_ALPHA_SIZE, 1,
		EGL_DEPTH_SIZE, 1,
		EGL_RENDERABLE_TYPE, GL_BIT,
		EGL_NONE
	};

#ifdef USE_CAIRO_GLESV2
	static const EGLint context_attribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLint api = EGL_OPENGL_ES_API;
#else
	EGLint *context_attribs = NULL;
	EGLint api = EGL_OPENGL_API;
#endif

	eglDisplay_ = eglGetDisplay( (EGLNativeDisplayType)display_);
	if (not eglInitialize(eglDisplay_, &major, &minor))
	{
// 		fprintf(stderr, "failed to initialize display\n");
		return false;
	}

	if (not eglBindAPI(api))
	{
// 		fprintf(stderr, "failed to bind api EGL_OPENGL_API\n");
		return false;
	}

	if (not eglChooseConfig(eglDisplay_, argb_cfg_attribs,
			     &argbConfig_, 1, &n) || n != 1)
	{
// 		fprintf(stderr, "failed to choose argb config\n");
		return false;
	}

	argbContext_ = eglCreateContext(eglDisplay_, argbConfig_,
				       EGL_NO_CONTEXT, context_attribs);
	if (not argbContext_)
	{
// 		fprintf(stderr, "failed to create context\n");
		return false;
	}

	if (not eglMakeCurrent(eglDisplay_, NULL, NULL, argbContext_))
	{
// 		fprintf(stderr, "failed to make context current\n");
		return false;
	}

#ifdef HAVE_CAIRO_EGL
	argbDevice_ = cairo_egl_device_create(eglDisplay_, argbContext_);
	if (cairo_device_status(argbDevice_) != CAIRO_STATUS_SUCCESS)
	{
// 		fprintf(stderr, "failed to get cairo egl argb device\n");
		return false;
	}
#endif

	return true;
}


} // namespace wayland
