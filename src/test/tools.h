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

#ifndef __WFITS_TEST_TOOLS_H__
#define __WFITS_TEST_TOOLS_H__

#include <iostream>
#include <map>
#include <vector>
#include <check.h> // Check unit testing framework... see http://check.sourceforge.net
#include <boost/test/utils/wrap_stringstream.hpp>
#include <boost/units/detail/utility.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "common/singleton.h"

namespace wfits {
namespace test {

class GlobalSuite
{
	typedef std::map<std::string, TCase*> Cases;
public:
	bool registerTest(TFun, const std::string&);

	SRunner* createRunner(const std::string& = ".*") const;
	
	int argc;
	char** argv;

	const std::string name;

	std::vector<std::string> testNames(const std::string& = ".*") const;

	void logMessage(boost::wrap_stringstream& message) const;

private:
	friend class Singleton<GlobalSuite>;
	GlobalSuite(); // non-default constructible
	GlobalSuite(const GlobalSuite&); // non-copyable

	Cases cases_;
};

typedef Singleton<GlobalSuite> TheGlobalSuite;

template <typename T>
std::string fullname(const T& t)
{
	std::string result(boost::units::simplify_typename(t));
	boost::replace_all(result, "wfits::test::", "");
	boost::replace_all(result, "wfits::", "");
	boost::replace_all(result, "class_", "");
	boost::replace_all(result, "::", "/");
	return result;
}

template <typename T>
std::string fullname()
{
	T tmp;
	return fullname(tmp);
}

#define LOG(message)					\
{							\
	TheGlobalSuite::instance().logMessage(		\
		boost::wrap_stringstream().ref()	\
		<< message				\
	);						\
}

#define TEST_LOG(message)	\
{				\
	LOG("..." << message);	\
}

#define FAIL_UNLESS(expr) fail_unless(expr)
#define FAIL_UNLESS_EQUAL(lhs, rhs) fail_unless(lhs == rhs)
#define FAIL_IF(expr) fail_if(expr)
#define FAIL_IF_EQUAL(lhs, rhs) fail_if(lhs == rhs)
#define FAIL_UNLESS_GE(lhs, rhs) fail_unless(lhs >= rhs)
#define FAIL_UNLESS_LE(lhs, rhs) fail_unless(lhs <= rhs)
#ifndef ASSERT
	#define ASSERT(expr) fail_unless(expr)
#endif

#define ASSERT_MSG(expr, msg)			\
{						\
	boost::wrap_stringstream s;		\
	s.ref() << msg;				\
	ck_assert_msg(expr, s.str().c_str());	\
}

#define TEST(name)								\
	class class_ ## name {							\
	public:									\
	START_TEST(ck_ ## name)							\
	{									\
		LOG(fullname_);							\
		run();								\
	}									\
	END_TEST								\
	static void run();							\
	static std::string fullname_;						\
	static bool registered_;						\
	};									\
	std::string class_ ## name::fullname_ = fullname<class_ ## name>();	\
	bool class_ ## name::registered_ = TheGlobalSuite::instance().registerTest(class_ ## name::ck_ ## name, class_ ## name::fullname_); \
	void class_ ## name::run()

} // namespace test
} // namespace wfits

#endif
