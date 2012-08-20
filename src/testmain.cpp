#include <iostream>
#include <boost/program_options.hpp>
#include "test.h"

namespace po = boost::program_options;

po::variables_map parseArgs(int argc, char** argv)
{
	po::options_description options("optional arguments");
	options.add_options()
		("help", "display this help message")
		("xml", po::value<std::string>(), "send results to XML file")
	;

	po::variables_map args;
	try
	{
		po::store(po::parse_command_line(argc, argv, options), args);
		po::notify(args);
	}
	catch (po::error& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << options << std::endl;
		exit(EXIT_FAILURE);
	}

	if(args.count("help"))
	{
		std::cout << options << std::endl;
		exit(EXIT_SUCCESS);
	}
	
	return args;
}

int main(int argc, char** argv)
{
	po::variables_map args(parseArgs(argc, argv));

	TheGlobalTestSuite::instance().argc = argc;
	TheGlobalTestSuite::instance().argv = argv;

	SRunner* sr = srunner_create (TheGlobalTestSuite::instance());

	if (args.count("xml"))
	{
		srunner_set_xml(sr, args["xml"].as<std::string>().c_str());
	}
	srunner_run_all (sr, CK_ENV);

	int n = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

