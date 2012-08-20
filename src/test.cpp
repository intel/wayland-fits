#include "test.h"

GlobalTestSuite::GlobalTestSuite()
	: suite_(suite_create("Wayland Functional Test Suite"))
{
	return;
}

GlobalTestSuite::operator Suite*() const
{
	return suite_;
}

bool GlobalTestSuite::registerTest(TFun fn, const std::string& s)
{
	TCase* tc(getCase(s));
	tcase_add_test(tc, fn);
	tcase_set_timeout(tc, 30);
}

TCase* GlobalTestSuite::getCase(const std::string& name)
{
	typedef std::map<std::string, TCase*>::iterator Iterator;
	const Iterator match = cases_.find(name);
	if (match != cases_.end())
	{
		return match->second;
	}
	TCase* tc(tcase_create(name.c_str()));
	suite_add_tcase(suite_, tc);
	cases_.insert(std::make_pair(name, tc));
	return tc;
}
