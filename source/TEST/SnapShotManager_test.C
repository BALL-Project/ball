
// $Id: SnapShotManager_test.C,v 1.3 2001/12/30 13:29:01 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MOLMEC/COMMON/snapShot.h>

///////////////////////////

START_TEST(SnapShotManager, "$Id: SnapShotManager_test.C,v 1.3 2001/12/30 13:29:01 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

CHECK(SnapshotManager::SnapshotManager())
	// ?????
RESULT

CHECK(SnapshotManager::SnapshotManager(const System* my_system, const
ForceField* my_force_field, const String& my_snapshot_file, bool
overwrite))
	// ?????
RESULT

CHECK(SnapshotManager::SnapShotManager	(const System* my_system, const
ForceField* my_force_field, const Options& my_options, const String&
filename, bool overwrite = true))
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
