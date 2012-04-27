#ifndef __WAYLAND_DISPLAY_H__
#define __WAYLAND_DISPLAY_H__

#include <cstdint>

struct wl_display;
struct wl_compositor;
struct wl_shell;
struct wl_shm;
struct wl_buffer;
struct wl_shm_listener;

namespace wayland {

class Display {

public:
	Display();

	virtual ~Display();

	wl_compositor*	compositor() { return compositor_; }
	wl_display*	display() { return display_; }
	wl_shell*	shell() { return shell_; }

	wl_buffer*	createShmBuffer(int, int, uint32_t, void**);

	void		run();

private:
	wl_display*	display_;
	uint32_t	formats_;
	wl_compositor*	compositor_;
	wl_shell*	shell_;
	wl_shm*		shm_;
	uint32_t	mask_;

	static int evtMaskUpdate(uint32_t, void*);
	static void handleShmFormat(void*, wl_shm*, uint32_t);
	static void handleGlobal(wl_display*, uint32_t, const char*, uint32_t, void*);

	static const wl_shm_listener shmListener_;
};

} // namespace wayland

#endif
