#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"
#include "evasobject.h"
#include "elmtestharness.h"
#include "templates.h"

using std::vector;

// Basic suite START
EFL_RESIZE_TEST(FileselectorEntry, fileselector_entry, 300, 200, 200, 100)
EFL_POSITION_TEST(FileselectorEntry, fileselector_entry, 100, 50, 150, 100)
EFL_VISIBILITY_TEST(FileselectorEntry, fileselector_entry)
// Basic suite END

// TODO: FileselectorEntry and FileselectorButton share a lot of commonality
// TODO: so an aggressive refactor with cpp macros could be leveraged.

class FileselectorEntryExpandableTest : public ElmTestHarness
{
public:

	FileselectorEntryExpandableTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorEntryExpandableTest", "Fileselector Entry Expandable Test")
		, control_(elm_fileselector_entry_add(window_))
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
					boost::bind(elm_fileselector_entry_expandable_set, boost::ref(control_), *it),
					boost::bind(&FileselectorEntryExpandableTest::checkExpandable, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkExpandable(const Eina_Bool expandable)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_entry_expandable_get(control_), expandable);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorEntryFolderOnlyTest : public ElmTestHarness
{
public:

	FileselectorEntryFolderOnlyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorEntryFolderOnlyTest", "Fileselector Entry FolderOnly Test")
		, control_(elm_fileselector_entry_add(window_))
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
					boost::bind(elm_fileselector_entry_folder_only_set, boost::ref(control_), *it),
					boost::bind(&FileselectorEntryFolderOnlyTest::checkFolderOnly, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkFolderOnly(const Eina_Bool folderonly)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_entry_folder_only_get(control_), folderonly);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorEntryIsSaveTest : public ElmTestHarness
{
public:

	FileselectorEntryIsSaveTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorEntryIsSaveTest", "Fileselector Entry IsSave Test")
		, control_(elm_fileselector_entry_add(window_))
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
					boost::bind(elm_fileselector_entry_is_save_set, boost::ref(control_), *it),
					boost::bind(&FileselectorEntryIsSaveTest::checkIsSave, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkIsSave(const Eina_Bool save)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_entry_is_save_get(control_), save);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorEntryInwinTest : public ElmTestHarness
{
public:

	FileselectorEntryInwinTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorEntryInwinTest", "Fileselector Entry Inwin Test")
		, control_(elm_fileselector_entry_add(window_))
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
					boost::bind(elm_fileselector_entry_inwin_mode_set, boost::ref(control_), *it),
					boost::bind(&FileselectorEntryInwinTest::checkInwin, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkInwin(const Eina_Bool inwin)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_entry_inwin_mode_get(control_), inwin);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(FileSelectorEntry)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryExpandableTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryFolderOnlyTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryIsSaveTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryInwinTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
