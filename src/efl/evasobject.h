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
	void hide();

	const int getWidth();
	const int getHeight();
	const int getX();
	const int getY();
	const Eina_Bool isVisible();

	//FIXME: these compiled fine before... but for some reason they are flagged
	// as ambiguous by the compiler now... this started happending when
	// EO was added as dependency to Evas.
// 	operator Evas*();
// 	operator Ecore_Evas*();
	
	operator Evas_Object*();

	void checkSize(const int width, const int height);
	void checkPosition(const int x, const int y);
	void checkVisible(const Eina_Bool isVisible);

private:
	Evas_Object*	obj_;

};

#endif
