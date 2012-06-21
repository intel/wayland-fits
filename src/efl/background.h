#ifndef __WAYLAND_EFL_BACKGROUND_H__
#define __WAYLAND_EFL_BACKGROUND_H__

#ifndef MEDIA_PATH
#error "The MEDIA_PATH macro must be defined-- pass definition in CXXFLAGS."
#endif

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <Elementary.h>
#include "evasobject.h"

class Background : public EvasObject
{
public:
	Background(Evas_Object *parent);

	void setColor(int, int, int);
	void setImage(const boost::filesystem::path&);
	void setImageOpt(Elm_Bg_Option);

	void getColor(int*, int*, int*);
	void getImage(boost::filesystem::path&);
	Elm_Bg_Option getImageOpt();

private:
	Evas_Object *parent;
};

#endif
