#include "actionslider.h"

Actionslider::Actionslider(Evas_Object *parent, const std::string& left, const bool leftOn, const std::string& center, const bool centerOn, const std::string& right, const bool rightOn)
	: EvasObject::EvasObject(
		elm_actionslider_add(parent)
	)
	, parent(parent)
{
	elm_object_part_text_set(*this, "left", left.c_str());
	elm_object_part_text_set(*this, "center", center.c_str());
	elm_object_part_text_set(*this, "right", right.c_str());

	unsigned int posEnabled = ELM_ACTIONSLIDER_NONE;
	if (leftOn)
	{
		posEnabled |= ELM_ACTIONSLIDER_LEFT;
	}
	if (centerOn)
	{
		posEnabled |= ELM_ACTIONSLIDER_CENTER;
	}
	if (rightOn)
	{
		posEnabled |= ELM_ACTIONSLIDER_RIGHT;
	}
	elm_actionslider_enabled_pos_set(*this, static_cast<Elm_Actionslider_Pos>(posEnabled));
}

void Actionslider::setPos(Elm_Actionslider_Pos pos)
{
	elm_actionslider_indicator_pos_set(*this, pos);
}

Elm_Actionslider_Pos Actionslider::getPos()
{
	return elm_actionslider_indicator_pos_get(*this);
}

Actionslider::operator Evas_Object*()
{
	return EvasObject::obj_;
}
