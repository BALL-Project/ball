// $Id: ClassTest_test.C,v 1.7 2001/12/07 00:47:07 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

using namespace BALL;

START_TEST(ClassTest, "$Id: ClassTest_test.C,v 1.7 2001/12/07 00:47:07 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	
CHECK("CHECK(string)")
RESULT

CHECK("RESULT")
RESULT

CHECK("NEW_TMP_FILE(filename)")
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename);
	TEST::this_test = (tmp_filename != "");
RESULT


CHECK("PRECISION(a)")
	PRECISION(0.5)
	TEST_EQUAL(TEST::precision, 0.5)
RESULT

CHECK("TEST_REAL_EQUAL(a, b)")
	PRECISION(0.011)
	TEST_REAL_EQUAL(1.0, 1.01)
	TEST_REAL_EQUAL(1.0, 1.0)
	TEST_REAL_EQUAL(-1.0, -1.01)
	TEST_REAL_EQUAL(-1.01, -1.0)
RESULT

CHECK("TEST_EQUAL(a, b)")
	TEST_EQUAL(1.0, 1.0)
	TEST_EQUAL('A', 'A')
RESULT

CHECK("TEST_NOT_EQUAL(a, b)")
	TEST_NOT_EQUAL(0, 1)
	TEST_NOT_EQUAL('A', 'B')
RESULT

CHECK("TEST_EXCEPTION(a, b)")
	TEST_EXCEPTION(Exception::NullPointer, throw Exception::NullPointer(__FILE__, __LINE__))
RESULT

CHECK("STATUS(a)")
	STATUS("status message")
RESULT

CHECK("TEST_FILE(filename, templatename, use_regexps)")
	TEST_FILE("data/class_test_infile.txt", "data/class_test_template.txt", false)
	TEST_FILE("data/class_test_infile.txt", "data/class_test_regexp_template.txt", true)
RESULT

CHECK("ABORT_IF(condition)")
	ABORT_IF(true)
	TEST_EQUAL(1, 0)
RESULT

CHECK("CAPTURE_OUTPUT(level)/COMPARE_OUTPUT(text)")
	CAPTURE_OUTPUT(123)
		Log.level(123) << "TEST123" << std::endl;
		Log.level(124) << "TEST124" << std::endl;
		Log.level(122) << "TEST122" << std::endl;
	COMPARE_OUTPUT("TEST123\n")

	CAPTURE_OUTPUT(123);
		Log.level(124) << "TEST124" << std::endl;
	COMPARE_OUTPUT("")

	CAPTURE_OUTPUT(124)
		Log.level(124) << "TEST124" << std::endl;
	COMPARE_OUTPUT("TEST124\n")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
