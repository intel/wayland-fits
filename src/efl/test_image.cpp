#include <boost/filesystem.hpp>

#include <vector>

#include "templates.h"
#include "background.h"

static const boost::filesystem::path img(MEDIA_PATH"/bridge_of_the_gods.png");

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
		BOOST_CHECK_EQUAL(elm_image_file_set(control_, desired.c_str(), NULL), EINA_TRUE);
	}

	void checkImage(boost::filesystem::path& expected)
	{
		const char* actual_path;
		elm_image_file_get(control_, &actual_path, NULL);

		boost::filesystem::path actual(actual_path);

		BOOST_CHECK_EQUAL(actual, expected);
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

		BOOST_CHECK_EQUAL(elm_image_file_set(control_, img.c_str(), NULL), EINA_TRUE);

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
		BOOST_CHECK_EQUAL(elm_image_orient_get(control_), expected);
	}

private:
	Window	window_;
	Image	control_;
	std::vector<Elm_Image_Orient> orientations_;
};


typedef ResizeObjectTest<Image> ImageResizeTest;
typedef PositionObjectTest<Image> ImagePositionTest;
typedef VisibleObjectTest<Image> ImageVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Image)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(ImageResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ImagePositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ImageVisibilityTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ImageFileTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(ImageOrientTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

