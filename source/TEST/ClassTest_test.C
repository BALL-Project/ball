// $Id: ClassTest_test.C,v 1.1 1999/11/26 16:47:01 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

using namespace BALL;

START_TEST(ClassTest, "$Id: ClassTest_test.C,v 1.1 1999/11/26 16:47:01 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	
CHECK(CHECK(string))
RESULT

CHECK(RESULT)
RESULT

CHECK(NEW_TMP_FILE(filename))
String tmp_filename;
NEW_TMP_FILE(tmp_filename);
TEST::this_test = (tmp_filename != "");
RESULT

CHECK(TEST_REAL_EQUAL(a, b))
#undef PRECISION
#define PRECISION 0.011
TEST_REAL_EQUAL(1.0, 1.01)
TEST_REAL_EQUAL(1.0, 1.0)
TEST_REAL_EQUAL(-1.0, -1.01)
TEST_REAL_EQUAL(-1.01, -1.0)
RESULT

CHECK(TEST_EQUAL(a, b))
TEST_EQUAL(1.0, 1.0)
TEST_EQUAL('A', 'A')
TEST_EQUAL("abc", "abc")
RESULT

CHECK(TEST_NOT_EQUAL(a, b))
TEST_NOT_EQUAL(0, 1)
TEST_NOT_EQUAL("abc", "bcd")
TEST_NOT_EQUAL('A', 'B')
RESULT

CHECK(TEST_EXCEPTION(a, b))
TEST_EXCEPTION(Exception::NullPointer, throw Exception::NullPointer(__FILE__, __LINE__))
RESULT

CHECK(TEST_FILE(filename, templatename, use_regexps))
TEST_FILE("data/class_test_infile.txt", "data/class_test_template.txt", false)
TEST_FILE("data/class_test_infile.txt", "data/class_test_regexp_template.txt", true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
