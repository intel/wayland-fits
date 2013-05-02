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

#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"

#include "elmtestharness.h"
#include "templates.h"

using std::vector;

// TODO: FileselectorEntry and FileselectorButton share a lot of commonality
// TODO: so an aggressive refactor with cpp templates could be leveraged.

class FileselectorEntry : public EvasObject
{
public:
	FileselectorEntry(EvasObject &parent)
		: EvasObject::EvasObject(elm_fileselector_entry_add(parent))
	{
		return;
	}
};

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
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Eina_Bool state, states_) {
			queueStep(boost::bind(elm_fileselector_entry_expandable_set, boost::ref(control_), state));
			queueStep(boost::bind(&FileselectorEntryExpandableTest::checkExpandable, boost::ref(*this), state));
		}
	}

	void checkExpandable(const Eina_Bool expandable)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_entry_expandable_get(control_), expandable);
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
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Eina_Bool state, states_) {
			queueStep(boost::bind(elm_fileselector_entry_folder_only_set, boost::ref(control_), state));
			queueStep(boost::bind(&FileselectorEntryFolderOnlyTest::checkFolderOnly, boost::ref(*this), state));
		}
	}

	void checkFolderOnly(const Eina_Bool folderonly)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_entry_folder_only_get(control_), folderonly);
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
	}

	void setup()
	{
		window_.show();
		control_.show();

		foreach (Eina_Bool state, states_) {
			queueStep(boost::bind(elm_fileselector_entry_is_save_set, boost::ref(control_), state));
			queueStep(boost::bind(&FileselectorEntryIsSaveTest::checkIsSave, boost::ref(*this), state));
		}
	}

	void checkIsSave(const Eina_Bool save)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_entry_is_save_get(control_), save);
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
	}

	void setup()
	{
		control_.setSize(100, 100);
		control_.setPosition(50, 10);

		window_.show();
		control_.show();

		foreach (Eina_Bool state, states_) {
			queueStep(boost::bind(elm_fileselector_entry_inwin_mode_set, boost::ref(control_), state));
			queueStep(boost::bind(&FileselectorEntryInwinTest::checkInwin, boost::ref(*this), state));
		}
	}

	void checkInwin(const Eina_Bool inwin)
	{
		FAIL_UNLESS_EQUAL(elm_fileselector_entry_inwin_mode_get(control_), inwin);
	}

private:
	Window			window_;
	EvasObject		control_;
	vector<Eina_Bool>	states_;
};

typedef ResizeObjectTest<FileselectorEntry> FileselectorEntryResizeTest;
typedef PositionObjectTest<FileselectorEntry> FileselectorEntryPositionTest;
typedef VisibleObjectTest<FileselectorEntry> FileselectorEntryVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryResizeTest, "FileSelectorEntry")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryPositionTest, "FileSelectorEntry")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryVisibilityTest, "FileSelectorEntry")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryExpandableTest, "FileSelectorEntry")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryFolderOnlyTest, "FileSelectorEntry")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryIsSaveTest, "FileSelectorEntry")
WAYLAND_ELM_HARNESS_TEST_CASE(FileselectorEntryInwinTest, "FileSelectorEntry")

