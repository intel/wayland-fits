/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __WFITS_EFL_EVASOBJECT_H__
#define __WFITS_EFL_EVASOBJECT_H__

#include <Evas.h>
#include "common/util.h"

class EvasObject
{
public:
	EvasObject(Evas_Object* = NULL, bool autodel = true);

	virtual ~EvasObject();

	void setSize(int w, int h);
	void setPosition(int x, int y);
	void show();
	void hide();

	int getWidth() const;
	int getHeight() const;
	int getX() const;
	int getY() const;
	
	Geometry getFramespaceGeometry() const;
	Geometry getGeometry() const;
	Position getPosition() const;

	Eina_Bool isVisible() const;


// 	operator Evas*();
	
	operator Evas_Object*();
	operator const Evas_Object*() const;

	void checkSize(const int width, const int height);
	void checkPosition(const int x, const int y);
	void checkVisible(const Eina_Bool isVisible);

private:
	Evas_Object*	obj_;
	bool		autodel_;
};

#endif
