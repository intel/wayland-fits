#ifndef __WAYLAND_EFL_EVAS_OBJECT_H__
#define __WAYLAND_EFL_EVAS_OBJECT_H__

class EvasObject
{
public:
	EvasObject(Evas_Object* = NULL);

	virtual ~EvasObject();

	void setSize(int w, int h);
	void setPosition(int x, int y);
	void show();

	const int getWidth();
	const int getHeight();
	const int getX();
	const int getY();

	operator Evas*();
	operator Ecore_Evas*();
	operator Evas_Object*();

	void checkSize(const int width, const int height);
	void checkPosition(const int x, const int y);
	void checkVisible(const Eina_Bool isVisible);

private:
	Evas_Object*	obj_;

};


#endif
