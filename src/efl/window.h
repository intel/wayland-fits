#ifndef __WAYLAND_EFL_WINDOW_H__
#define __WAYLAND_EFL_WINDOW_H__

#include <string>

class Window
{
public:
	Window(const std::string&, const std::string&);
	virtual ~Window();

	void setSize(int w, int h);
	void setPosition(int x, int y);
	void show();

	int getWidth();
	int getHeight();
	int getX();
	int getY();
private:
	operator Evas*();
	operator Ecore_Evas*();
	operator Evas_Object*();

	Evas_Object*	win_;
};

#endif
