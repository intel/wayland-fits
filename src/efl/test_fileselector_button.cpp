#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"

#include "elmtestharness.h"
#include "templates.h"

using std::vector;

class FileselectorButton : public EvasObject
{
public:
	FileselectorButton(EvasObject &parent)
		: EvasObject::EvasObject(elm_fileselector_button_add(parent))
	{
		return;
	}
};

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
	}

	void setup()
	{
		window_.show();
		control_.show();

		typedef vector<Eina_Bool>::const_iterator CIterator;
		const CIterator endIt(states_.end());
		for (CIterator it(states_.begin()); it != endIt; ++it) {
			queueStep(boost::bind(elm_fileselector_button_expandable_set, boost::ref(control_), *it));
			queueStep(boost::bind(&FileselectorButtonExpandableTest::checkExpandable, boost::ref(*this), *it));
		}
	}

	void checkExpandable(const Eina_Bool expandable)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_button_expandable_get(control_), expandable);
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
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Eina_Bool state, states_) {
			queueStep(boost::bind(elm_fileselector_button_folder_only_set, boost::ref(control_), state));
			queueStep(boost::bind(&FileselectorButtonFolderOnlyTest::checkFolderOnly, boost::ref(*this), state));
		}
	}

	void checkFolderOnly(const Eina_Bool folderonly)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_button_folder_only_get(control_), folderonly);
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
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Eina_Bool state, states_) {
			queueStep(boost::bind(elm_fileselector_button_is_save_set, boost::ref(control_), state));
			queueStep(boost::bind(&FileselectorButtonIsSaveTest::checkIsSave, boost::ref(*this), state));
		}
	}

	void checkIsSave(const Eina_Bool save)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_button_is_save_get(control_), save);
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
	}

	void setup()
	{
		control_.setSize(100, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();

		foreach (Eina_Bool state, states_) {
			queueStep(boost::bind(elm_fileselector_button_inwin_mode_set, boost::ref(control_), state));
			queueStep(boost::bind(&FileselectorButtonInwinTest::checkInwin, boost::ref(*this), state));
		}
	}

	void checkInwin(const Eina_Bool inwin)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_button_inwin_mode_get(control_), inwin);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

typedef ResizeObjectTest<FileselectorButton> FileselectorButtonResizeTest;
typedef PositionObjectTest<FileselectorButton> FileselectorButtonPositionTest;
typedef VisibleObjectTest<FileselectorButton> FileselectorButtonVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonResizeTest, "FileSelectorButton")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonPositionTest, "FileSelectorButton")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonVisibilityTest, "FileSelectorButton")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonExpandableTest, "FileSelectorButton")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonFolderOnlyTest, "FileSelectorButton")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonIsSaveTest, "FileSelectorButton")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorButtonInwinTest, "FileSelectorButton")

