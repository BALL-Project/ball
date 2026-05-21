// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/COMMON/version.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

START_TEST(VersionInfo)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// This test is intentionally VERSION-AGNOSTIC. Every expectation is derived
// at compile time from BALL_RELEASE_STRING, which is generated from the CMake
// PROJECT_VERSION (cmake/config.h.in -> BALL/CONFIG/config.h). A version bump
// (1.7.1 -> 1.7.2 -> 2.0.0 ...) therefore needs NO edit here. Do NOT add
// hardcoded-version assertions (e.g. TEST_EQUAL(getVersion(), "1.7.2")) —
// they would re-introduce exactly the per-release maintenance this test
// deliberately avoids.

CHECK(static const char* getVersion() throw())
	TEST_NOT_EQUAL(VersionInfo::getVersion(), 0)
	STATUS("Release string: " << VersionInfo::getVersion())
	TEST_EQUAL(String(VersionInfo::getVersion()).hasPrefix(BALL_RELEASE_STRING), true)
RESULT

CHECK(static int getMajorRevision())
	TEST_NOT_EQUAL(VersionInfo::getMajorRevision(), -1)
	// the major revision must equal the first dotted field of the release
	// string (derived, not hardcoded — stays correct across version bumps)
	String major_str(String(BALL_RELEASE_STRING).before("."));
	TEST_EQUAL(VersionInfo::getMajorRevision(), major_str.toInt())
RESULT

CHECK(static int getMinorRevision())
	TEST_NOT_EQUAL(VersionInfo::getMinorRevision(), -1)

	// accessors -> string: the release string starts with "<major>.<minor>"
	String rel(VersionInfo::getMajorRevision());
	rel += '.';
	rel += String(VersionInfo::getMinorRevision());
	TEST_EQUAL(String(BALL_RELEASE_STRING).hasPrefix(rel), true)

	// string -> accessors: the minor revision equals the second dotted field
	String after_major(String(BALL_RELEASE_STRING).after("."));
	String minor_str(after_major.before("."));
	TEST_EQUAL(VersionInfo::getMinorRevision(), minor_str.toInt())
RESULT

CHECK(static Type getType() throw())
	TEST_NOT_EQUAL(VersionInfo::getType(), VersionInfo::UNKNOWN)
RESULT
 											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
