#include <Ecore_Evas.h>

#include "evasobject.h"

EvasObject::EvasObject(Evas_Object* o)
	: obj_(o)
{
	return;
}

EvasObject::~EvasObject()
{
	obj_ = NULL;
}

EvasObject::operator Evas*()
{
	return evas_object_evas_get(*this);
}

EvasObject::operator Ecore_Evas*()
{
	return ecore_evas_object_ecore_evas_get(*this);
}

EvasObject::operator Evas_Object*()
{
	return obj_;
}

void EvasObject::setSize(int w, int h)
{
	evas_object_resize(*this, w, h);
}

void EvasObject::setPosition(int x, int y)
{
	evas_object_move(*this, x, y);
}

void EvasObject::show()
{
	evas_object_show(*this);
}

int EvasObject::getX()
{
	int x;
	evas_object_geometry_get(*this, &x, NULL, NULL, NULL);
	return x;
}

int EvasObject::getY()
{
	int y;
	evas_object_geometry_get(*this, NULL, &y, NULL, NULL);
	return y;
}

int EvasObject::getWidth()
{
	int w;
	evas_object_geometry_get(*this, NULL, NULL, &w, NULL);
// 	evas_output_size_get(*this, &w, NULL);
	return w;
}

int EvasObject::getHeight()
{
	int h;
	evas_object_geometry_get(*this, NULL, NULL, NULL, &h);
// 	evas_output_size_get(*this, NULL, &h);
	return h;
}
