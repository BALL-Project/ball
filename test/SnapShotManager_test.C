// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(SnapShotManager)

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

CHECK(full_test)
	System system;
	PDBFile pfile(BALL_TEST_DATA_PATH(DCDFile_test.pdb));
	Size nr_of_atoms = system.countAtoms();
	pfile.read(system);
	system.getAtom(0)->setPosition(Vector3(1,2,1111));
	DCDFile dcd(BALL_TEST_DATA_PATH(DCD_test2.dcd), std::ios::in);
	SnapShotManager sm(&system, 0, &dcd);
	sm.applyFirstSnapShot();
	TEST_EQUAL(system.getAtom(0)->getPosition(), Vector3(11.936, 104.294, 10.149))
	sm.applySnapShot(2);
	sm.applyFirstSnapShot();
	sm.applySnapShot(1);
	sm.applyFirstSnapShot();
	sm.applySnapShot(0);
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
