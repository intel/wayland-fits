#include <string>
#include <wayland-client.h>
#include "common/test.h"

template <typename O, const wl_interface& interface, const std::string& str_interface>
class BindInterface
{
public:
	BindInterface()
		: display_(wl_display_connect(0))
		, object_(0)
		, listener_(0)
	{
		FAIL_IF_EQUAL(display_, NULL);
	}

	~BindInterface()
	{
		wl_display_remove_global_listener(display_, listener_);
		wl_display_disconnect(display_);
	}

	void bind()
	{
		listener_ = wl_display_add_global_listener(display_, &BindInterface::callback, this);
		wl_display_iterate(display_, WL_DISPLAY_READABLE);
	}

	static void callback(wl_display* display, uint32_t id, const char* iface, uint32_t version, void* data)
	{
		BindInterface* bi = static_cast<BindInterface*>(data);
		if (std::string(iface) == str_interface) {
				bi->object_ = static_cast<O*>(
					wl_display_bind(
						display, id, &interface
					)
				);
			}
	}

	wl_display*			display_;
	O*					object_;
	wl_global_listener*	listener_;
};

#define BIND_TEST(name) \
	std::string str_iface_##name = #name; \
	typedef BindInterface<name, name##_interface, str_iface_##name> bind_##name##_interface; \
	TEST(bind_##name, "Core/BindInterface") \
	{ \
		bind_##name##_interface bi; \
		FAIL_UNLESS(bi.object_ == NULL); \
		bi.bind(); \
		FAIL_IF(bi.object_ == NULL); \
	}

BIND_TEST(wl_compositor)
BIND_TEST(wl_display)
BIND_TEST(wl_shm)
BIND_TEST(wl_output)
BIND_TEST(wl_seat)
BIND_TEST(wl_shell)
