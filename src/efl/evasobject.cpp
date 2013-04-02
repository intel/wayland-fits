#include "evasobject.h"
#include "common/test.h"

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

// EvasObject::operator Evas*()
// {
// 	return evas_object_evas_get(*this);
// }

EvasObject::operator Evas_Object*()
{
	return obj_;
}

EvasObject::operator const Evas_Object*() const
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

Geometry EvasObject::getGeometry() const
{
	Geometry g;
	evas_object_geometry_get(*this, &g.x, &g.y, &g.width, &g.height);
	return g;
}

Position EvasObject::getPosition() const
{
	Position p;
	evas_object_geometry_get(*this, &p.x, &p.y, NULL, NULL);
	return p;
}

int EvasObject::getX() const
{
	int x;
	evas_object_geometry_get(*this, &x, NULL, NULL, NULL);
	return x;
}

int EvasObject::getY() const
{
	int y;
	evas_object_geometry_get(*this, NULL, &y, NULL, NULL);
	return y;
}

int EvasObject::getWidth() const
{
	int w;
	evas_object_geometry_get(*this, NULL, NULL, &w, NULL);
// 	evas_output_size_get(*this, &w, NULL);
	return w;
}

int EvasObject::getHeight() const
{
	int h;
	evas_object_geometry_get(*this, NULL, NULL, NULL, &h);
// 	evas_output_size_get(*this, NULL, &h);
	return h;
}

Eina_Bool EvasObject::isVisible() const
{
	return evas_object_visible_get(*this);
}

void EvasObject::checkSize(const int width, const int height)
{
	ASSERT_MSG(getWidth() == width, "width: " << getWidth() << " != " << width);
	ASSERT_MSG(getHeight() == height, "height: " << getHeight() << " != " << height);

// 	FAIL_UNLESS_EQUAL(this->getWidth(), width);
// 	FAIL_UNLESS_EQUAL(this->getHeight(), height);
}

void EvasObject::checkPosition(const int xcoord, const int ycoord)
{
	ASSERT_MSG(getX() == xcoord, "x: " << getX() << " != " << xcoord);
	ASSERT_MSG(getY() == ycoord, "y: " << getY() << " != " << ycoord);

// 	FAIL_UNLESS_EQUAL(this->getX(), xcoord);
// 	FAIL_UNLESS_EQUAL(this->getY(), ycoord);
}

void EvasObject::checkVisible(const Eina_Bool isVisible)
{
	FAIL_UNLESS_EQUAL(this->isVisible(), isVisible);
}

