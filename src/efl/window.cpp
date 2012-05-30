#include <Elementary.h>

#include "../test.h"
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

Window::operator Evas*()
{
	return evas_object_evas_get(*this);
}

Window::operator Ecore_Evas*()
{
	return ecore_evas_object_ecore_evas_get(*this);
}

Window::operator Evas_Object*()
{
	return win_;
}

void Window::setSize(int w, int h)
{
	evas_object_resize(*this, w, h);
}

void Window::setPosition(int x, int y)
{
	evas_object_move(*this, x, y);
}

void Window::show()
{
	evas_object_show(*this);
}

int Window::getX()
{
	int x;
	evas_object_geometry_get(*this, &x, NULL, NULL, NULL);
	return x;
}

int Window::getY()
{
	int y;
	evas_object_geometry_get(*this, NULL, &y, NULL, NULL);
	return y;
}

int Window::getWidth()
{
	int w;
// 	evas_object_geometry_get(*this, NULL, NULL, &w, NULL);
	evas_output_size_get(*this, &w, NULL);
	return w;
}

int Window::getHeight()
{
	int h;
// 	evas_object_geometry_get(*this, NULL, NULL, NULL, &h);
	evas_output_size_get(*this, NULL, &h);
	return h;
}
