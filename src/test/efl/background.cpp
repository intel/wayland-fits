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
#include "background.h"

namespace wfits {
namespace test {
namespace efl {

using boost::filesystem::path;

Background::Background(EvasObject& parent)
	: EvasObject::EvasObject(elm_bg_add(parent))
{
	return;
}

void Background::setColor(int red, int green, int blue)
{
	Application::synchronized(
		[this, &red, &green, &blue]() {
			elm_bg_color_set(*this, red, green, blue);
		}
	);
}

void Background::setImage(const path& p)
{
	Application::synchronized(
		[this, &p]() {
			elm_bg_file_set(*this, p.c_str(), NULL);
		}
	);
}

void Background::setImageOpt(Elm_Bg_Option option)
{
	Application::synchronized(
		[this, &option]() {
			elm_bg_option_set(*this, option);
		}
	);
}

void Background::getColor(int* red, int* green, int* blue)
{
	Application::synchronized(
		[this, &red, &green, &blue]() {
			elm_bg_color_get(*this, red, green, blue);
		}
	);
}

void Background::getImage(path& p)
{
	char* ret = NULL;

	Application::synchronized(
		[this, &ret]() {
			elm_bg_file_get(*this, const_cast<const char**>(&ret), NULL);
		}
	);

	p = ret;
}

Elm_Bg_Option Background::getImageOpt()
{
	return Application::synchronizedResult(
		[this]()->Elm_Bg_Option {
			return elm_bg_option_get(*this);
		}
	);
}

} // namespace efl
} // namespace test
} // namespace wfits
