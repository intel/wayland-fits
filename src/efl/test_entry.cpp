#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>
#include <string>

#include "window.h"
#include "actionslider.h"
#include "evasobject.h"
#include "elmtestharness.h"

using std::string;
using std::vector;

class EntryEmoticonTest : public ElmTestHarness
{
public:

	EntryEmoticonTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryEmoticonTest", "Entry Emoticon Test")
		, control_(elm_entry_add(window_))
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

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		vector<string>::iterator it;
		vector<int>::iterator sizeit;
		for (it = emoticons_.begin(); it != emoticons_.end(); it++)
			for (sizeit = sizes_.begin(); sizeit != sizes_.end(); sizeit++)
		{
			char buf[1024] = { '\0' };
			snprintf(buf, 1024, "Emoticon: <item size=%dx%d vsize=full href=emoticon/%s></item>",
				*sizeit, *sizeit, it->c_str()); 
			string markup = buf;

			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_entry_entry_set, boost::ref(control_), markup.c_str()),
					boost::bind(&EntryEmoticonTest::checkEmoticon, boost::ref(*this), markup)
				)
			);
		}
	}

	void checkEmoticon(string& expected)
	{
		const char* actual_ = elm_entry_entry_get(control_);
		string actual = actual_ != NULL ? actual_ : "";

		BOOST_CHECK_EQUAL(actual, expected);
	}

private:
	Window		window_;
	EvasObject	control_;
	vector<string>	emoticons_;
	vector<int>	sizes_;
};

class EntryCutTest : public ElmTestHarness
{
public:

	EntryCutTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryCutTest", "Entry Cut Test")
		, control_(elm_entry_add(window_))
		, sentinel_("Sentinel Text")
	{
		control_.setSize(50, 100);
		control_.setPosition(50, 10);

		elm_entry_autosave_set(control_, EINA_FALSE);
		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		elm_entry_select_all(control_);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_entry_entry_set, boost::ref(control_), sentinel_.c_str()),
				boost::bind(&EntryCutTest::checkEntry, boost::ref(*this), sentinel_)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_entry_selection_cut, boost::ref(control_)),
				boost::bind(&EntryCutTest::checkCut, boost::ref(*this), "")
			)
		);

	}

	void checkEntry(const string& expected)
	{
		const char* actual_raw = elm_entry_entry_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		BOOST_CHECK_EQUAL(actual, expected);
	}

	void checkCut(const char* exp)
	{
		string expected(exp != NULL ? exp : "");

		const char* actual_raw = elm_entry_selection_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		BOOST_CHECK_EQUAL(actual, expected);
	}

private:
	Window		window_;
	EvasObject	control_;
	string		sentinel_;
};

class EntryCopyTest : public ElmTestHarness
{
public:

	EntryCopyTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryCopyTest", "Entry Copy Test")
		, control_(elm_entry_add(window_))
		, sentinel_("Sentinel Text")
	{
		control_.setSize(50, 100);
		control_.setPosition(50, 10);

		elm_entry_autosave_set(control_, EINA_FALSE);
		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		elm_entry_select_all(control_);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_entry_entry_set, boost::ref(control_), sentinel_.c_str()),
				boost::bind(&EntryCopyTest::checkEntry, boost::ref(*this), sentinel_)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_entry_selection_copy, boost::ref(control_)),
				boost::bind(&EntryCopyTest::checkEntry, boost::ref(*this), sentinel_)
			)
		);

	}

	void checkEntry(const string& expected)
	{
		const char* actual_raw = elm_entry_entry_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		BOOST_CHECK_EQUAL(actual, expected);
	}

private:
	Window		window_;
	EvasObject	control_;
	string		sentinel_;
};

class EntryPasteTest : public ElmTestHarness
{
public:

	EntryPasteTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("EntryPasteTest", "Entry Paste Test")
		, control_(elm_entry_add(window_))
		, sentinel_("Sentinel Text")
		, jibberish_("jibberish")
	{
		control_.setSize(50, 100);
		control_.setPosition(50, 10);

		elm_entry_autosave_set(control_, EINA_FALSE);
		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_entry_entry_set, boost::ref(control_), sentinel_.c_str()),
				boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), sentinel_)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&EntryPasteTest::cut, boost::ref(*this)),
				boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), "")
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_entry_entry_set, boost::ref(control_), jibberish_.c_str()),
				boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), jibberish_.c_str())
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(&EntryPasteTest::paste, boost::ref(*this)),
				boost::bind(&EntryPasteTest::checkEntry, boost::ref(*this), jibberish_.c_str())
			)
		);

	}

	void cut(void)
	{
		elm_entry_select_all(control_);
		elm_entry_selection_cut(control_);
	}

	void paste(void)
	{
		elm_entry_select_all(control_);
		elm_entry_selection_paste(control_);
	}

	void checkEntry(const string& expected)
	{
		const char* actual_raw = elm_entry_entry_get(control_);
		string actual(actual_raw != NULL ? actual_raw : "");

		BOOST_CHECK_EQUAL(actual, expected);
	}

private:
	Window		window_;
	EvasObject	control_;
	string		sentinel_;
	string		jibberish_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(Entry)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(EntryEmoticonTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(EntryCutTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(EntryCopyTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(EntryPasteTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
