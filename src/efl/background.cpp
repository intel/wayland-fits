#include "background.h"

Background::Background(Evas_Object *parent)
	: EvasObject::EvasObject(
		elm_bg_add(parent)
	)
	, parent(parent)
{
	return;
}

void Background::setColor(int red, int green, int blue)
{
	elm_bg_color_set(*this, red, green, blue);
}

void Background::getColor(int* red, int* green, int* blue)
{
	elm_bg_color_get(*this, red, green, blue);
}

Background::operator Evas_Object*()
{
	return EvasObject::obj_;
}
