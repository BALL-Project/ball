
// $Id: SnapShotManager_test.C,v 1.2 2001/07/15 17:32:41 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MOLMEC/COMMON/snapShot.h>

///////////////////////////

START_TEST(SnapShotManager, "$Id: SnapShotManager_test.C,v 1.2 2001/07/15 17:32:41 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

CHECK(SnapshotManager::SnapshotManager())
	// BAUSTELLE
RESULT

CHECK(SnapshotManager::SnapshotManager(const System* my_system, const
ForceField* my_force_field, const String& my_snapshot_file, bool
overwrite))
	// BAUSTELLE
RESULT

CHECK(SnapshotManager::SnapShotManager	(const System* my_system, const
ForceField* my_force_field, const Options& my_options, const String&
filename, bool overwrite = true))
	// BAUSTELLE
RESULT

CHECK(SnapShotManager(const SnapShotManager& manager))
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::~SnapShotManager())
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::setup(const ))
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::operator = (const SnapShotManager& manager))
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::clear())
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::isValid())
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::setFlushToDiskFrequency(Size number))
	// BAUSTELLE
RESULT

CHECK(SnapShotManager:: getFlushToDiskFrequency())
	// BAUSTELLE
RESULT

CHECK(takeSnapShot())
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::flushToDisk())
	// BAUSTELLE
RESULT

CHECK(SnapShotManager::getNumberOfSnapShots())
	// BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
