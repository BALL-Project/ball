// $Id: HINFile_test.C,v 1.10 2001/05/22 20:26:54 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(HINFile, "$Id: HINFile_test.C,v 1.10 2001/05/22 20:26:54 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HINFile* ptr;

CHECK(HINFile::HINFile())
	ptr = new HINFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(HINFile::~HINFile())
  delete ptr;
RESULT

HINFile hin;

CHECK(HINFile::HINFile(const String& filename, File::OpenMode open_mode = File::IN))
  hin = HINFile("data/HINFile_test.hin");
  TEST_EQUAL(hin.isValid(), true)
RESULT

System system;
CHECK(HINFile::read(System& system))
  hin.read(system);
	hin.reopen();
	Vector3 position(0.59038, -0.410275, -0.860515);
  TEST_EQUAL(hin.isValid(), true)
  TEST_EQUAL(system.countAtoms(), 648)
  TEST_EQUAL(system.countMolecules(), 216)
	TEST_EQUAL(system.getAtom(0)->getName(), "O")
  TEST_EQUAL(system.getAtom(0)->getElement(), PTE["O"])
	TEST_REAL_EQUAL(system.getAtom(0)->getCharge(), -0.834)
  TEST_EQUAL(system.getAtom(0)->getPosition(), position)
  TEST_REAL_EQUAL(system.getAtom(0)->getRadius(), 1.4)
  TEST_EQUAL(system.getAtom(0)->countBonds(), 2) 

  TEST_NOT_EQUAL(system.getAtom(1)->getRadius(), 0)
RESULT

CHECK(HINFile::write(const System& system))
  String filename;
  NEW_TMP_FILE(filename)
  HINFile hin2(filename, std::ios::out);
	hin2.write(system);
  TEST_FILE(filename.c_str(), "data/HINFile_test2.hin", true)
RESULT

CHECK(HINFile::HINFile& operator >> (System& system))
	System system2;
  hin >> system2;
	TEST_EQUAL(system.countAtoms(), system2.countAtoms())
RESULT


CHECK(HINFile::HINFile& operator << (const System& system))
  String filename;
  NEW_TMP_FILE(filename)
  HINFile hin2(filename, std::ios::out);
  hin2 << system;
  TEST_FILE(filename.c_str(), "data/HINFile_test2.hin", true)
RESULT


CHECK(HINFile::hasPeriodicBoundary() const )
	TEST_EQUAL(hin.hasPeriodicBoundary(), true)
RESULT


CHECK(HINFile::getPeriodicBoundary() const )
	Box3 box3(-9.35068, -9.35068, -9.35068, 9.35068, 9.35068, 9.35068);
	TEST_EQUAL(hin.getPeriodicBoundary(), box3)
RESULT


CHECK(HINFile::getTemperature() const )
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
