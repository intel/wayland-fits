#include <Ecore_Evas.h>

#include "evasobject.h"
#include "application.h"

EvasObject::EvasObject(Evas_Object* o)
	: obj_(o)
{
	return;
}

EvasObject::~EvasObject()
{
	if (obj_)
	{
		evas_object_del(obj_);
	}
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

void EvasObject::hide()
{
	evas_object_hide(*this);
}

const int EvasObject::getX()
{
	int x;
	evas_object_geometry_get(*this, &x, NULL, NULL, NULL);
	return x;
}

const int EvasObject::getY()
{
	int y;
	evas_object_geometry_get(*this, NULL, &y, NULL, NULL);
	return y;
}

const int EvasObject::getWidth()
{
	int w;
	evas_object_geometry_get(*this, NULL, NULL, &w, NULL);
// 	evas_output_size_get(*this, &w, NULL);
	return w;
}

const int EvasObject::getHeight()
{
	int h;
	evas_object_geometry_get(*this, NULL, NULL, NULL, &h);
// 	evas_output_size_get(*this, NULL, &h);
	return h;
}

const Eina_Bool EvasObject::isVisible()
{
	return evas_object_visible_get(*this);
}

void EvasObject::checkSize(const int width, const int height)
{
	BOOST_CHECK_EQUAL(this->getWidth(), width);
	BOOST_CHECK_EQUAL(this->getHeight(), height);
}

void EvasObject::checkPosition(const int xcoord, const int ycoord)
{
	BOOST_CHECK_EQUAL(this->getX(), xcoord);
	BOOST_CHECK_EQUAL(this->getY(), ycoord);
}

void EvasObject::checkVisible(const Eina_Bool isVisible)
{
	BOOST_CHECK_EQUAL(this->isVisible(), isVisible);
}

void EvasObject::checkHidden(const Eina_Bool isHidden)
{
	BOOST_CHECK_EQUAL(this->isVisible(), (isHidden == EINA_TRUE ? EINA_FALSE : EINA_TRUE));
}
