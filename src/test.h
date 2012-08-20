#ifndef _WAYLAND_TEST_TEST_H_
#define _WAYLAND_TEST_TEST_H_

#include <boost/foreach.hpp>
#include <map>
#include <check.h> // Check unit testing framework... see http://check.sourceforge.net

#include "singleton.h"

#define foreach BOOST_FOREACH
#define foreach_reverse BOOST_REVERSE_FOREACH

class GlobalTestSuite
{
public:
	operator Suite*() const;
	bool registerTest(TFun, const std::string&);

	int argc;
	char** argv;

private:
	friend class Singleton<GlobalTestSuite>;
	GlobalTestSuite(); // non-default constructible
	GlobalTestSuite(const GlobalTestSuite&); // non-copyable
	TCase* getCase(const std::string&);

	Suite* suite_;
	std::map<std::string, TCase*> cases_;
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

#define TEST(name, suite) \
	class class_ ## name { \
	public: \
	START_TEST(name) \
	{ \
		run(); \
	} \
	END_TEST \
	static void run(); \
	static bool registered; \
	}; \
	bool class_ ## name::registered = TheGlobalTestSuite::instance().registerTest(class_ ## name::name, suite "/" # name); \
	void class_ ## name::run()

#endif
