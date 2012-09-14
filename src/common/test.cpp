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
