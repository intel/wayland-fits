#ifndef __WAYLAND_EFL_EVAS_OBJECT_H__
#define __WAYLAND_EFL_EVAS_OBJECT_H__

class EvasObject
{
public:
	EvasObject(Evas_Object* o);

	virtual ~EvasObject();

	void setSize(int w, int h);
	void setPosition(int x, int y);
	void show();

	int getWidth();
	int getHeight();
	int getX();
	int getY();

	operator Evas*();
	operator Ecore_Evas*();
	operator Evas_Object*();

private:
	Evas_Object*	obj_;
};


#endif
