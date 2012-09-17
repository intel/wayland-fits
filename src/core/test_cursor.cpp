#include <string>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include "common/test.h"

/** This is as defined in wayland/cursor/wayland-cursor.c **/
struct wl_cursor_theme {
	unsigned int cursor_count;
	struct wl_cursor **cursors;
	struct wl_shm *shm;
	struct shm_pool *pool;
	char *name;
	int size;
};

class TestCursorTheme
{
public:
	TestCursorTheme(const std::string& themeName = "default")
		: display_(wl_display_connect(0))
		, theme_(0)
		, listener_(0)
		, shm_(0)
		, themeName_(themeName)
	{
		FAIL_IF_EQUAL(display_, NULL);
	}

	~TestCursorTheme()
	{
		wl_cursor_theme_destroy(theme_);
		wl_display_remove_global_listener(display_, listener_);
		wl_display_disconnect(display_);
	}

	void run()
	{
		listener_ = wl_display_add_global_listener(display_, &TestCursorTheme::callback, this);
		wl_display_iterate(display_, WL_DISPLAY_READABLE);
	}

	static void callback(wl_display* display, uint32_t id, const char* iface, uint32_t version, void* data)
	{
		TestCursorTheme* test = static_cast<TestCursorTheme*>(data);
		if (std::string(iface) == "wl_shm")
		{
			test->shm_ = static_cast<wl_shm*>(wl_display_bind(display, id, &wl_shm_interface));
			FAIL_IF_EQUAL(test->shm_, NULL);
			test->theme_ = wl_cursor_theme_load(test->themeName_.c_str(), 32, test->shm_);
			FAIL_IF_EQUAL(test->theme_, NULL);
		}
	}

	wl_display*			display_;
	wl_cursor_theme*	theme_;
	wl_global_listener* listener_;
	wl_shm*				shm_;
	const std::string	themeName_;
};

TEST(load_default_theme, "Core/Cursor")
{
	TestCursorTheme test;
	ASSERT(test.themeName_ == "default");
	test.run();
	FAIL_UNLESS_EQUAL(test.themeName_, std::string(test.theme_->name));
}

TEST(load_missing_theme, "Core/Cursor")
{
	TestCursorTheme test("this_theme_does_not_exist");
	ASSERT(test.themeName_ == "this_theme_does_not_exist");
	test.run();
	FAIL_UNLESS_EQUAL("default", std::string(test.theme_->name));
}

