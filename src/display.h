#ifndef __WAYLAND_DISPLAY_H__
#define __WAYLAND_DISPLAY_H__

#include <EGL/egl.h>
#include <EGL/eglext.h>

#ifdef USE_CAIRO_GLESV2
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	#define GL_BIT EGL_OPENGL_ES2_BIT
#else
	#include <GL/gl.h>
	#define GL_BIT EGL_OPENGL_BIT
#endif

#ifdef HAVE_CAIRO_EGL
	#include <cairo-gl.h>
#endif


#include <cstdint>

struct wl_display;
struct wl_compositor;
struct wl_shell;

namespace wayland {

class Display {

public:
	Display();

	wl_compositor*	compositor() { return compositor_; }
	wl_display*	display() { return display_; }
	wl_shell*	shell() { return shell_; }
	EGLConfig&	config() { return argbConfig_; }
	EGLContext&	context() { return argbContext_; }
	
private:
	wl_display*	display_;
	uint32_t	mask_;
	wl_compositor*	compositor_;
	wl_shell*	shell_;

	bool		running_;

	EGLDisplay	eglDisplay_;
	EGLConfig	argbConfig_;
	EGLContext	argbContext_;

	static int evtMaskUpdate(uint32_t, void*);
	static void handleGlobal(wl_display*, uint32_t, const char*, uint32_t, void*);

	bool initEGL();
};

} // namespace wayland

#endif
