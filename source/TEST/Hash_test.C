// $Id: Hash_test.C,v 1.2 2000/06/06 09:46:02 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/COMMON/hash.h>
///////////////////////////

START_TEST(class_name, "$Id: Hash_test.C,v 1.2 2000/06/06 09:46:02 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
char* c = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
string s1 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
String s2 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

CHECK(hashString(const char* str))
	TEST_EQUAL(hashString(c), 140)
RESULT

CHECK(hashString2(const char* str))
	//TEST_EQUAL(hashString2(c), 248)
RESULT

CHECK(hashString3(const char* str))
	TEST_EQUAL(hashString3(c), 3780768363U)
RESULT

CHECK(hashPJWString(const char* str))
	TEST_EQUAL(hashPJWString(c), 1450744505U)
RESULT

CHECK(hashElfString(const char* str))
	TEST_EQUAL(hashElfString(c), 19269177)
RESULT

CHECK(Hash(const T& key))
	TEST_EQUAL(Hash(1), 1)
RESULT

CHECK(Hash(const string& s))
	TEST_EQUAL(Hash(s1), 140)
RESULT

CHECK(Hash(const String& s))
	TEST_EQUAL(Hash(s2), 140)
RESULT

CHECK(Hash(void *const& ptr))
	//TEST_EQUAL(Hash(c), 248)
RESULT

CHECK(getNextPrime(HashIndex l))
	TEST_EQUAL(getNextPrime(0), 3)
	TEST_EQUAL(getNextPrime(1), 3)
	TEST_EQUAL(getNextPrime(4), 5)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
