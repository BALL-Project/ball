// $Id: TransformationManager_test.C,v 1.5 2001/12/31 00:30:54 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/file.h>

///////////////////////////

START_TEST(TransformationManager, "$Id: TransformationManager_test.C,v 1.5 2001/12/31 00:30:54 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

TransformationManager* ptr = 0;
CHECK(TransformationManger::TransformationManager())
	ptr = new TransformationManager;
	TEST_NOT_EQUAL(ptr, 0)
RESULT											

CHECK(TransformationManger::~TransformationManager())
	delete ptr;
RESULT											

TransformationManager tm;
CHECK(TransformationManager::registerTransformation(const String& pattern, const String& command))
	tm.registerTransformation("asd", "asd_command");
RESULT

CHECK(TransformationManager::findTransformation(const String& name) const)
	TEST_EQUAL(tm.findTransformation("asd"), "asd_command")
	TEST_EQUAL(tm.findTransformation("ascasc"), "")
	tm.registerTransformation(".*\\.gz", "exec:gunzip -c %s");
	TEST_EQUAL(tm.findTransformation("asd"), "asd_command")
	TEST_EQUAL(tm.findTransformation("ascasc"), "")
	TEST_EQUAL(tm.findTransformation("asd.gz"), "exec:gunzip -c %s")
RESULT

CHECK(TransformationManager::unregisterTransformation(const String& pattern))
	tm.unregisterTransformation("asd");
	TEST_EQUAL(tm.findTransformation("asd"), "")
	TEST_EQUAL(tm.findTransformation("ascasc"), "")
	TEST_EQUAL(tm.findTransformation("asd.gz"), "exec:gunzip -c %s")
	tm.unregisterTransformation(".*\\.gz");
	TEST_EQUAL(tm.findTransformation("asd"), "")
	TEST_EQUAL(tm.findTransformation("ascasc"), "")
	TEST_EQUAL(tm.findTransformation("asd.gz"), "")
RESULT

CHECK(TransformationManager::transform(const String& name))
	// %s: full name 
	String test_test(String("test") + FileSystem::PATH_SEPARATOR + "test.sfx");
	TransformationManager tm;
	tm.registerTransformation("", "%s %s");
	TEST_EQUAL(tm.transform(test_test), test_test + " " + test_test)
	TEST_EQUAL(tm.transform(" "), "   ")
	TEST_EQUAL(tm.transform(""), " ")
	// check whether we really avoid infinite recursion	
	TEST_EQUAL(tm.transform("%s-").hasPrefix("%s---"), true)
	tm.unregisterTransformation("");
	tm.registerTransformation("", "TEST");
	TEST_EQUAL(tm.transform(test_test), "TEST")
	TEST_EQUAL(tm.transform(" "), "TEST")
	TEST_EQUAL(tm.transform(""), "TEST")
	tm.unregisterTransformation("");

	// %p: path name
	const String& PS = FileSystem::PATH_SEPARATOR;
	tm.registerTransformation("", "AA%pAA%p");
	TEST_EQUAL(tm.transform(test_test), String("AAtest") + PS + "AAtest" + PS)
	TEST_EQUAL(tm.transform(" "), "AAAA")
	TEST_EQUAL(tm.transform(""), "AAAA")
	tm.unregisterTransformation("");

	// %t: temporary file name
	tm.registerTransformation("", "AA%tAA%t");
	TEST_EQUAL(tm.transform(test_test).hasPrefix("AA"), true)
	TEST_EQUAL(tm.transform(" ").hasPrefix("AA"), true)
	TEST_EQUAL(tm.transform("").hasPrefix("AA"), true)
	tm.unregisterTransformation("");

 	// %f: full name without last dot-separated suffix
	tm.registerTransformation("", "A%fB%fC");
	TEST_EQUAL(tm.transform(test_test), String("Atest") + PS + "testBtest" + PS + "testC")
	TEST_EQUAL(tm.transform(" "), "A B C")
	TEST_EQUAL(tm.transform(""), "ABC")
	tm.unregisterTransformation("");

	// %f: full name without user-defined suffix
	String test(PS + "test" + PS + PS + "TEST" + PS + "basename");
	tm.registerTransformation("", "A%f[suffix]B%fC");
	TEST_EQUAL(tm.transform(test + "suffix"), String("A") + test + "B" + test + "suffixC")
	TEST_EQUAL(tm.transform(" suffix"), "A B suffixC")
	TEST_EQUAL(tm.transform(""), "ABC")
	tm.unregisterTransformation("");

 	// %b: base name without last dot-separated suffix
	tm.registerTransformation("", "A%bB%bC");
	TEST_EQUAL(tm.transform(test_test), "AtestBtestC");
	TEST_EQUAL(tm.transform(" "), "A B C")
	TEST_EQUAL(tm.transform(""), "ABC")
	tm.unregisterTransformation("");

	// %b: base name without user-defined suffix
	tm.registerTransformation("", "A%b[suffix]B%bC");
	TEST_EQUAL(tm.transform(test + "suffix"), "AbasenameBbasenamesuffixC")
	TEST_EQUAL(tm.transform(" suffix"), "A B suffixC")
	TEST_EQUAL(tm.transform(""), "ABC")
	tm.unregisterTransformation("");
	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
