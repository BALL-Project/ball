// $Id: classTest.h,v 1.14 2000/07/04 16:43:48 oliver Exp $

#include <BALL/common.h>
#include <BALL/SYSTEM/file.h>

#include <string>
#include <list>

/**	@name	Class Black Box Testing
		To provide a maximum reliability for all BALL classes, each class
		provides its own test program to ensure that each class compiles
		and behaves (at least basically) as intended.

		The testprograms reside in the directory source/TEST, they may 
		be built and executed by calling {\bf make test}.

		Each test program prints after execution either "PASSED" or "FAILED".
		If any of the subtest contained in the test program fails, the whole 
		test failed. The result of the test program can also be checked via its
		exit code. An exit code of zero means "PASSED", non-zero exit code means 
		"FAILED".

		There are several macros defined to simplify the creation of a test program
		and to provide a common interface.
		Each test program consists of several subtests the usually test one
		method or property of the class. Each of this subtests uses a series
		of elementary tests to check the functionality of the method.

		A number of elementary tests has been implemented that is sufficient
		for most cases:
		\begin{itemize}
			\item \Ref{TEST_EQUAL}
			\item \Ref{TEST_NOT_EQUAL}
			\item \Ref{TEST_REAL_EQUAL}
		\end{itemize}
		A subtest is defined by calling the \Ref{CHECK} macro with the subtest name
		as an argument. Then a series of calls to {\bf TEST} macros follows,
		mixed with standard BALL code (remember to include all neccessary header files).
		The subtest is terminated by calling \Ref{RESULT}.
		Use the two macros \Ref{START_TEST} and \Ref{END_TEST} to generate a complete
		test program.

		To create a new test program, use the file 
		\URL[source/TEST/Skeleton_test.C]{../../source/BALL/TEST/Skeleton_test.C}
		\\ 
		{\bf Definitions:} \URL{BALL/CONCEPT/classTest.h}
*/
//@{

/**	Define the precision for floating point comparisons.
		The macro \Ref{CHECK_REAL_EQUAL} checks whether the floating point number returned by
		the subtest is close to the expected result by comparing the absolute value
		of the difference of the two values to {\bf PRECISION}.\\
		The default value is $10^{-6}$. It is possible to redefine precision in the
		test program by calling this macro with the new value.\\
*/
#define PRECISION(a) \
		TEST::precision = (a);

/**	Create the test header for a certain class.
		This macro defines the start of the test program for a given classname.
		The classname is printed together with some information when calling the 
		test program with any arguments (except for #-v# or #-V#).\\
		This macro should be the first to call in a test program. It introduces a global {\tt try}
		block to catch any unwanted exceptions. If any of these exceptions occurs, all tests failed.
		Exceptions defined by BALL (i.e. exception classes derived from \Ref{GeneralException}) provide
		some additional information that is evaluated by the \Ref{END_TEST} macro. The END_TEST macro
		also closes the {\tt try} block. This {\tt try} block should never catch an exception! 
		All exceptions that are thrown due to some malfunction in one of the member functions should be 
		caught by the {\tt try} block created by \Ref{CHECK} and \Ref{RESULT}.
*/
#define START_TEST(class_name,version)\
/* define a special namespace for all internal variables */\
/* to avoid potential collisions                         */\
namespace TEST {\
	int						verbose = 0;\
	bool					all_tests = true;\
  bool					test = true;\
	bool					this_test;\
	int 					exception = 0;\
	string 				exception_name = "";\
	char*					version_string = version;\
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
		This macro also closes the global {\tt try} block opened by \Ref{START_TEST}
 		and contains the related {\tt catch} clauses. If an exception is caught here,
		the test program fails.
*/
#define END_TEST \
	/* global try block */\
	}\
	/* catch FileNotFound exceptions to print out the file name */\
	catch (BALL::Exception::FileNotFound e)\
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
			if ((e.getLine() > 0) && (!(e.getFile() == "")))\
				std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " while looking for file " << e.getFilename();\
			std::cout << " - unexpected!) " << std::endl;\
		}\
  }\
	/* catch BALL exceptions to retrieve additional information */\
	catch (BALL::Exception::GeneralException e)\
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
			if ((e.getLine() > 0) && (!(e.getFile() == "")))\
				std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " - unexpected!) " << std::endl;\
		}\
  }\
	/* catch all non-BALL exceptions */\
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
\
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
}\

/**	Declare subtest name.
		This macro is used to declare the name of a subtest.
		If you want to check e.g. the setName method of a class,
		insert a line\\
			#CHECK(setName)#\\
		in your test program. If the test program is called in verbose mode,
		this leads to the name of the subtest being printed on execution.\\
		This macro also opens a {\tt try} block to catch any unexpected exceptions thrown
		in the course of a subtest. To catch {\em wanted} exceptions (i.e. to check for exceptions that are
		the expected result of some command) use the \Ref{TEST_EXCEPTION} macro.
		The {\tt try} block opened by CHECK is closed in \Ref{RESULT}, so these two macros
		have to be balanced.
*/
#define CHECK(test_name)  \
	TEST::test = true;\
	TEST::newline = false;\
	if (TEST::verbose > 0)\
		std::cout << "checking " << #test_name << "... " << std::flush;\
	try\
	{\

/**	Print a status message.
		If tests require longer preparations, {\tt STATUS} may be used to 
		print some intermediated progress messages.
		{\tt STATUS} uses {\tt cout} to print these messages (in verbose mode only).
		The given stream expression {\tt message} is prefixed by the string {\tt status:}
		and terminated with a newline. All valid operations on a stream may be performed
		in {\tt message}.\\
		{\bf Example:}\\
		\begin{verbatim}
		STATUS("just calculated x = " << setprecision(10) << x)
		\end{verbatim}
*/
#define STATUS(message)\
	if (TEST::verbose > 1)\
	{\
		if (!TEST::newline) \
		{\
			TEST::newline = true;\
			std::cout << std::endl;\
		}\
		std::cout << "  status: " << message << std::endl;\
	}\

/**	Check subtest result.
		Each elementary test macro updates an internal variable ({\bf TEST}, defined by 
		\Ref{START_TEST}) that holds the state of the current subtest.\\
		{\bf RESULT} prints a whether the subtest has failed or passed in verbose mode
		and updates the internal variables {\bf TEST::all_tests} that describes the state of
		the whole class test. {\bf TEST::all_tests} is initialized to be {\bf true}.
		If any elementary test fails, {\bf TEST::test} becomes {\bf false}.
		At the time of the next call to {\bf RESULT}, {\bf TEST::all_tests} will be
    set to false, if {\bf TEST::test} is false. One failed elementary test leads therefore
		to a failed subtest, which leads to a failed class test.\\
		This macro closes the {\tt try} block opened by CHECK, so CHECK and RESULT--if they are not balanced, 
		ugly compile-time errors may occur.
		RESULT first tries to catch all {\tt BALL} exceptions (i.e. exceptions derived from GeneralException).
		If this fails, it tries to catch any exception. After the exception is thrown, the execution will continue with the 
		next subtest, the current subtest will be marked as failed (as is the whole test program).
*/
#define RESULT \
  }\
	/* catch FileNotFound exceptions to print out the file name */\
	catch (BALL::Exception::FileNotFound e)\
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
			if ((e.getLine() > 0) && (!(e.getFile() == "")))\
				std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " while looking for file " << e.getFilename();\
			std::cout << " - unexpected!) " << std::endl;\
		}\
  }\
  catch (::BALL::Exception::GeneralException e)\
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
      if ((e.getLine() > 0) && (!(e.getFile() == "")))\
        std::cout << ", which was thrown in line " << e.getLine() << " of file " << e.getFile();\
      std::cout << " - unexpected!) " << std::endl;\
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
		its argument. The filename is created using \Ref{File::createTemporaryFilename}.
		All temporary files are deleted if \Ref{END_TEST} is called.
		@param	filename String will contain the filename on completion of the macro
*/
#define NEW_TMP_FILE(filename)\
	::BALL::File::createTemporaryFilename(filename);\
	TEST::tmp_file_list.push_back(filename);\
	
	
/**	Floating point equality macro.
		Checks whether the absolute value of the difference of the two floating point
		values {\bf a} and {\bf b} is less or equal to the value defined by \Ref{PRECISION}.
		@param	a floating point value to test
		@param  b expected value
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
 		std::cout << "    (line " << __LINE__ << ": TEST_REAL_EQUAL("<< #a << ", " << #b << "): got " << (a) << ", expected " << (b) << ") ";\
		if (TEST::this_test)\
			std::cout << " + " << std::endl;\
		else \
			std::cout << " - " << std::endl;\
	}\

/**	Generic equality macro.
		This macro uses the operator == to check its two arguments
		for equality. Besides handling some internal stuff, it basically
		evaluates #((a) == (b))#.\\ 
		Remember that operator == has to be defined somehow for the two 
		argument types.\\
		@param	a value/object to test
		@param	b expected value
*/
#define TEST_EQUAL(a,b)  \
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

/**	Generic inequality macro.
		This macro checks for inequality as \Ref{TEST_EQUAL} tests for equality.
		The only difference between the two macros is that{\bf  TEST_NOT_EQUAL} evaluates
		#!((a) == (b))#.\\
		@param	a value/object to test
		@param	b forbidden value
*/
#define TEST_NOT_EQUAL(a,b)  \
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
	}


/**	Exception test macro.
*/
#define TEST_EXCEPTION(exception_type, command) \
	TEST::exception = 0;\
	try\
	{\
		command;\
	}\
	catch (exception_type)\
	{\
		TEST::exception = 1;\
	}\
	catch (::BALL::Exception::GeneralException e)\
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

/**	File comparison macro.
		This macro is used to test file operations. It compares the file with name {\tt filename} 
		against a template file {\tt templatename}. If {\tt use_regexps} is {\bf true}, 
		each line of the template file starting with {\tt ``/''} is considered to contain a regular
		expression. 
*/
#define TEST_FILE(filename, templatename, use_regexps) \
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
			if ((use_regexps) && (TEST_FILE__template_line.size() > 0) && (TEST_FILE__template_line[0] == '/') && (TEST_FILE__template_line[1] != '/'))\
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
						std::cout << "   TEST_FILE: regexp mismatch: " << TEST_FILE__line << " did not match " << TEST_FILE__template_line(1) << "." << std::endl;\
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
						std::cout << "   TEST_FILE: line mismatch: " << TEST_FILE__line << " differs from " << TEST_FILE__template_line << "." << std::endl;\
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
			std::cout << "    (line " << __LINE__ << ": TEST_FILE(" << #filename << ", " << #templatename << ", ";\
			if (use_regexps)\
			{\
				std::cout << "true";\
			} else {\
				std::cout << "false";\
			}\
			std::cout << ") : " << " cannot open file: ";\
			if (!TEST::infile.good())\
			{\
				std::cout << #filename << " (input file) ";\
			}\
			if (!TEST::templatefile.good())\
			{\
				std::cout << #templatename << " (template file) ";\
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
 		std::cout << "    (line " << __LINE__ << ": TEST_FILE("<< #filename << ", " << #templatename << ", ";\
		if (use_regexps)\
		{\
			std::cout << "true): ";\
		} else {\
			std::cout << "false): ";\
		}\
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
	}

//@}
