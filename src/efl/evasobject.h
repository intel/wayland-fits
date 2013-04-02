#ifndef __WAYLAND_EFL_EVAS_OBJECT_H__
#define __WAYLAND_EFL_EVAS_OBJECT_H__

#include <Evas.h>
#include "common/util.h"

class EvasObject
{
public:
	EvasObject(Evas_Object* = NULL, bool autodel = true);

	virtual ~EvasObject();

	void setSize(int w, int h);
	void setPosition(int x, int y);
	void show();
	void hide();

	int getWidth() const;
	int getHeight() const;
	int getX() const;
	int getY() const;
	Geometry getGeometry() const;
	Position getPosition() const;

	Eina_Bool isVisible() const;


// 	operator Evas*();
	
	operator Evas_Object*();
	operator const Evas_Object*() const;

	void checkSize(const int width, const int height);
	void checkPosition(const int x, const int y);
	void checkVisible(const Eina_Bool isVisible);

private:
	Evas_Object*	obj_;
	bool		autodel_;
};

#endif
