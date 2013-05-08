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

#include <boost/format.hpp>

#include <vector>
#include <string>

#include "templates.h"

namespace wfits {
namespace test {
namespace efl {

using std::string;
using std::vector;

class Entry : public EvasObject
{
public:
	Entry(EvasObject &parent)
		: EvasObject::EvasObject(elm_entry_add(parent))
	{
		return;
	}
};

class EntryEmoticonTest : public ElmTestHarness
{
public:

	EntryEmoticonTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryEmoticonTest", "Entry Emoticon Test")
		, control_(window_)
	{
		control_.setSize(200, 100);
		control_.setPosition(50, 10);
		elm_entry_autosave_set(control_, EINA_FALSE);

		emoticons_.push_back("angry");
		emoticons_.push_back("angry-shout");
		emoticons_.push_back("crazy-laugh");
		emoticons_.push_back("evil-laugh");
		emoticons_.push_back("evil");
		emoticons_.push_back("goggle-smile");
		emoticons_.push_back("grumpy");
		emoticons_.push_back("grumpy-smile");
		emoticons_.push_back("guilty");
		emoticons_.push_back("guilty-smile");
		emoticons_.push_back("haha");
		emoticons_.push_back("half-smile");
		emoticons_.push_back("happy-panting");
		emoticons_.push_back("happy");
		emoticons_.push_back("indifferent");
		emoticons_.push_back("kiss");
		emoticons_.push_back("knowing-grin");
		emoticons_.push_back("laugh");
		emoticons_.push_back("little-bit-sorry");
		emoticons_.push_back("love-lots");
		emoticons_.push_back("love");
		emoticons_.push_back("minimal-smile");
		emoticons_.push_back("not-happy");
		emoticons_.push_back("not-impressed");
		emoticons_.push_back("omg");
		emoticons_.push_back("opensmile");
		emoticons_.push_back("smile");
		emoticons_.push_back("sorry");
		emoticons_.push_back("squint-laugh");
		emoticons_.push_back("surprised");
		emoticons_.push_back("suspicious");
		emoticons_.push_back("tongue-dangling");
		emoticons_.push_back("tongue-poke");
		emoticons_.push_back("uh");
		emoticons_.push_back("unhappy");
		emoticons_.push_back("very-sorry");
		emoticons_.push_back("what");
		emoticons_.push_back("wink");
		emoticons_.push_back("worried");
		emoticons_.push_back("wtf");

		sizes_.push_back(16);
		sizes_.push_back(24);
		sizes_.push_back(48);
		sizes_.push_back(64);
		sizes_.push_back(128);
	}

	void setup()
	{
		window_.show();
		control_.show();

		boost::format tmarkup("Emoticon: <item size=%1%x%1% vsize=full href=emoticon/%2%></item>");
		foreach (const string& e, emoticons_) {
			foreach (int size, sizes_) {
				std::string markup(boost::str(tmarkup % size % e));

				queueStep(boost::bind(elm_entry_entry_set, boost::ref(control_), markup.c_str()));
				queueStep(boost::bind(&EntryEmoticonTest::checkEmoticon, boost::ref(*this), markup));
			}
		}
	}

	void checkEmoticon(const string& expected)
	{
		const char* actual_ = elm_entry_entry_get(control_);
		string actual = actual_ != NULL ? actual_ : "";

		FAIL_UNLESS_EQUAL(actual, expected);
	}

private:
	Window		window_;
	Entry		control_;
	vector<string>	emoticons_;
	vector<int>	sizes_;
};

class EntryCutTest : public ElmTestHarness
{
public:

	EntryCutTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryCutTest", "Entry Cut Test")
		, control_(window_)
		, sentinel_("Sentinel Text")
	{
		control_.setSize(50, 100);
		control_.setPosition(50, 10);

		elm_entry_autosave_set(control_, EINA_FALSE);
	}

	void setup()
	{
		window_.show();
		control_.show();

		elm_entry_select_all(control_);

		queueStep(boost::bind(elm_entry_entry_set, boost::ref(control_), sentinel_.c_str()));
		queueStep(boost::bind(&EntryCutTest::checkEntry, boost::ref(*this), sentinel_));

		queueStep(boost::bind(elm_entry_selection_cut, boost::ref(control_)));
		queueStep(boost::bind(&EntryCutTest::checkCut, boost::ref(*this), ""));
	}

	void checkEntry(const string& expected)
	{
		const char* actual_raw = elm_entry_entry_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		FAIL_UNLESS_EQUAL(actual, expected);
	}

	void checkCut(const char* exp)
	{
		string expected(exp != NULL ? exp : "");

		const char* actual_raw = elm_entry_selection_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		FAIL_UNLESS_EQUAL(actual, expected);
	}

private:
	Window	window_;
	Entry	control_;
	string	sentinel_;
};

class EntryCopyTest : public ElmTestHarness
{
public:

	EntryCopyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryCopyTest", "Entry Copy Test")
		, control_(window_)
		, sentinel_("Sentinel Text")
	{
		control_.setSize(50, 100);
		control_.setPosition(50, 10);

		elm_entry_autosave_set(control_, EINA_FALSE);
	}

	void setup()
	{
		window_.show();
		control_.show();

		elm_entry_select_all(control_);

		queueStep(boost::bind(elm_entry_entry_set, boost::ref(control_), sentinel_.c_str()));
		queueStep(boost::bind(&EntryCopyTest::checkEntry, boost::ref(*this), sentinel_));

		queueStep(boost::bind(elm_entry_selection_copy, boost::ref(control_)));
		queueStep(boost::bind(&EntryCopyTest::checkEntry, boost::ref(*this), sentinel_));
	}

	void checkEntry(const string& expected)
	{
		const char* actual_raw = elm_entry_entry_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		FAIL_UNLESS_EQUAL(actual, expected);
	}

private:
	Window	window_;
	Entry	control_;
	string	sentinel_;
};

class EntryPasteTest : public ElmTestHarness
{
public:

	EntryPasteTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryPasteTest", "Entry Paste Test")
		, control_(window_)
		, sentinel_("Sentinel Text")
		, jibberish_("jibberish")
	{
		control_.setSize(50, 100);
		control_.setPosition(50, 10);

		elm_entry_autosave_set(control_, EINA_FALSE);
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueStep(boost::bind(elm_entry_entry_set, boost::ref(control_), sentinel_.c_str()));
		queueStep(boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), sentinel_));

		queueStep(boost::bind(&EntryPasteTest::cut, boost::ref(*this)));
		queueStep(boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), ""));

		queueStep(boost::bind(elm_entry_entry_set, boost::ref(control_), jibberish_.c_str()));
		queueStep(boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), jibberish_.c_str()));

		queueStep(boost::bind(&EntryPasteTest::paste, boost::ref(*this)));
		queueStep(boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), jibberish_.c_str()));
	}

	void cut()
	{
		elm_entry_select_all(control_);
		elm_entry_selection_cut(control_);
	}

	void paste()
	{
		elm_entry_select_all(control_);
		elm_entry_selection_paste(control_);
	}

	void checkEntry(const string& expected)
	{
		const char* actual_raw = elm_entry_entry_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		FAIL_UNLESS_EQUAL(actual, expected);
	}

private:
	Window	window_;
	Entry	control_;
	string	sentinel_;
	string	jibberish_;
};

typedef ResizeObjectTest<Entry> EntryResizeTest;
typedef PositionObjectTest<Entry> EntryPositionTest;
typedef VisibleObjectTest<Entry> EntryVisibilityTest;

WFITS_EFL_HARNESS_TEST_CASE(EntryResizeTest)
WFITS_EFL_HARNESS_TEST_CASE(EntryPositionTest)
WFITS_EFL_HARNESS_TEST_CASE(EntryVisibilityTest)
WFITS_EFL_HARNESS_TEST_CASE(EntryEmoticonTest)
WFITS_EFL_HARNESS_TEST_CASE(EntryCutTest)
WFITS_EFL_HARNESS_TEST_CASE(EntryCopyTest)
WFITS_EFL_HARNESS_TEST_CASE(EntryPasteTest)

} // namespace efl
} // namespace test
} // namespace wfits
