// $Id: Options_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/DATATYPE/options.h>

START_TEST(Options, "$Id: Options_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

using BALL::Options;
using BALL::Vector3;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	
Options*	options;
CHECK(default constructor)
options = new Options;
TEST_NOT_EQUAL(options, 0)
RESULT

CHECK(~Options())
delete options;
RESULT

CHECK(setname(String)/getName())
options = new Options;
options->setName("ABCDEFG");
TEST_EQUAL(options->getName(), "ABCDEFG");
RESULT

CHECK(set(String&, String&)/get(String&))
options->set("ABC", "DEF");	
TEST_EQUAL("DEF", options->get("ABC"))
RESULT

Options*	options2;
CHECK(Options(Options&))
options2 = new Options(*options);
TEST_EQUAL(options2->getName(), options->getName())
TEST_EQUAL(options2->get("ABC"), options->get("ABC"))
RESULT

CHECK(setReal(String&, float)/getReal(String&))
options->setReal("REAL", 1.23456);
TEST_REAL_EQUAL(1.23456, options->getReal("REAL"))
RESULT

Vector3	vector(1.0, 2.0, 3.0);
CHECK(setVector/getVector)
options->setVector("VECTOR", vector);
TEST_EQUAL(vector, options->getVector("VECTOR"))
RESULT


CHECK(setInteger/getInteger)
options->setInteger("INT", 1234567890);
TEST_EQUAL(1234567890, options->getInteger("INT"))
RESULT

CHECK(setBool/getBool)
options->setBool("BOOL", true);
TEST_EQUAL(options->getBool("BOOL"), true)
options->setBool("BOOL", false);
TEST_EQUAL(options->getBool("BOOL"), false)
RESULT

CHECK(has)
TEST_EQUAL(options->has("BOOL"), true)
TEST_EQUAL(options->has("UNDEFINED"), false)
RESULT

CHECK(isBool)
TEST_EQUAL(options->isBool("BOOL"), true)
TEST_EQUAL(options->isBool("UNDEFINED"), false)
TEST_EQUAL(options->isBool("INT"), false)
TEST_EQUAL(options->isBool("REAL"), false)
TEST_EQUAL(options->isBool("VECTOR"), false)
RESULT

CHECK(isReal)
TEST_EQUAL(options->isReal("REAL"), true)
TEST_EQUAL(options->isReal("INT"), true)
TEST_EQUAL(options->isReal("BOOL"), false)
TEST_EQUAL(options->isReal("UNDEFINED"), false)
TEST_EQUAL(options->isBool("VECTOR"), false)
RESULT

CHECK(isInteger)
TEST_EQUAL(options->isInteger("INT"), true)
TEST_EQUAL(options->isInteger("REAL"), false)
TEST_EQUAL(options->isInteger("BOOL"), false)
TEST_EQUAL(options->isBool("VECTOR"), false)
TEST_EQUAL(options->isInteger("undefined"), false)
RESULT

CHECK(isVector)
TEST_EQUAL(options->isVector("INT"), false)
TEST_EQUAL(options->isVector("REAL"), false)
TEST_EQUAL(options->isVector("BOOL"), false)
TEST_EQUAL(options->isVector("undefined"), false)
TEST_EQUAL(options->isVector("VECTOR"), true)
options->set("SVECTOR", "(0.0 1.0 2.0) ");
TEST_EQUAL(options->isVector("SVECTOR"), true)
RESULT

CHECK(setDefault)
options->setDefault("DEF", "default");
options->setDefault("DEF", "default2");
TEST_EQUAL(options->get("DEF"), "default")
RESULT

CHECK(setDefaultReal)
options->setDefaultReal("DEFREAL", 1.23456);
options->setDefaultReal("DEFREAL", 2.34567);
TEST_REAL_EQUAL(options->getReal("DEFREAL"), 1.23456)
RESULT

CHECK(setDefaultInteger)
options->setDefaultInteger("DEFINT", 123456);
options->setDefaultInteger("DEFINT", 234567);
TEST_EQUAL(options->getInteger("DEFINT"), 123456)
RESULT

CHECK(setDefaultBool)
options->setDefaultBool("DEFBOOL", true);
options->setDefaultBool("DEFBOOL", false);
TEST_EQUAL(options->getBool("DEFBOOL"), true)
RESULT




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
