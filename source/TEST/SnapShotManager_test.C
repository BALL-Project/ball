// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// $Id: SnapShotManager_test.C,v 1.5.32.1 2007/03/25 21:49:00 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MOLMEC/COMMON/snapShot.h>

///////////////////////////

START_TEST(SnapShotManager, "$Id: SnapShotManager_test.C,v 1.5.32.1 2007/03/25 21:49:00 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(SnapshotManager::SnapshotManager())
	// ?????
RESULT

CHECK(SnapshotManager::SnapshotManager(const System* my_system, const ForceField* my_force_field, const String& my_snapshot_file, bool overwrite))
	// ?????
RESULT

CHECK(SnapshotManager::SnapShotManager	(const System* my_system, const ForceField* my_force_field, const Options& my_options, const String& filename, bool overwrite = true))
	// ?????
RESULT

CHECK(SnapShotManager(const SnapShotManager& manager))
	// ?????
RESULT

CHECK(SnapShotManager::~SnapShotManager())
	// ?????
RESULT

CHECK(SnapShotManager::setup(const ))
	// ?????
RESULT

CHECK(SnapShotManager::operator = (const SnapShotManager& manager))
	// ?????
RESULT

CHECK(SnapShotManager::clear())
	// ?????
RESULT

CHECK(SnapShotManager::isValid())
	// ?????
RESULT

CHECK(SnapShotManager::setFlushToDiskFrequency(Size number))
	// ?????
RESULT

CHECK(SnapShotManager:: getFlushToDiskFrequency())
	// ?????
RESULT

CHECK(takeSnapShot())
	// ?????
RESULT

CHECK(SnapShotManager::flushToDisk())
	// ?????
RESULT

CHECK(SnapShotManager::getNumberOfSnapShots())
	// ?????
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
