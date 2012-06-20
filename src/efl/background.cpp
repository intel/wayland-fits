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

void Background::setImage(const path& p)
{
	(void) elm_bg_file_set(*this, p.c_str(), NULL);
}

void Background::getColor(int* red, int* green, int* blue)
{
	elm_bg_color_get(*this, red, green, blue);
}

void Background::getImage(path& p)
{
	char* ret = NULL;

	elm_bg_file_get(*this, const_cast<const char**>(&ret), NULL);

	p = ret;
}

Background::operator Evas_Object*()
{
	return EvasObject::obj_;
}
