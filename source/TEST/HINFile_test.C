// $Id: HINFile_test.C,v 1.3 2001/03/11 21:03:07 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(HINFile, "$Id: HINFile_test.C,v 1.3 2001/03/11 21:03:07 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HINFile* ptr;

CHECK(HINFile::HINFile())
	ptr = new HINFile;
	CHECK_NOT_EQUAL(ptr, 0)
RESULT


CHECK(HINFile::~HINFile())
  delete ptr;
RESULT


CHECK(HINFile::HINFile(const String& filename, File::OpenMode open_mode = File::IN))
  //BAUSTELLE
RESULT


CHECK(HINFile::write(const System& system))
  //BAUSTELLE
RESULT


CHECK(HINFile::read(System& system))
  //BAUSTELLE
RESULT


CHECK(HINFile::HINFile& operator >> (System& system))
  //BAUSTELLE
RESULT


CHECK(HINFile::HINFile& operator << (const System& system))
  //BAUSTELLE
RESULT


CHECK(HINFile::hasPeriodicBoundary() const )
  HINFile box("data/water.hin");
	TEST_EQUAL(box.hasPeriodicBoundary(), true)
RESULT


CHECK(HINFile::getPeriodicBoundary() const )
  HINFile box("data/water.hin");
RESULT


CHECK(HINFile::getTemperature() const )
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
