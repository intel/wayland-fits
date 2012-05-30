#include <Elementary.h>

#include "window.h"

Window::Window(const std::string& name, const std::string& title)
	: EvasObject::EvasObject(
		elm_win_util_standard_add(name.c_str(), title.c_str())
	)
{
	return;
}

