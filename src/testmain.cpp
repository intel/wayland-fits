#include "test.h"

int main(int argc, char** argv)
{
	TheGlobalTestSuite::instance().argc = argc;
	TheGlobalTestSuite::instance().argv = argv;

	SRunner* sr = srunner_create (TheGlobalTestSuite::instance());
	//   srunner_set_xml(sr, "results.xml");
	srunner_run_all (sr, CK_ENV);

	int n = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

