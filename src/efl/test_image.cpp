#include <boost/filesystem.hpp>

#include <vector>

#include "templates.h"
#include "background.h"

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
		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&ImageFileTest::checkImageSet, boost::ref(*this), img),
				boost::bind(&ImageFileTest::checkImage, boost::ref(*this), img)
			)
		);
	}

	void checkImageSet(boost::filesystem::path& desired)
	{
		FAIL_UNLESS_EQUAL(elm_image_file_set(control_, desired.c_str(), NULL), EINA_TRUE);
	}

	void checkImage(boost::filesystem::path& expected)
	{
		const char* actual_path;
		elm_image_file_get(control_, &actual_path, NULL);

		boost::filesystem::path actual(actual_path);

		FAIL_UNLESS_EQUAL(actual, expected);
	}

private:
	Window	window_;
	Image	control_;
};

class ImageOrientTest : public ElmTestHarness
{
public:
	ImageOrientTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ImageOrientTest", "Image Orientation Test")
		, control_(window_)
	{

		FAIL_UNLESS_EQUAL(elm_image_file_set(control_, img.c_str(), NULL), EINA_TRUE);

		control_.setSize(300, 300);
		control_.setPosition(50, 10);

		orientations_.push_back(ELM_IMAGE_ORIENT_0);
		orientations_.push_back(ELM_IMAGE_ROTATE_90);
		orientations_.push_back(ELM_IMAGE_ROTATE_180);
		orientations_.push_back(ELM_IMAGE_ROTATE_270);
		orientations_.push_back(ELM_IMAGE_FLIP_HORIZONTAL);
		orientations_.push_back(ELM_IMAGE_FLIP_VERTICAL);
		orientations_.push_back(ELM_IMAGE_FLIP_TRANSPOSE);
		orientations_.push_back(ELM_IMAGE_FLIP_TRANSVERSE);

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (const Elm_Image_Orient orientation, orientations_)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_image_orient_set, boost::ref(control_), orientation),
					boost::bind(&ImageOrientTest::checkOrient, boost::ref(*this), orientation)
				)
			);

			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_image_orient_set, boost::ref(control_), ELM_IMAGE_ORIENT_0),
					boost::bind(&ImageOrientTest::checkOrient, boost::ref(*this), ELM_IMAGE_ORIENT_0)
				)
			);
		}

	}

	void checkOrient(const Elm_Image_Orient expected)
	{
		FAIL_UNLESS_EQUAL(elm_image_orient_get(control_), expected);
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
		control_.setSize(181, 313);
		control_.setPosition(50, 10);

		FAIL_UNLESS_EQUAL(elm_image_file_set(control_, gif.c_str(), NULL), EINA_TRUE);
	}

	void setup()
	{
		window_.show();
		control_.show();

		FAIL_UNLESS(elm_image_animated_available_get(control_) == EINA_TRUE);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_image_animated_set, boost::ref(control_), EINA_TRUE),
				boost::bind(&ImageAnimateTest::checkAnimated, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_image_animated_play_set, boost::ref(control_), EINA_TRUE),
				boost::bind(&ImageAnimateTest::checkPlay, boost::ref(*this), EINA_TRUE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&ImageAnimateTest::pause, boost::ref(*this)),
				boost::bind(&ImageAnimateTest::pause, boost::ref(*this))
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_image_animated_play_set, boost::ref(control_), EINA_FALSE),
				boost::bind(&ImageAnimateTest::checkPlay, boost::ref(*this), EINA_FALSE)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_image_animated_set, boost::ref(control_), EINA_FALSE),
				boost::bind(&ImageAnimateTest::checkAnimated, boost::ref(*this), EINA_FALSE)
			)
		);


	}

	void pause(void)
	{
		int loop;

		// total is a half-second delay, but keep the main loop moving
		for (loop = 0; loop < 5000; ++loop)
		{
			Application::yield(100);
		}
	}

	void checkAnimated(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_image_animated_get(control_), expected);
	}

	void checkPlay(const Eina_Bool expected)
	{
		FAIL_UNLESS_EQUAL(elm_image_animated_play_get(control_), expected);
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

WAYLAND_ELM_HARNESS_TEST_CASE(ImageResizeTest, "Image")
WAYLAND_ELM_HARNESS_TEST_CASE(ImagePositionTest, "Image")
WAYLAND_ELM_HARNESS_TEST_CASE(ImageVisibilityTest, "Image")
WAYLAND_ELM_HARNESS_TEST_CASE(ImageAnimateTest, "Image")
WAYLAND_ELM_HARNESS_TEST_CASE(ImageFileTest, "Image")
WAYLAND_ELM_HARNESS_TEST_CASE(ImageOrientTest, "Image")

