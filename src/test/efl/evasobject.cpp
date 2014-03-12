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

#include "application.h"
#include "evasobject.h"

namespace wfits {
namespace test {
namespace efl {

EvasObject::EvasObject(Evas_Object* o, bool autodel)
	: obj_(o)
	, autodel_(autodel)
{
	return;
}

EvasObject::~EvasObject()
{
	if (obj_ and autodel_)
	{
		Application::synchronized(
			[this]() {
				evas_object_del(obj_);
			}
		);
	}
	obj_ = NULL;
}

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
	Application::synchronized(
		[this, &w, &h]() {
			evas_object_resize(*this, w, h);
		}
	);
}

void EvasObject::setPosition(int x, int y)
{
	Application::synchronized(
		[this, &x, &y]() {
			evas_object_move(*this, x, y);
		}
	);
}

void EvasObject::show()
{
	Application::synchronized(
		[this]() {
			evas_object_show(*this);
		}
	);
}

void EvasObject::hide()
{
	Application::synchronized(
		[this]() {
			evas_object_hide(*this);
		}
	);
}

Geometry EvasObject::getFramespaceGeometry() const
{
	Geometry g;

	Application::synchronized(
		[this, &g]() {
			evas_output_framespace_get(
				evas_object_evas_get(*this),
				&g.x, &g.y, &g.width, &g.height
			);
		}
	);

	return g;
}


Geometry EvasObject::getGeometry() const
{
	Geometry g;

	Application::synchronized(
		[this, &g]() {
			evas_object_geometry_get(
				*this, &g.x, &g.y,
				&g.width, &g.height
			);
		}
	);

	return g;
}

Position EvasObject::getPosition() const
{
	Position p;

	Application::synchronized(
		[this, &p]() {
			evas_object_geometry_get(
				*this, &p.x, &p.y,
				NULL, NULL
			);
		}
	);

	return p;
}

int EvasObject::getX() const
{
	int x;

	Application::synchronized(
		[this, &x]() {
			evas_object_geometry_get(
				*this, &x, NULL,
				NULL, NULL
			);
		}
	);

	return x;
}

int EvasObject::getY() const
{
	int y;

	Application::synchronized(
		[this, &y]() {
			evas_object_geometry_get(
				*this, NULL, &y,
				NULL, NULL
			);
		}
	);

	return y;
}

int EvasObject::getWidth() const
{
	int w;

	Application::synchronized(
		[this, &w]() {
			evas_object_geometry_get(
				*this, NULL, NULL,
				&w, NULL
			);
		}
	);

	return w;
}

int EvasObject::getHeight() const
{
	int h;

	Application::synchronized(
		[this, &h]() {
			evas_object_geometry_get(
				*this, NULL, NULL,
				NULL, &h
			);
		}
	);

	return h;
}

Eina_Bool EvasObject::isVisible() const
{
	return Application::synchronizedResult(
		[this]()->Eina_Bool {
			return evas_object_visible_get(*this);
		}
	);
}

void EvasObject::checkSize(const int width, const int height)
{
	const int w(getWidth());
	const int h(getHeight());

	ASSERT_MSG(w == width, "width: " << w << " != " << width);
	ASSERT_MSG(h == height, "height: " << h << " != " << height);
}

void EvasObject::checkPosition(const int xcoord, const int ycoord)
{
	const int x(getX());
	const int y(getY());

	ASSERT_MSG(x == xcoord, "x: " << x << " != " << xcoord);
	ASSERT_MSG(y == ycoord, "y: " << y << " != " << ycoord);
}

void EvasObject::checkVisible(const Eina_Bool isVisible)
{
	FAIL_UNLESS_EQUAL(this->isVisible(), isVisible);
}

} // namespace efl
} // namespace test
} // namespace wfits
