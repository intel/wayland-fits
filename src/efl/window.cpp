#include <Elementary.h>

#include "window.h"

Window::Window(const std::string& name, const std::string& title, const unsigned width, const unsigned height)
	: EvasObject::EvasObject(
		elm_win_util_standard_add(name.c_str(), title.c_str())
	)
{
	setSize(width, height);
}

