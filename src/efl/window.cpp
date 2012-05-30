#include <Elementary.h>

#include "window.h"

Window::Window(const std::string& name, const std::string& title)
{
	win_ = elm_win_util_standard_add(name.c_str(), title.c_str());
}

/* virtual */
Window::~Window()
{
	win_ = NULL;
}

void Window::setSize(int w, int h)
{
	evas_object_resize(win_, w, h);
}

void Window::setPosition(int x, int y)
{
	evas_object_move(win_, x, y);
}

void Window::show()
{
	evas_object_show(win_);
}

int Window::getWidth()
{
	int x, y, w, h;
	evas_object_geometry_get(win_, &x, &y, &w, &h);
	return w;
}

int Window::getHeight()
{
	int x, y, w, h;
	evas_object_geometry_get(win_, &x, &y, &w, &h);
	return h;
}
