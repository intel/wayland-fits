#include <string>
#include <wayland-client.h>
#include "test.h"

template <typename O, const wl_interface& interface, const std::string& str_interface>
struct BindInterface {
	BindInterface()
		: display(wl_display_connect(0))
		, object(0)
	{
		BOOST_REQUIRE(display);
	}

	void bind()
	{
		wl_display_add_global_listener(display, &BindInterface::callback, this);
		wl_display_iterate(display, WL_DISPLAY_READABLE);
	}

	static void callback(wl_display* display, uint32_t id, const char* iface, uint32_t version, void* data)
	{
		BindInterface* bi = static_cast<BindInterface*>(data);
		if (std::string(iface) == str_interface) {
				bi->object = static_cast<O*>(
					wl_display_bind(
						display, id, &interface
					)
				);
			}
	}

	wl_display*	display;
	O*		object;
};

#define BIND_TEST(name) \
\
std::string str_iface_##name = #name; \
typedef BindInterface<name, name##_interface, str_iface_##name> bind_##name##_interface; \
BOOST_FIXTURE_TEST_CASE(bind_##name, bind_##name##_interface) \
{ \
	BOOST_REQUIRE(object == NULL); \
	bind(); \
	BOOST_CHECK(object != NULL); \
}

BIND_TEST(wl_compositor)
BIND_TEST(wl_display)
BIND_TEST(wl_shm)
BIND_TEST(wl_output)
BIND_TEST(wl_input_device)
BIND_TEST(wl_shell)
