// $Id: TransformationManager_test.C,v 1.3 2001/12/29 17:58:29 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/file.h>

///////////////////////////

START_TEST(TransformationManager, "$Id: TransformationManager_test.C,v 1.3 2001/12/29 17:58:29 oliver Exp $")

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
	TransformationManager tm;
	tm.registerTransformation("", "%s %s");
	TEST_EQUAL(tm.transform("test"), "test test")
	TEST_EQUAL(tm.transform(" "), "   ")
	TEST_EQUAL(tm.transform(""), " ")
	tm.unregisterTransformation("");
	tm.registerTransformation("", "TEST");
	TEST_EQUAL(tm.transform("test"), "TEST")
	TEST_EQUAL(tm.transform(" "), "TEST")
	TEST_EQUAL(tm.transform(""), "TEST")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
