// $Id: TransformationManager_test.C,v 1.1 2001/12/29 01:07:05 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/file.h>

///////////////////////////

START_TEST(TransformationManager, "$Id: TransformationManager_test.C,v 1.1 2001/12/29 01:07:05 oliver Exp $")

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
	TEST_EQUAL(tm.findTransformation("asdasd"), "")
	tm.registerTransformation(".*\\.gz", "exec:gunzip -c %s");
	TEST_EQUAL(tm.findTransformation("asd"), "asd_command")
	TEST_EQUAL(tm.findTransformation("asdasd"), "")
	TEST_EQUAL(tm.findTransformation("asd.gz"), "exec:gunzip -c %s")
RESULT

CHECK(TransformationManager::unregisterTransformation(const String& pattern))
	tm.unregisterTransformation("asd");
	TEST_EQUAL(tm.findTransformation("asd"), "")
	TEST_EQUAL(tm.findTransformation("asdasd"), "")
	TEST_EQUAL(tm.findTransformation("asd.gz"), "exec:gunzip -c %s")
	tm.unregisterTransformation(".*\\.gz");
	TEST_EQUAL(tm.findTransformation("asd"), "")
	TEST_EQUAL(tm.findTransformation("asdasd"), "")
	TEST_EQUAL(tm.findTransformation("asd.gz"), "")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
