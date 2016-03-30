// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_AUTODELETABLE_H
# include <BALL/CONCEPT/autoDeletable.h>
#endif

#ifndef BALL_SYSTEM_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_REGULAREXPRESSION_H
# include <BALL/DATATYPE/regularExpression.h>
#endif

#include <string>
#include <list>

#ifdef BALL_HAS_SSTREAM
# include <sstream>
#else
# include <strstream>
#endif

/**	Define the precision for floating point comparisons.
		The macro  \link #TEST_REAL_EQUAL TEST_REAL_EQUAL \endlink  checks whether the floating point number returned by
		the subtest is close to the expected result by comparing the absolute value
		of the difference of the two values to <b>PRECISION</b>. \par
		The default value is $10^{-6}$. It is possible to redefine precision in the
		test program by calling this macro with the new value. \par
		\ingroup ClassTest
*/
#define PRECISION(a) \
		TEST::precision = (a);

/**	Create the test header for a certain class.
		This macro defines the start of the test program for a given classname.
		The classname is printed together with some information when calling the 
		test program with any arguments (except for #-v# or #-V#). \par
		This macro should be the first to call in a test program. It introduces a global <tt>try</tt>
		block to catch any unwanted exceptions. If any of these exceptions occurs, all tests failed.
		Exceptions defined by BALL (i.e. exception classes derived from  \link BALL::Exception::GeneralException GeneralException \endlink ) provide
		some additional information that is evaluated by the  \link #END_TEST END_TEST \endlink  macro. The END_TEST macro
		also closes the <tt>try</tt> block. This <tt>try</tt> block should never catch an exception! 
		All exceptions that are thrown due to some malfunction in one of the member functions should be 
		caught by the <tt>try</tt> block created by  \link #CHECK CHECK \endlink  and  \link #RESULT RESULT \endlink .
		\ingroup ClassTest
*/
#define START_TEST(class_name)\
/* define a special namespace for all internal variables */\
/* to avoid potential collisions                         */\
namespace TEST {\
	int						verbose = 0;\
	bool					all_tests = true;\
  bool					test = true;\
	bool					this_test;\
	int 					exception = 0;\
	string 				exception_name = "";\
	const char*		version_string = BALL_RELEASE_STRING;\
	bool					newline = false;\
	list<string>	tmp_file_list;\
	std::ifstream	infile;\
	std::ifstream	templatefile;\
	bool					equal_files;\
	double				precision = 1e-6;\
}\
\
\
int main(int argc, char **argv)\
{\
\
	if (argc == 2) {\
		if (!strcmp(argv[1], "-v"))\
			TEST::verbose = 1;\
		if (!strcmp(argv[1], "-V"))\
			TEST::verbose = 2;\
	};\
\
	if ((argc > 2) || ((argc == 2) && (TEST::verbose == 0))) {\
		std::cerr << "Checks " #class_name " class" << std::endl;\
\
		std::cerr << "On successful operation it simply returns OK," << std::endl;\
		std::cerr << "otherwise FAILURE is printed." << std::endl;\
		std::cerr << "If called with an argument of -v, " << argv[0] << " prints detailed" << std::endl;\
		std::cerr << "information about individual tests." << std::endl;\
		std::cerr << "Option -V provides verbose information on" << std::endl;\
		std::cerr << "every subtest." << std::endl;\
		return 1;\
	}\
\
	if (TEST::verbose > 0)\
		std::cout << "Version: " << TEST::version_string << std::endl;\
\
	try {\


/**	Termination of test program.
		This macro implements the correct termination of the 
		test program and should therefore be the last macro to call.
		It determines the exit code based on all previously run
		subtests and prints out the message "PASSED" or "FAILED".
		This macro also closes the global <tt>try</tt> block opened by  \link #START_TEST START_TEST \endlink 
 		and contains the related <tt>catch</tt> clauses. If an exception is caught here,
		the test program fails.
		\ingroup ClassTest
*/
#define END_TEST \
	/* global try block */\
	}\
	/* catch FileNotFound exceptions to print out the file name */\
	catch (BALL::Exception::FileNotFound& e)\
	{\
		TEST::this_test = false;\
		TEST::test = false;\
		TEST::all_tests = false;\
  	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (TEST::exception == 1) /* dummy to avoid compiler warnings */\
				TEST::exception++;\
    	std::cout << std::endl << "    (caught exception of type ";\
			std::cout << e.getName();\
			if ((e.getLine() > 0) && (!(e.getFile()[0] == '\0')))\
				std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " while looking for file " << e.getFilename();\
			std::cout << " - unexpected!) " << std::endl;\
		}\
  }\
	/* catch BALL exceptions to retrieve additional information */\
	catch (BALL::Exception::GeneralException& e)\
	{\
		TEST::this_test = false;\
		TEST::test = false;\
		TEST::all_tests = false;\
  	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (TEST::exception == 1) /* dummy to avoid compiler warnings */\
				TEST::exception++;\
    	std::cout << std::endl << "    (caught exception of type ";\
			std::cout << e.getName();\
			if ((e.getLine() > 0) && (!(e.getFile()[0] == '\0')))\
				std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " - unexpected!) " << std::endl;\
			std::cout << "    (message is: " << e.getMessage() << ")" << std::endl;\
		}\
  }\
	/* catch all std::exception-derived exceptions */\
	catch (std::exception& e)\
	{\
		TEST::this_test = false;\
		TEST::test = false;\
		TEST::all_tests = false;\
  	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
    	std::cout << std::endl << "    (caught expected STL exception outside a subtest: " << e.what() << ")" << std::endl;\
		}\
	}\
\
	/* catch all non-BALL/non-STL exceptions */\
	catch (...)\
	{\
		TEST::this_test = false;\
		TEST::test = false;\
		TEST::all_tests = false;\
  	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
    	std::cout << std::endl << "    (caught unidentified and unexpected exception outside a subtest!) " << std::endl;\
		}\
	}\
	/* clean up all temporary files */\
	while (TEST::tmp_file_list.size() > 0 && TEST::verbose < 1)\
	{\
		::BALL::File::remove(TEST::tmp_file_list.back());\
		TEST::tmp_file_list.pop_back();\
	}\
	/* check for exit code */\
	if (!TEST::all_tests)\
	{\
		std::cout << "FAILED" << std::endl;\
		return 1;\
	} else {\
		std::cout << "PASSED" << std::endl;\
		return 0;\
	}\
	/* Finally, clean up pointers still pointing to */\
	/* AutoDeletable objects, as this might lead to strange */\
	/* warnings (still reachable) when using valgrind. */\
	BALL::AutoDeletable::clearLastPtr(); \
}\


/**	Declare subtest name.
		This macro is used to declare the name of a subtest.
		If you want to check e.g. the setName method of a class,
		insert a line \par
			#CHECK(setName)# \par
		in your test program. If the test program is called in verbose mode,
		this leads to the name of the subtest being printed on execution. \par
		This macro also opens a <tt>try</tt> block to catch any unexpected exceptions thrown
		in the course of a subtest. To catch <b>  wanted </b> exceptions (i.e. to check for exceptions that are
		the expected result of some command) use the  \link #TEST_EXCEPTION TEST_EXCEPTION \endlink  macro.
		The <tt>try</tt> block opened by CHECK is closed in  \link #RESULT RESULT \endlink , so these two macros
		have to be balanced.
		\ingroup ClassTest
*/
#define CHECK(test_name)  \
	TEST::test = true;\
	TEST::newline = false;\
	if (TEST::verbose > 0)\
		std::cout << "checking " << #test_name << "... " << std::flush;\
	try\
	{\
		while (true)\
		{\

/**	Print a status message.
		If tests require longer preparations, <tt>STATUS</tt> may be used to 
		print some intermediated progress messages.
		<tt>STATUS</tt> uses <tt>cout</tt> to print these messages (in verbose mode only).
		The given stream expression <tt>message</tt> is prefixed by the string <tt>status:</tt>
		and terminated with a newline. All valid operations on a stream may be performed
		in <tt>message</tt>. \par
		<b>Example:</b> \par
		\verbatim
		STATUS("just calculated x = " << setprecision(10) << x)
		\endverbatim
		\ingroup ClassTest
*/
#define STATUS(message)\
					if (TEST::verbose > 1)\
					{\
						if (!TEST::newline) \
						{\
							TEST::newline = true;\
							std::cout << std::endl;\
						}\
						std::cout << "  status (line " << __LINE__ << "): " << message << std::endl;\
					}\

/**	Check subtest result.
		Each elementary test macro updates an internal variable (<b>TEST</b>, defined by 
		 \link #START_TEST START_TEST \endlink ) that holds the state of the current subtest. \par
		<b>RESULT</b> prints whether the subtest has failed or passed in verbose mode
		and updates the internal variables <b>TEST::all_tests</b> that describes the state of
		the whole class test. <b>TEST::all_tests</b> is initialized to be <b>true</b>.
		If any elementary test fails, <b>TEST::test</b> becomes <b>false</b>.
		At the time of the next call to <b>RESULT</b>, <b>TEST::all_tests</b> will be
    set to false, if <b>TEST::test</b> is false. One failed elementary test leads therefore
		to a failed subtest, which leads to a failed class test. \par
		This macro closes the <tt>try</tt> block opened by CHECK, so CHECK and RESULT have to 
		be balanced, or some ugly compile-time errors may occur.
		RESULT first tries to catch all <tt>BALL</tt> exceptions (i.e. exceptions 
		derived from GeneralException).	If this fails, it tries to catch any exception. 
		After the exception is thrown, the execution will continue with the next subtest,
		the current subtest will be marked as failed (as is the whole test program).
		\ingroup ClassTest
*/
#define RESULT \
			break;\
		}\
  }\
	/* catch FileNotFound exceptions to print out the file name */\
	catch (BALL::Exception::FileNotFound& e)\
	{\
		TEST::this_test = false;\
		TEST::test = false;\
		TEST::all_tests = false;\
  	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (TEST::exception == 1) /* dummy to avoid compiler warnings */\
				TEST::exception++;\
    	std::cout << std::endl << "    (caught exception of type ";\
			std::cout << e.getName();\
			if ((e.getLine() > 0) && (!(e.getFile()[0] == '\0')))\
				std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " while looking for file " << e.getFilename();\
			std::cout << " - unexpected!) " << std::endl;\
		}\
  }\
  catch (::BALL::Exception::GeneralException& e)\
  {\
    TEST::this_test = false;\
    TEST::test = false;\
    TEST::all_tests = false;\
    if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
    {\
			if (!TEST::newline) \
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
      std::cout << "    (caught exception of type ";\
      std::cout << e.getName();\
      if ((e.getLine() > 0) && (!(e.getFile()[0] == '\0')))\
        std::cout << ", which was thrown in line " << e.getLine() << " of file " << e.getFile();\
      std::cout << " - unexpected!) " << std::endl;\
			std::cout << "    (message is: " << e.getMessage() << ")" << std::endl;\
    }\
  }\
  catch (...)\
  {\
    TEST::this_test = false;\
    TEST::test = false;\
    TEST::all_tests = false;\
    if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
    {\
			if (!TEST::newline) \
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
      std::cout << "    (caught unidentified and unexpected exception!)" << std::endl;\
    }\
  }\
\
	TEST::all_tests = TEST::all_tests && TEST::test;\
	if (TEST::verbose > 0){\
		if (TEST::newline)\
			std::cout << "    ";\
		if (TEST::test){\
			std::cout << "passed" << std::endl;\
		} else {\
			std::cout << "FAILED" << std::endl;\
		}\
	}\

/**	Create a temporary filename.
		This macro assigns a new temporary filename to the string variable given as
		its argument. The filename is created using  \link BALL::File::createTemporaryFilename File::createTemporaryFilename \endlink .
		All temporary files are deleted if  \link #END_TEST END_TEST \endlink  is called.
		@param	filename String will contain the filename on completion of the macro
		\ingroup ClassTest
*/
#define NEW_TMP_FILE(filename)\
					::BALL::File::createTemporaryFilename(filename);\
					TEST::tmp_file_list.push_back(filename);\
					if (TEST::verbose > 1)\
					{\
						if (!TEST::newline) \
						{\
							TEST::newline = true;\
							std::cout << std::endl;\
						}\
						std::cout << "  creating new temporary file '" << filename << "' (line " << __LINE__ << ")" << std::endl;\
					}\
	
/**	Create a temporary filename with given suffix.
		This macro assigns a new temporary filename to the string variable given as
		its argument. The filename is created using  \link BALL::File::createTemporaryFilename File::createTemporaryFilename \endlink .
		All temporary files are deleted if  \link #END_TEST END_TEST \endlink  is called.
		@param	filename String will contain the filename on completion of the macro
		\ingroup ClassTest
*/
#define NEW_TMP_FILE_WITH_SUFFIX(filename, suffix)\
					::BALL::File::createTemporaryFilename(filename, suffix);\
					TEST::tmp_file_list.push_back(filename);\
					if (TEST::verbose > 1)\
					{\
						if (!TEST::newline) \
						{\
							TEST::newline = true;\
							std::cout << std::endl;\
						}\
						std::cout << "  creating new temporary file '" << filename << "' (line " << __LINE__ << ")" << std::endl;\
					}\
	
/**	Floating point equality macro.
		Checks whether the absolute value of the difference of the two floating point
		values <b>a</b> and <b>b</b> is less or equal to the value defined by  \link #PRECISION PRECISION \endlink .
		@param	a floating point value to test
		@param  b expected value
		\ingroup ClassTest
*/
#define TEST_REAL_EQUAL(a,b)  \
	TEST::this_test = BALL_REAL_EQUAL((a), (b), TEST::precision); \
	TEST::test = TEST::test && TEST::this_test;\
	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
	{\
		if (!TEST::newline)\
		{\
			TEST::newline = true;\
			std::cout << std::endl;\
		}\
 		std::cout << "    (line " << __LINE__ << " TEST_REAL_EQUAL("<< #a << ", " << #b << "): got " << (a) << ", expected " << (b) << ") ";\
		if (TEST::this_test)\
			std::cout << " + " << std::endl;\
		else \
			std::cout << " - " << std::endl;\
	}\

/**	Generic equality macro.
		This macro uses the operator == to check its two arguments
		for equality. Besides handling some internal stuff, it basically
		evaluates #((a) == (b))#. \par
		Remember that operator == has to be defined somehow for the two 
		argument types. \par
		@param	a value/object to test
		@param	b expected value
		\ingroup ClassTest
*/
#define TEST_EQUAL(a,b)  \
	{\
		TEST::this_test = ((a) == (b));\
		TEST::test = TEST::test && TEST::this_test;\
		if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (!TEST::newline)\
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
			std::cout << "    (line " << __LINE__ << " TEST_EQUAL(" << #a << ", " << #b << "): got " << (a) << ", expected " << (b) << ") ";\
			if (TEST::this_test)\
				std::cout << " + " << std::endl;\
			else \
				std::cout << " - " << std::endl;\
		}\
	}\

/**	Generic inequality macro.
		This macro checks for inequality as  \link #TEST_EQUAL TEST_EQUAL \endlink  tests for equality.
		The only difference between the two macros is that<b> TEST_NOT_EQUAL</b> evaluates
		#!((a) == (b))#. \par
		@param	a value/object to test
		@param	b forbidden value
		\ingroup ClassTest
*/
#define TEST_NOT_EQUAL(a,b)  \
	{\
		TEST::this_test = !((a) == (b));\
		TEST::test = TEST::test && TEST::this_test;\
		if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (!TEST::newline)\
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
			std::cout << "    (line " << __LINE__ << " TEST_NOT_EQUAL(" << #a << ", " << #b << "): got " << (a) << ", forbidden is " << (b) << ") ";\
			if (TEST::this_test)\
				std::cout << " + " << std::endl;\
			else \
				std::cout << " - " << std::endl;\
		}\
	}\

/**	Exception test macro.
		This macro checks if a given type of exception occured while executing the
		given command.
		Example: \par
		#TEST_EXCEPTION(Exception::IndexOverflow, vector3[-1])# \par
		If no or a wrong exception occured, false is returned, otherwise true.
		@param exception_type the exception-class
		@param command any general C++ or BALL-specific command
		\ingroup ClassTest
*/
#define TEST_EXCEPTION(exception_type, command) \
	{\
		TEST::exception = 0;\
		try\
		{\
			command;\
		}\
		catch (exception_type&)\
		{\
			TEST::exception = 1;\
		}\
		catch (::BALL::Exception::GeneralException& e)\
		{\
			TEST::exception = 2;\
			TEST::exception_name = e.getName();\
		}\
		catch (...)\
		{ \
			TEST::exception = 3;\
		}\
		TEST::this_test = (TEST::exception == 1);\
		TEST::test = TEST::test && TEST::this_test;\
		\
		if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (!TEST::newline)\
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
			std::cout << "    (line " << __LINE__ << " TEST_EXCEPTION(" << #exception_type << ", " << #command << "): ";\
			switch (TEST::exception)\
			{\
				case 0:	std::cout << " ERROR: no exception!) "; break;\
				case 1:	std::cout << " OK) "; break;\
				case 2:	std::cout << " ERROR: wrong exception: " << TEST::exception_name << ") "; break;\
				case 3:	std::cout << " ERROR: wrong exception!) "; break;\
			}\
			if (TEST::this_test)\
				std::cout << " + " << std::endl;\
			else \
				std::cout << " - " << std::endl;\
		}\
	}\

#ifdef BALL_DEBUG
/**	Precondition exception test macro.
		This macro checks if a the command specified correctly throws
		an exception of type Precondition because one of its preconditions
		is violated. If BALL is not compile din DEBUG mode, this macro
		won't do anything, as the BALL_EXCEPTION_PRECONDITION macro is
		expanded to nothing in that case.
		\ingroup ClassTest
*/
#define TEST_PRECONDITION_EXCEPTION(command) \
	{\
		TEST::exception = 0;\
		try\
		{\
			command;\
		}\
		catch (Exception::Precondition&)\
		{\
			TEST::exception = 1;\
		}\
		catch (::BALL::Exception::GeneralException& e)\
		{\
			TEST::exception = 2;\
			TEST::exception_name = e.getName();\
		}\
		catch (...)\
		{ \
			TEST::exception = 3;\
		}\
		TEST::this_test = (TEST::exception == 1);\
		TEST::test = TEST::test && TEST::this_test;\
		\
		if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (!TEST::newline)\
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
			std::cout << "    (line " << __LINE__ << " TEST_PRECONDITION_EXCEPTION(" << ", " << #command << "): ";\
			switch (TEST::exception)\
			{\
				case 0:	std::cout << " ERROR: no exception!) "; break;\
				case 1:	std::cout << " OK) "; break;\
				case 2:	std::cout << " ERROR: wrong exception: " << TEST::exception_name << ") "; break;\
				case 3:	std::cout << " ERROR: wrong exception!) "; break;\
			}\
			if (TEST::this_test)\
				std::cout << " + " << std::endl;\
			else \
				std::cout << " - " << std::endl;\
		}\
	}\

#else

# define TEST_PRECONDITION_EXCEPTION(command)\
	if (TEST::verbose > 1)\
	{\
		std::cout << "  TEST_EXCEPTION_PRECONDITION(" #command ") : (DEBUG mode disabled!)" << std::endl;\
	}\

#endif // BALL_DEBUG

/** Skip remainder of subtest.
    If the condition is not fulfilled, the remainder of the test is skipped.
    The status (whether it fails or passes) remains unchanged.
		\ingroup ClassTest
*/
#define ABORT_IF(condition) \
  if (condition) break;

/**	File comparison macro.
		This macro is used to test file operations. It compares the file with name <tt>filename</tt> 
		against a template file <tt>templatename</tt>. Corresponding lines of the two files
		have to be identical. 
		@see TEST_FILE_REGEXP for more sophisticated comparisons
		\ingroup ClassTest
*/
#define TEST_FILE(filename, templatename) \
	{\
		TEST::equal_files = true;\
		TEST::infile.open(filename, std::ios::in);\
		TEST::templatefile.open(templatename, std::ios::in);\
		\
		if (TEST::infile.good() && TEST::templatefile.good())\
		{\
			String TEST_FILE__template_line;\
			String TEST_FILE__line;\
			\
			while (TEST::infile.good() && TEST::templatefile.good())\
			{\
				TEST_FILE__template_line.getline(TEST::templatefile);\
				TEST_FILE__line.getline(TEST::infile);\
				\
				TEST::equal_files &= (TEST_FILE__template_line == TEST_FILE__line);\
				if (TEST_FILE__template_line != TEST_FILE__line)\
				{\
					if (TEST::verbose > 0)\
					{\
						if (!TEST::newline)\
						{\
							TEST::newline = true;\
							std::cout << std::endl;\
						}\
						\
						std::cout << "   TEST_FILE: line mismatch:\n    got:      '" << TEST_FILE__line << "'\n    expected: '" << TEST_FILE__template_line << "'" << std::endl;\
					}\
				}\
			}\
		} else {\
			TEST::equal_files = false;\
			\
			if (TEST::verbose > 0)\
			{\
				if (!TEST::newline)\
				{\
					TEST::newline = true;\
					std::cout << std::endl;\
				}\
				\
				std::cout << "    (line " << __LINE__ << ": TEST_FILE(" << #filename << ", " << #templatename ;\
				std::cout << ") : " << " cannot open file: \"";\
				if (!TEST::infile.good())\
				{\
					std::cout << filename << "\" (input file) ";\
				}\
				if (!TEST::templatefile.good())\
				{\
					std::cout << templatename << "\" (template file) ";\
				}\
				std::cout << std::endl;\
				\
			}\
		}\
		TEST::infile.close();\
		TEST::templatefile.close();\
		TEST::infile.clear();\
		TEST::templatefile.clear();\
		\
		TEST::this_test = TEST::equal_files;\
		TEST::test = TEST::test && TEST::this_test;\
		if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (!TEST::newline)\
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
			std::cout << "    (line " << __LINE__ << ": TEST_FILE("<< #filename << ", " << #templatename << "): ";\
			if (TEST::this_test)\
			{\
				std::cout << "true";\
			} else {\
				std::cout << "false";\
			}\
			\
			if (TEST::this_test)\
			{\
				std::cout << " + " << std::endl;\
			} else {\
				std::cout << " - " << std::endl;\
			}\
		}\
	}


/**	Regular expression file comparison macro.
		This macro is used to test file operations. It compares the file with name <tt>filename</tt> 
		against a template file <tt>templatename</tt>. 
		Each line of the template file starting with <tt>``/''</tt> is considered to contain a regular
		expression, which has to match the corresponding line in the input file. All other lines
		of the input and the template file have to be identical.
		\ingroup ClassTest
*/
#define TEST_FILE_REGEXP(filename, templatename) \
	{\
		TEST::equal_files = true;\
		TEST::infile.open(filename, std::ios::in);\
		TEST::templatefile.open(templatename, std::ios::in);\
		\
		if (TEST::infile.good() && TEST::templatefile.good())\
		{\
			String TEST_FILE__template_line;\
			String TEST_FILE__line;\
			\
			while (TEST::infile.good() && TEST::templatefile.good())\
			{\
				TEST_FILE__template_line.getline(TEST::templatefile);\
				TEST_FILE__line.getline(TEST::infile);\
				\
				if ((TEST_FILE__template_line.size() > 0) && (TEST_FILE__template_line[0] == '/') && (TEST_FILE__template_line[1] != '/'))\
				{\
					RegularExpression expression(TEST_FILE__template_line(1));\
					bool match = expression.match(TEST_FILE__line);\
					TEST::equal_files &= match;\
					if (!match)\
					{\
						if (TEST::verbose > 0)\
						{\
							if (!TEST::newline)\
							{\
								TEST::newline = true;\
								std::cout << std::endl;\
							}\
							\
							std::cout << "   TEST_FILE_REGEXP: regexp mismatch: " << TEST_FILE__line << " did not match " << TEST_FILE__template_line(1) << "." << std::endl;\
						}\
					}\
				} else {\
					TEST::equal_files &= (TEST_FILE__template_line == TEST_FILE__line);\
					if (TEST_FILE__template_line != TEST_FILE__line)\
					{\
						if (TEST::verbose > 0)\
						{\
							if (!TEST::newline)\
							{\
								TEST::newline = true;\
								std::cout << std::endl;\
							}\
							\
  						std::cout << "   TEST_FILE: line mismatch:\n    got:      '" << TEST_FILE__line << "'\n    expected: '" << TEST_FILE__template_line << "'" << std::endl;\
						}\
					}\
				}\
			}\
		} else {\
			TEST::equal_files = false;\
			\
			if (TEST::verbose > 0)\
			{\
				if (!TEST::newline)\
				{\
					TEST::newline = true;\
					std::cout << std::endl;\
				}\
				\
				std::cout << "    (line " << __LINE__ << ": TEST_FILE_REGEXP(" << #filename << ", " << #templatename ;\
				std::cout << ") : " << " cannot open file: \"";\
				if (!TEST::infile.good())\
				{\
					std::cout << filename << "\" (input file) ";\
				}\
				if (!TEST::templatefile.good())\
				{\
					std::cout << templatename << "\" (template file) ";\
				}\
				std::cout << std::endl;\
				\
			}\
		}\
		TEST::infile.close();\
		TEST::templatefile.close();\
		TEST::infile.clear();\
		TEST::templatefile.clear();\
		\
		TEST::this_test = TEST::equal_files;\
		TEST::test = TEST::test && TEST::this_test;\
		if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			if (!TEST::newline)\
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
			std::cout << "    (line " << __LINE__ << ": TEST_FILE_REGEXP("<< #filename << ", " << #templatename << "): ";\
			if (TEST::this_test)\
			{\
				std::cout << "true";\
			} else {\
				std::cout << "false";\
			}\
			\
			if (TEST::this_test)\
			{\
				std::cout << " + " << std::endl;\
			} else {\
				std::cout << " - " << std::endl;\
			}\
		}\
	}


/**	Redirect output to the global logging facility.
		This macro (together with  \link #COMPARE_OUTPUT COMPARE_OUTPUT \endlink ) can be used
		to ensure that a function prints an error message to the
		global logging facility  \link BALL::LogStream Log \endlink . It disables the output
		to <tt>cout</tt> and <tt>cerr</tt> and redirects all output to
		<tt>level</tt> to a temporary <tt>ostringstream</tt>. The contents 
		of this stream can be compared with the expected output	
		afterwards using the macro  \link #COMPARE_OUTPUT COMPARE_OUTPUT \endlink .
		Each <tt>CAPTURE_OUTPUT</tt> requires exactly one subsequent
		 \link #COMPARE_OUTPUT COMPARE_OUTPUT \endlink  macro.
		\ingroup ClassTest
*/
#ifdef BALL_HAS_SSTREAM
#define CAPTURE_OUTPUT_LEVEL(level) \
	{\
		std::ostringstream TEST_strstr;\
		Log.remove(std::cout);\
		Log.remove(std::cerr);\
		Log.insert(TEST_strstr, level, level);
#else
#define CAPTURE_OUTPUT_LEVEL(level) \
	{\
		std::ostrstream TEST_strstr;\
		Log.remove(std::cout);\
		Log.remove(std::cerr);\
		Log.insert(TEST_strstr, level, level);
#endif

/**	Redirect output to the global logging facility.
		This macro (together with  \link #COMPARE_OUTPUT COMPARE_OUTPUT \endlink ) can be used
		to ensure that a function prints an error message to the
		global logging facility  \link BALL::LogStream Log \endlink . It disables the output
		to <tt>cout</tt> and <tt>cerr</tt> and redirects all output to
		<tt>level</tt> to a temporary <tt>ostringstream</tt>. The contents 
		of this stream can be compared with the expected output	
		afterwards using the macro  \link #COMPARE_OUTPUT COMPARE_OUTPUT \endlink .
		Each <tt>CAPTURE_OUTPUT</tt> requires exactly one subsequent
		 \link #COMPARE_OUTPUT COMPARE_OUTPUT \endlink  macro.
		\ingroup ClassTest
*/
#ifdef BALL_HAS_SSTREAM
#define CAPTURE_OUTPUT_LEVEL_RANGE(minlevel, maxlevel) \
	{\
		std::ostringstream TEST_strstr;\
		Log.remove(std::cout);\
		Log.remove(std::cerr);\
		Log.insert(TEST_strstr, minlevel, maxlevel);
#else
#define CAPTURE_OUTPUT_LEVEL_RANGE(minlevel, maxlevel) \
	{\
		std::ostrstream TEST_strstr;\
		Log.remove(std::cout);\
		Log.remove(std::cerr);\
		Log.insert(TEST_strstr, minlevel, maxlevel);
#endif
/**	Compare output made to the global logging facility.
		@see CAPTURE_OUTPUT
		\ingroup ClassTest
*/

#ifdef BALL_HAS_SSTREAM
#define COMPARE_OUTPUT(text) \
                Log.remove(TEST_strstr);\
                Log.insert(std::cout, LogStream::INFORMATION_LEVEL, LogStream::ERROR_LEVEL - 1);\
                Log.insert(std::cerr, LogStream::ERROR_LEVEL);\
                TEST::this_test = (::strncmp(TEST_strstr.str().c_str(), text, TEST_strstr.str().size()) == 0);\
                TEST::test = TEST::test && TEST::this_test;\
                \
                if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
                {\
                        /* reserve space for the null-terminated content of the strstrem */\
                        char* TEST_strstr_contents = new char[TEST_strstr.str().size() + 1];\
                        ::strncpy(TEST_strstr_contents, TEST_strstr.str().c_str(), TEST_strstr.str().size());\
                        TEST_strstr_contents[TEST_strstr.str().size()] = '\0';\
                        \
                        if (!TEST::newline)\
                        {\
                                TEST::newline = true;\
                                std::cout << std::endl;\
                        }\
                        std::cout << "    (line " << __LINE__ << " COMPARE_OUTPUT(" << #text << "): got '" << (TEST_strstr_contents) << "', expected '" << (text) << ") ";\
                        if (TEST::this_test)\
                                std::cout << " + " << std::endl;\
                        else \
                                std::cout << " - " << std::endl;\
                        delete [] TEST_strstr_contents;\
                }\
        }
#else
#define COMPARE_OUTPUT(text) \
		Log.remove(TEST_strstr);\
		Log.insert(std::cout, LogStream::INFORMATION_LEVEL, LogStream::ERROR_LEVEL - 1);\
		Log.insert(std::cerr, LogStream::ERROR_LEVEL);\
		TEST::this_test = (::strncmp(TEST_strstr.str(), text, TEST_strstr.str()!=0?strlen(TEST_strstr.str()):0) == 0);\
		TEST::test = TEST::test && TEST::this_test;\
		\
		if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))\
		{\
			/* reserve space for the null-terminated content of the strstrem */\
			char* TEST_strstr_contents = new char[TEST_strstr.str()!=0?strlen(TEST_strstr.str()):0 + 1];\
			::strncpy(TEST_strstr_contents, TEST_strstr.str(), TEST_strstr.str()!=0?strlen(TEST_strstr.str()):0);\
			TEST_strstr_contents[TEST_strstr.str()!=0?strlen(TEST_strstr.str()):0] = '\0';\
			\
			if (!TEST::newline)\
			{\
				TEST::newline = true;\
				std::cout << std::endl;\
			}\
			std::cout << "    (line " << __LINE__ << " COMPARE_OUTPUT(" << #text << "): got '" << (TEST_strstr_contents) << "', expected '" << (text) << "') ";\
			if (TEST::this_test)\
				std::cout << " + " << std::endl;\
			else \
				std::cout << " - " << std::endl;\
			delete [] TEST_strstr_contents;\
		}\
	}

#endif	
