#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Wayland Functional Test Suite
#define TESTING_PROGRAM
#define BOOST_AUTO_TEST_MAIN

#include <fstream>
#include "test.h"


/**
 *
 * See http://www.boost.org/doc/libs/1_47_0/libs/test/doc/html/index.html
 *
 */

struct GlobalFixture
{
	GlobalFixture()
	{
		std::string log(TESTLOG);
		if (getenv("TESTS_LOGFILE"))
		{
			log = std::string(getenv("TESTS_LOGFILE"));
		}
		testLog = new std::ofstream(log);
		boost::unit_test::unit_test_log.set_stream( *testLog );
	}

	~GlobalFixture()
	{
		return;
	}

	static std::ofstream* testLog;
};

std::ofstream* GlobalFixture::testLog;

BOOST_GLOBAL_FIXTURE( GlobalFixture );