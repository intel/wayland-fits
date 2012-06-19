#ifndef __WAYLAND_EFL_BACKGROUND_H__
#define __WAYLAND_EFL_BACKGROUND_H__

#include <string>
#include <Elementary.h>

#include "evasobject.h"

class Background : public EvasObject
{
public:
	Background(Evas_Object *parent);

	void setColor(int, int, int);

	void getColor(int*, int*, int*);

	operator Evas_Object*();

private:
	Evas_Object *parent;
};

#endif
