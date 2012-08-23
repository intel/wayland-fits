#include <iostream>
#include <fstream>
#include <deque>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "test.h"

namespace po = boost::program_options;

po::variables_map parseArgs(int argc, char** argv)
{
	po::options_description options("optional arguments");
	options.add_options()
		("help", "display this help message")
		("xml", po::value<std::string>(), "send results to XML file")
		("filter", po::value<std::string>()->default_value(".*"), "egrep style regular expression to match tests to run")
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

// <?xml version="1.0"?>
// <TestLog>
//   <TestSuite name="Sanity">
//     <TestSuite name="SuiteA">
//       <TestCase name="TestA">
//         <Info file="main.cpp" line="12"><![CDATA[check true passed]]></Info>
//         <TestingTime>0</TestingTime>
//       </TestCase>
//     </TestSuite>
//     <TestCase name="TestB">
//       <Error file="main.cpp" line="18"><![CDATA[check false failed]]></Error>
//       <TestingTime>0</TestingTime>
//     </TestCase>
//     <TestCase name="TestC">
//       <FatalError file="main.cpp" line="23"><![CDATA[critical check false failed]]></FatalError>
//       <TestingTime>0</TestingTime>
//     </TestCase>
//     <TestCase name="TestD">
//       <Info file="main.cpp" line="28"><![CDATA[check 1 == 1 passed]]></Info>
//       <TestingTime>0</TestingTime>
//     </TestCase>
//     <TestCase name="TestE">
//       <Error file="main.cpp" line="33"><![CDATA[check false == true failed [false != true]]]></Error>
//       <TestingTime>0</TestingTime>
//     </TestCase>
//   </TestSuite>
// </TestLog>

namespace wfit {

class Test
{
public:
	typedef boost::shared_ptr<Test> Shared;
	virtual void toXML(std::ostream&) const = 0;
	const std::string name;
protected:
	Test(const std::string& nm)
		: name(nm)
	{
		return;
	}
};
	
class TestCase : public Test
{
public:
	typedef boost::shared_ptr<TestCase> Shared;

	static Shared create(const std::string& name)
	{
		return Shared(new TestCase(name));
	}

	void toXML(std::ostream& os) const
	{
		std::string cdata("<![CDATA[" + message + "]]>");
		std::string tag = "Info";
		switch (result)
		{
			case CK_ERROR:
				tag = "FatalError";
				break;
			case CK_FAILURE:
				tag = "Error";
				break;
			default:
				break;
		}
		os	<< "<TestCase name=\"" << name << "\">"
			<< "<TestingTime>0</TestingTime>" // "Check" does not provide test times
			<< "<" << tag
			<< " file=\"" << file << "\""
			<< " line=\"" << lineno << "\""
			<< ">" << cdata << "</" << tag << ">"
			<< "</TestCase>"
		;
	}
	
	test_result	result;
	int			lineno;
	std::string	message;
	std::string	file;

private:
	TestCase(const std::string& name)
		: Test::Test(name)
	{
		return;
	}
};

class TestSuite : public Test
{
	// name, type
	typedef std::pair<const std::string, const std::string> Key;
	typedef std::map<Key, Test::Shared> Tests;
public:
	typedef boost::shared_ptr<TestSuite> Shared;

	static Shared create(const std::string& name)
	{
		return Shared(new TestSuite(name));
	}

	void add(::TestResult* result)
	{
		const std::string tcname(tr_tcname(result));
		std::deque<std::string> path;
		boost::split(path, tcname, boost::is_any_of("/"));

		TestCase::Shared test(TestCase::create(path.back()));
		test->result = static_cast<test_result>(tr_rtype(result));
		test->lineno = tr_lno(result);
		test->message = std::string(tr_msg(result));
		test->file = std::string(tr_lfile(result));
		path.pop_back();
		add(Key(test->name, "case"), test, path);
	}

	void toXML(std::ostream& os) const
	{
		os << "<TestSuite name=\"" << name << "\">";
		foreach (Tests::value_type test, tests_)
		{
			test.second->toXML(os);
		}
		os << "</TestSuite>";
	}

private:
	TestSuite(const std::string& name)
		: Test::Test(name)
	{
		return;
	}

	void add(const Key& key, const Test::Shared test)
	{
		tests_.insert(std::make_pair(key, test));
	}

	void add(const Key& key, const Test::Shared test, std::deque<std::string>& path)
	{
		if (path.size() == 0)
		{
			add(key, test);
		}
		else
		{
			typedef Tests::iterator Iterator;
			const Iterator match(tests_.find(Key(path.front(), "suite")));
			Shared suite;
			if (match != tests_.end())
			{
				suite = boost::static_pointer_cast<TestSuite>(match->second);
			}
			else
			{
				suite = create(path.front());
				add(Key(suite->name, "suite"), suite);
			}
			path.pop_front();
			suite->add(key, test, path);
		}
	}

	Tests tests_;
};

} // namespace wfit

/**
 * The "Check" unit test framework does not create valid xml
 * output.  That is, it does not escape/wrap message text
 * with CDATA.  Thus, we'll handle it correctly... also,
 * we use the boost.test xml schema.
 **/
void toXML(SRunner* sr, const std::string& filename)
{
	TestResult **results(srunner_results(sr));
	const int nresults(srunner_ntests_run(sr));

	wfit::TestSuite::Shared master(
		wfit::TestSuite::create(TheGlobalTestSuite::instance().name)
	);
	
	for (int i(0); i < nresults; ++i)
	{
		master->add(results[i]);
	}

	std::ofstream fs;
	fs.open(filename);
	fs << "<TestLog>";
	master->toXML(fs);
	fs << "</TestLog>" << std::endl;
	fs.close();
	free(results);
}

int main(int argc, char** argv)
{
	po::variables_map args(parseArgs(argc, argv));

	TheGlobalTestSuite::instance().argc = argc;
	TheGlobalTestSuite::instance().argv = argv;

	SRunner* sr = TheGlobalTestSuite::instance()
		.createRunner(args["filter"].as<std::string>());

	srunner_run_all (sr, CK_ENV);

	if (args.count("xml"))
	{
		toXML(sr, args["xml"].as<std::string>());
	}

	int n = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

