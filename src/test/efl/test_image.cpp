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

#include <boost/filesystem.hpp>

#include <vector>

#include "templates.h"
#include "background.h"

namespace wfits {
namespace test {
namespace efl {

static const boost::filesystem::path img(MEDIA_PATH"/bridge_of_the_gods.png");
static const boost::filesystem::path gif(MEDIA_PATH"/ADN_animation.gif");

class Image : public EvasObject
{
public:
	Image(EvasObject &parent)
		: EvasObject::EvasObject(elm_image_add(parent))
	{
		return;
	}
};

class ImageFileTest : public ElmTestHarness
{
public:
	ImageFileTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ImageFileTest", "Image File Test")
		, control_(window_)
	{
		return;
	}

	void setup()
	{
		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		TEST_LOG("setting image = " << img);
		FAIL_UNLESS_EQUAL(
			elm_image_file_set(control_, img.c_str(), NULL),
			EINA_TRUE
		);

		window_.show();
		control_.show();
	}

	void test()
	{
		TEST_LOG("checking image == " << img);
		const char* actual_path;
		synchronized(
			[this, &actual_path]() {
				elm_image_file_get(control_, &actual_path, NULL);
			}
		);
		boost::filesystem::path actual(actual_path);

		TEST_LOG("got image == " << actual);
		FAIL_UNLESS_EQUAL(actual, img);
	}

private:
	Window	window_;
	Image	control_;
};

static std::string toString(Elm_Image_Orient orient)
{
	switch(orient)
	{
		case ELM_IMAGE_ORIENT_0:
			return "orient 0";
		case ELM_IMAGE_ROTATE_90:
			return "rotate 90";
		case ELM_IMAGE_ROTATE_180:
			return "rotate 180";
		case ELM_IMAGE_ROTATE_270:
			return "rotate 270";
		case ELM_IMAGE_FLIP_HORIZONTAL:
			return "flip horizontal";
		case ELM_IMAGE_FLIP_VERTICAL:
			return "flip vertical";
		case ELM_IMAGE_FLIP_TRANSPOSE:
			return "flip transpose";
		case ELM_IMAGE_FLIP_TRANSVERSE:
			return "flip transverse";
		default:
			return "UNKNOWN";
	}
}

class ImageOrientTest : public ElmTestHarness
{
public:
	ImageOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ImageOrientTest", "Image Orientation Test")
		, control_(window_)
	{
		return;
	}

	void setup()
	{
		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		TEST_LOG("setting image = " << img);
		FAIL_UNLESS_EQUAL(
			elm_image_file_set(control_, img.c_str(), NULL),
			EINA_TRUE
		);

		orientations_.push_back(ELM_IMAGE_ORIENT_0);
		orientations_.push_back(ELM_IMAGE_ROTATE_90);
		orientations_.push_back(ELM_IMAGE_ROTATE_180);
		orientations_.push_back(ELM_IMAGE_ROTATE_270);
		orientations_.push_back(ELM_IMAGE_FLIP_HORIZONTAL);
		orientations_.push_back(ELM_IMAGE_FLIP_VERTICAL);
		orientations_.push_back(ELM_IMAGE_FLIP_TRANSPOSE);
		orientations_.push_back(ELM_IMAGE_FLIP_TRANSVERSE);

		window_.show();
		control_.show();
	}

	void test()
	{
		foreach (const Elm_Image_Orient orientation, orientations_) {
			TEST_LOG("setting image orientation = " << toString(orientation));
			synchronized(boost::bind(elm_image_orient_set, boost::ref(control_), orientation));

			TEST_LOG("checking image orientation attribute == " << toString(orientation));
			FAIL_UNLESS_EQUAL(
				Application::synchronizedResult(
					[this]()->Elm_Image_Orient {
						return elm_image_orient_get(control_);
					}
				), orientation
			);
		}
	}

private:
	Window	window_;
	Image	control_;
	std::vector<Elm_Image_Orient> orientations_;
};

class ImageAnimateTest : public ElmTestHarness
{
public:
	ImageAnimateTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ImageAnimateTest", "Image Animate Gif Test")
		, control_(window_)
	{
		return;
	}

	void setup()
	{
		control_.setSize(181, 313);
		control_.setPosition(50, 10);

		FAIL_UNLESS_EQUAL(
			elm_image_file_set(control_, gif.c_str(), NULL),
			EINA_TRUE
		);

		window_.show();
		control_.show();

		FAIL_UNLESS_EQUAL(
			elm_image_animated_available_get(control_),
			EINA_TRUE
		);
	}

	void test()
	{
		synchronized(boost::bind(elm_image_animated_set, boost::ref(control_), EINA_TRUE));
		checkAnimated(EINA_TRUE);

		synchronized(boost::bind(elm_image_animated_play_set, boost::ref(control_), EINA_TRUE));
		checkPlay(EINA_TRUE);

		yield(0.25*1e6); // allow the animation to play for a while

		synchronized(boost::bind(elm_image_animated_play_set, boost::ref(control_), EINA_FALSE));
		checkPlay(EINA_FALSE);

		synchronized(boost::bind(elm_image_animated_set, boost::ref(control_), EINA_FALSE));
		checkAnimated(EINA_FALSE);
	}

	void checkAnimated(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this, &expected]()->Eina_Bool {
					return elm_image_animated_get(control_);
				}
			), expected
		);
	}

	void checkPlay(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(
			Application::synchronizedResult(
				[this, &expected]()->Eina_Bool {
					return elm_image_animated_play_get(control_);
				}
			), expected
		);
	}

private:
	Window		window_;
	Image		control_;
	int		width;
	int		height;
	Eina_Bool	size_up;
	Eina_Bool	size_down;
};


typedef ResizeObjectTest<Image> ImageResizeTest;
typedef PositionObjectTest<Image> ImagePositionTest;
typedef VisibleObjectTest<Image> ImageVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(ImageResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(ImagePositionTest)
WFITS_EFL_HARNESS_TEST_CASE(ImageVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(ImageAnimateTest)
WFITS_EFL_HARNESS_TEST_CASE(ImageFileTest)
WFITS_EFL_HARNESS_TEST_CASE(ImageOrientTest)

} // namespace efl
} // namespace test
} // namespace wfits
