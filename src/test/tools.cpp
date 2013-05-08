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

#include <boost/regex.hpp>

#include "common/util.h"
#include "tools.h"

namespace wfits {
namespace test {

GlobalSuite::GlobalSuite()
	: name("Wayland Functional Integration Test Suite")
{
	return;
}

SRunner* GlobalSuite::createRunner(const std::string& testPattern) const
{
	boost::regex exp(testPattern, boost::regex::egrep | boost::regex::icase);
	boost::cmatch what;

	Suite* suite(suite_create(name.c_str()));
	foreach (const Cases::value_type& tcase, cases_)
	{
		if (boost::regex_match(tcase.first.c_str(), what, exp))
		{
			suite_add_tcase(suite, tcase.second);
		}
	}

	return srunner_create(suite);
}

bool GlobalSuite::registerTest(TFun fn, const std::string& fullname)
{
	TCase* tc(tcase_create(fullname.c_str()));
	tcase_add_test(tc, fn);
	tcase_set_timeout(tc, 30);
	cases_.insert(std::make_pair(fullname, tc));

	return true;
}

std::vector<std::string> GlobalSuite::testNames(const std::string& testPattern) const
{
	boost::regex exp(testPattern, boost::regex::egrep | boost::regex::icase);
	boost::cmatch what;
	std::vector<std::string> result;
	foreach (const Cases::value_type& tcase, cases_)
	{
		if (boost::regex_match(tcase.first.c_str(), what, exp))
		{
			result.push_back(tcase.first);
		}
	}
	return result;
}

namespace tools {
namespace sanity {

TEST(MacroTest)
{
	FAIL_UNLESS(1);
	FAIL_UNLESS(1 == 1);
	FAIL_UNLESS_EQUAL(0, 0);
	FAIL_IF(0);
	FAIL_IF(0 == 1);
	FAIL_IF_EQUAL(0, 1);
	FAIL_UNLESS_GE(1, 1);
	FAIL_UNLESS_GE(1, 0);
	ASSERT(1);
	ASSERT_MSG(1, "sanity failed");
}

TEST(FullnameTest)
{
	FAIL_UNLESS_EQUAL(
		fullname<class_FullnameTest>(),
		std::string("tools/sanity/FullnameTest")
	);
}

} // namespace sanity
} // namespace tools
} // namespace test
} // namespace wfits
