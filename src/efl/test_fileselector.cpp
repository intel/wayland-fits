#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"
#include "actionslider.h"
#include "evasobject.h"
#include "elmtestharness.h"

using std::vector;

// TODO: Fileselector controls all share a lot of commonality, so an
// TODO: aggressive refactor with cpp macros could be leveraged.

class FileselectorExpandableTest : public ElmTestHarness
{
public:

	FileselectorExpandableTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorExpandableTest", "Fileselector Expandable Test")
		, control_(elm_fileselector_add(window_))
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
					boost::bind(elm_fileselector_expandable_set, boost::ref(control_), *it),
					boost::bind(&FileselectorExpandableTest::checkExpandable, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkExpandable(const Eina_Bool expandable)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_expandable_get(control_), expandable);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorFolderOnlyTest : public ElmTestHarness
{
public:

	FileselectorFolderOnlyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorFolderOnlyTest", "Fileselector FolderOnly Test")
		, control_(elm_fileselector_add(window_))
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
					boost::bind(elm_fileselector_folder_only_set, boost::ref(control_), *it),
					boost::bind(&FileselectorFolderOnlyTest::checkFolderOnly, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkFolderOnly(const Eina_Bool folderonly)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_folder_only_get(control_), folderonly);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorIsSaveTest : public ElmTestHarness
{
public:

	FileselectorIsSaveTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorIsSaveTest", "Fileselector IsSave Test")
		, control_(elm_fileselector_add(window_))
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
					boost::bind(elm_fileselector_is_save_set, boost::ref(control_), *it),
					boost::bind(&FileselectorIsSaveTest::checkIsSave, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkIsSave(const Eina_Bool save)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_is_save_get(control_), save);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

class FileselectorModeTest : public ElmTestHarness
{
public:

	FileselectorModeTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorModeTest", "Fileselector Mode Test")
		, control_(elm_fileselector_add(window_))
	{
		states_.push_back(ELM_FILESELECTOR_LIST);
		states_.push_back(ELM_FILESELECTOR_GRID);
		states_.push_back(ELM_FILESELECTOR_LIST);
		states_.push_back(ELM_FILESELECTOR_GRID);

		return;
	}

	void setup()
	{
		control_.setSize(100, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();

		vector<Elm_Fileselector_Mode>::iterator it;
		for (it = states_.begin(); it != states_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_fileselector_mode_set, boost::ref(control_), *it),
					boost::bind(&FileselectorModeTest::checkMode, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkMode(const Eina_Bool mode)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_mode_get(control_), mode);
	}

private:
	Window				window_;
	EvasObject			control_;
	vector<Elm_Fileselector_Mode>	states_;
};

class FileselectorOkCancelTest : public ElmTestHarness
{
public:

	FileselectorOkCancelTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FileselectorOkCancelTest", "Fileselector OkCancel Test")
		, control_(elm_fileselector_add(window_))
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
					boost::bind(elm_fileselector_buttons_ok_cancel_set, boost::ref(control_), *it),
					boost::bind(&FileselectorOkCancelTest::checkOkCancel, boost::ref(*this), *it)
				)
			);
		}
	}

	void checkOkCancel(const Eina_Bool okcancel)
	{
		BOOST_CHECK_EQUAL(elm_fileselector_buttons_ok_cancel_get(control_), okcancel);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(FileSelector)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorExpandableTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorFolderOnlyTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorIsSaveTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorModeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorOkCancelTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
