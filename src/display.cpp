#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/mman.h>

#include <wayland-client.h>

#include "display.h"

namespace wayland {

/*static*/
int Display::evtMaskUpdate(uint32_t mask, void* data)
{
	Display* display = static_cast<Display*>(data);
	display->mask_ = mask;
	return 0;
}

/*static*/
void Display::handleShmFormat(void* data, wl_shm* shm, uint32_t format)
{
	Display* display = static_cast<Display*>(data);
	display->formats_ |= (1 << format);
}

/*static*/
const wl_shm_listener Display::shmListener_ = {
	Display::handleShmFormat
};

/*static*/
void Display::handleGlobal(wl_display* wldisplay, uint32_t id, const char* interface, uint32_t version, void* data)
{
	Display* display = static_cast<Display*>(data);
	const std::string intfc(interface);

	if ("wl_compositor" == intfc) {
		display->compositor_ = static_cast<wl_compositor*>(
			wl_display_bind(
				wldisplay, id, &wl_compositor_interface
			)
		);
	}
	else if ("wl_shell" == intfc) {
		display->shell_ = static_cast<wl_shell*>(
			wl_display_bind(
				wldisplay, id, &wl_shell_interface
			)
		);
	}
	else if ("wl_shm" == intfc) {
		display->shm_ = static_cast<wl_shm*>(
			wl_display_bind(
				wldisplay, id, &wl_shm_interface
			)
		);
		wl_shm_add_listener(display->shm_, &shmListener_, display);
	}
}

Display::Display()
	: display_(wl_display_connect(0))
	, formats_(0)
	, compositor_(0)
	, shell_(0)
	, shm_(0)
	, mask_(0)
{
	if (not display_) {
		std::cerr << "Failed to create display!" << std::endl;
		exit(1);
	}

	wl_display_add_global_listener(display_, handleGlobal, this);
	wl_display_iterate(display_, WL_DISPLAY_READABLE);
	wl_display_roundtrip(display_);

	if (not (formats_ & (1 << WL_SHM_FORMAT_XRGB8888))) {
		std::cerr << "WL_SHM_FORMAT_XRGB32 not available!" << std::endl;
		exit(1);
	}

	wl_display_get_fd(display_, evtMaskUpdate, this);
}

Display::~Display()
{
	if (shm_) {
		wl_shm_destroy(shm_);
	}

	if (shell_) {
		wl_shell_destroy(shell_);
	}

	if (compositor_) {
		wl_compositor_destroy(compositor_);
	}

	wl_display_flush(display_);
	wl_display_disconnect(display_);
}

void Display::run()
{
	while (1)
		wl_display_iterate(display_, mask_);
}

wl_buffer* Display::createShmBuffer(int w, int h, uint32_t format, void** dout)
{
	char	filename[] = "/tmp/wayland-shm-XXXXXX";
	int	fd(mkstemp(filename));

	if (fd < 0) {
		std::cerr << "open " << filename << " failed: %m" << std::endl;
		return NULL;
	}

	int stride(w * 4);
	int size(stride * h);

	if (ftruncate(fd, size) < 0) {
		std::cerr << "ftruncate failed: %m" << std::endl;
		close(fd);
		return NULL;
	}

	void* data(
		mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
	);
	unlink(filename);

	if (data == MAP_FAILED) {
		std::cerr << "mmap failed: %m" << std::endl;
		close(fd);
		return NULL;
	}

	wl_shm_pool* pool(
		wl_shm_create_pool(shm_, fd, size)
	);

	wl_buffer* buffer(
		wl_shm_pool_create_buffer(pool, 0, w, h, stride, format)
	);

	wl_shm_pool_destroy(pool);

	close(fd);

	*dout = data;

	return buffer;
}

} // namespace wayland
