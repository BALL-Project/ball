// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
<<<<<<< VersionInfo_test.C
// $Id: VersionInfo_test.C,v 1.4.14.1 2007/03/25 21:49:11 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

=======
// $Id: VersionInfo_test.C,v 1.4.14.1 2007/03/25 21:49:11 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

>>>>>>> 1.3.10.1
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/COMMON/version.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

<<<<<<< VersionInfo_test.C
START_TEST(VersionInfo, "$Id: VersionInfo_test.C,v 1.4.14.1 2007/03/25 21:49:11 oliver Exp $")
=======
START_TEST(VersionInfo, "$Id: VersionInfo_test.C,v 1.4.14.1 2007/03/25 21:49:11 oliver Exp $")
>>>>>>> 1.3.10.1

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(static const char* getVersion() throw())
	TEST_NOT_EQUAL(VersionInfo::getVersion(), 0)
	STATUS("Release string: " << VersionInfo::getVersion())
	TEST_EQUAL(String(VersionInfo::getVersion()).hasPrefix(BALL_RELEASE_STRING), true)
RESULT

CHECK(static int getMajorRevision() throw(Exception::InvalidFormat))
	// just to call the method
	TEST_NOT_EQUAL(VersionInfo::getMajorRevision(), -1)
RESULT

CHECK(static int getMinorRevision() throw(Exception::InvalidFormat))
	// just to call the method
	TEST_NOT_EQUAL(VersionInfo::getMinorRevision(), -1)
	String rel(VersionInfo::getMajorRevision());
	rel += '.';
	rel += String(VersionInfo::getMinorRevision());
	
	// test whether the release string really starts
	// with the numbers on getMinor/getMajorRevision
	TEST_EQUAL(String(BALL_RELEASE_STRING).hasPrefix(rel), true)
RESULT

CHECK(static Type getType() throw())
	TEST_NOT_EQUAL(VersionInfo::getType(), VersionInfo::UNKNOWN)
RESULT
 											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
