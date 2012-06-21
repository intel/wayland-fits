#ifndef __WAYLAND_EFL_ACTIONSLIDER_H__
#define __WAYLAND_EFL_ACTIONSLIDER_H__

#include <string>
#include <Elementary.h>

#include "evasobject.h"

class Actionslider : public EvasObject
{
public:
	Actionslider(Evas_Object *, const std::string& = "Left", const bool = true, const std::string& = "Center", const bool = true, const std::string& = "Right", const bool = true);

	void setPos(Elm_Actionslider_Pos pos);

	Elm_Actionslider_Pos getPos();

private:
	Evas_Object *parent;
};

#endif

