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

std::string str_iface_wl_compositor	= "wl_compositor";
std::string str_iface_wl_display	= "wl_display";
std::string str_iface_wl_shm		= "wl_shm";
std::string str_iface_wl_output		= "wl_output";
std::string str_iface_wl_input_device	= "wl_input_device";
std::string str_iface_wl_shell		= "wl_shell";

typedef boost::mpl::list<
	BindInterface<wl_compositor, wl_compositor_interface, str_iface_wl_compositor>,
	BindInterface<wl_display, wl_display_interface, str_iface_wl_display>,
	BindInterface<wl_shm, wl_shm_interface, str_iface_wl_shm>,
	BindInterface<wl_output, wl_output_interface, str_iface_wl_output>,
	BindInterface<wl_input_device, wl_input_device_interface, str_iface_wl_input_device>,
	BindInterface<wl_shell, wl_shell_interface, str_iface_wl_shell>
> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( bind_interface, T, test_types )
{
	T t;
	BOOST_REQUIRE(t.object == NULL);
	t.bind();
	BOOST_CHECK(t.object != NULL);
}
