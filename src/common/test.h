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

#ifndef _WAYLAND_TEST_TEST_H_
#define _WAYLAND_TEST_TEST_H_

#include <iostream>
#include <map>
#include <vector>
#include <check.h> // Check unit testing framework... see http://check.sourceforge.net
#include <boost/test/utils/wrap_stringstream.hpp>

#include "singleton.h"

class GlobalTestSuite
{
	typedef std::map<std::string, TCase*> Cases;

public:
	bool registerTest(TFun, const std::string&);

	SRunner* createRunner(const std::string& = ".*") const;
	
	int argc;
	char** argv;

	const std::string name;

	std::vector<std::string> testNames(const std::string& = ".*") const;

private:
	friend class Singleton<GlobalTestSuite>;
	GlobalTestSuite(); // non-default constructible
	GlobalTestSuite(const GlobalTestSuite&); // non-copyable

	Cases cases_;
};

typedef Singleton<GlobalTestSuite> TheGlobalTestSuite;

#define FAIL_UNLESS(expr) fail_unless(expr)
#define FAIL_UNLESS_EQUAL(lhs, rhs) fail_unless(lhs == rhs)
#define FAIL_IF(expr) fail_if(expr)
#define FAIL_IF_EQUAL(lhs, rhs) fail_if(lhs == rhs)
#define FAIL_UNLESS_GE(lhs, rhs) fail_unless(lhs >= rhs)
#ifndef ASSERT
	#define ASSERT(expr) fail_unless(expr)
#endif

#define ASSERT_MSG(expr, msg) \
{ \
	boost::wrap_stringstream s; \
	s.ref() << msg; \
	ck_assert_msg(expr, s.str().c_str()); \
}

#define TEST(name, suite) \
	class class_ ## name { \
	public: \
	START_TEST(ck_ ## name) \
	{ \
		std::cout << suite "/" # name << std::endl; \
		run(); \
	} \
	END_TEST \
	static void run(); \
	static bool registered; \
	}; \
	bool class_ ## name::registered = TheGlobalTestSuite::instance().registerTest(class_ ## name::ck_ ## name, suite "/" # name); \
	void class_ ## name::run()

#endif
