// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: VersionInfo_test.C,v 1.2 2002/02/27 12:25:03 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/COMMON/version.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

START_TEST(VersionInfo, "$Id: VersionInfo_test.C,v 1.2 2002/02/27 12:25:03 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


CHECK(const char* getVersion())
	TEST_NOT_EQUAL(VersionInfo::getVersion(), 0)
	TEST_EQUAL(String(VersionInfo::getVersion()).hasPrefix(BALL_RELEASE_STRING), true)
RESULT

CHECK(int getMajorRevision())
	// just to call the method
	TEST_NOT_EQUAL(VersionInfo::getMajorRevision(), -1)
RESULT

CHECK(int getMinorRevision())
	// just to call the method
	TEST_NOT_EQUAL(VersionInfo::getMinorRevision(), -1)
	String rel(VersionInfo::getMajorRevision());
	rel += '.';
	rel += String(VersionInfo::getMinorRevision());
	
	// test whether the release string really starts
	// with the numbers on getMinor/getMajorRevision
	TEST_EQUAL(String(BALL_RELEASE_STRING).hasPrefix(rel), true)
RESULT

CHECK(Type getType())
	TEST_NOT_EQUAL(VersionInfo::getType(), VersionInfo::UNKNOWN)
RESULT
 											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
