// $Id: HINFile_test.C,v 1.6 2001/04/03 14:24:58 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(HINFile, "$Id: HINFile_test.C,v 1.6 2001/04/03 14:24:58 amoll Exp $")

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
RESULT

CHECK(HINFile::read(System& system))
	System system;
  hin.read(system);
RESULT

CHECK(HINFile::write(const System& system))
  //BAUSTELLE
RESULT

CHECK(HINFile::HINFile& operator >> (System& system))
  //BAUSTELLE
RESULT


CHECK(HINFile::HINFile& operator << (const System& system))
  //BAUSTELLE
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
