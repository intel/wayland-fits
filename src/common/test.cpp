#include <boost/regex.hpp>

#include "test.h"

GlobalTestSuite::GlobalTestSuite()
	: name("Wayland Functional Test Suite")
{
	return;
}

SRunner* GlobalTestSuite::createRunner(const std::string& testPattern) const
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

bool GlobalTestSuite::registerTest(TFun fn, const std::string& name)
{
	TCase* tc(tcase_create(name.c_str()));
	tcase_add_test(tc, fn);
	tcase_set_timeout(tc, 30);
	cases_.insert(std::make_pair(name, tc));
}

std::vector<std::string> GlobalTestSuite::testNames(const std::string& testPattern) const
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

TEST(MacroTest, "Common/Sanity")
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