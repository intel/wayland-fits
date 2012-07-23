#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"

#include "elmtestharness.h"
#include "macros.h"

using std::vector;

// Basic suite START
EFL_RESIZE_TEST(FileselectorButton, fileselector_button, 300, 200, 200, 100)
EFL_POSITION_TEST(FileselectorButton, fileselector_button, 100, 50, 150, 100)
EFL_VISIBILITY_TEST(FileselectorButton, fileselector_button)
// Basic suite END

class FileselectorButtonExpandableTest : public ElmTestHarness
{
public:

	FileselectorButtonExpandableTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorButtonExpandableTest", "Fileselector Button Expandable Test")
		, control_(elm_fileselector_button_add(window_))
	{
		control_.setSize(100, 100);
		control_.setPosition(50, 10);

		states_.push_back(EINA_TRUE);
		states_.push_back(EINA_FALSE);
		states_.push_back(EINA_TRUE);

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		vector<Eina_Bool>::iterator it;
		for (it = states_.begin(); it != states_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_fileselector_button_expandable_set, boost::ref(control_), *it),
					boost::bind(&FileselectorButtonExpandableTest::checkExpandable, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkExpandable(const Eina_Bool expandable)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_button_expandable_get(control_), expandable);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorButtonFolderOnlyTest : public ElmTestHarness
{
public:

	FileselectorButtonFolderOnlyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorButtonFolderOnlyTest", "Fileselector Button FolderOnly Test")
		, control_(elm_fileselector_button_add(window_))
	{
		control_.setSize(100, 100);
		control_.setPosition(50, 10);

		states_.push_back(EINA_TRUE);
		states_.push_back(EINA_FALSE);
		states_.push_back(EINA_TRUE);

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		vector<Eina_Bool>::iterator it;
		for (it = states_.begin(); it != states_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_fileselector_button_folder_only_set, boost::ref(control_), *it),
					boost::bind(&FileselectorButtonFolderOnlyTest::checkFolderOnly, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkFolderOnly(const Eina_Bool folderonly)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_button_folder_only_get(control_), folderonly);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorButtonIsSaveTest : public ElmTestHarness
{
public:

	FileselectorButtonIsSaveTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorButtonIsSaveTest", "Fileselector Button IsSave Test")
		, control_(elm_fileselector_button_add(window_))
	{
		control_.setSize(100, 100);
		control_.setPosition(50, 10);

		states_.push_back(EINA_TRUE);
		states_.push_back(EINA_FALSE);
		states_.push_back(EINA_TRUE);

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		vector<Eina_Bool>::iterator it;
		for (it = states_.begin(); it != states_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_fileselector_button_is_save_set, boost::ref(control_), *it),
					boost::bind(&FileselectorButtonIsSaveTest::checkIsSave, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkIsSave(const Eina_Bool save)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_button_is_save_get(control_), save);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorButtonInwinTest : public ElmTestHarness
{
public:

	FileselectorButtonInwinTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorButtonInwinTest", "Fileselector Button Inwin Test")
		, control_(elm_fileselector_button_add(window_))
	{
		states_.push_back(EINA_TRUE);
		states_.push_back(EINA_FALSE);
		states_.push_back(EINA_TRUE);

		return;
	}

	void setup()
	{
		control_.setSize(100, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();

		vector<Eina_Bool>::iterator it;
		for (it = states_.begin(); it != states_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_fileselector_button_inwin_mode_set, boost::ref(control_), *it),
					boost::bind(&FileselectorButtonInwinTest::checkInwin, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkInwin(const Eina_Bool inwin)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_button_inwin_mode_get(control_), inwin);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(FileSelectorButton)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonExpandableTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonFolderOnlyTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonIsSaveTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonInwinTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
