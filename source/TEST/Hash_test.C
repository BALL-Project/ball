// $Id: Hash_test.C,v 1.4 2000/06/07 14:26:28 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/COMMON/hash.h>
///////////////////////////

START_TEST(class_name, "$Id: Hash_test.C,v 1.4 2000/06/07 14:26:28 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
char* c = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
string s1 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
String s2 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

CHECK(hashString(const char* str))
	TEST_EQUAL(hashString(c), 140)
RESULT

CHECK(hashPJWString(const char* str))
	TEST_EQUAL(hashPJWString(c), 1450744505)
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
	void *x;
	TEST_EQUAL(Hash(x), 2147483648)
RESULT

CHECK(getNextPrime(HashIndex l))
	TEST_EQUAL(getNextPrime(4), 5)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
