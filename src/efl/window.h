#ifndef __WAYLAND_EFL_WINDOW_H__
#define __WAYLAND_EFL_WINDOW_H__

#include <string>

#include "evasobject.h"

class Window : public EvasObject
{
public:
	Window(const std::string& = "name", const std::string& = "title", const unsigned = 50, const unsigned = 50);
};

#endif
