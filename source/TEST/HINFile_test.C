// $Id: HINFile_test.C,v 1.1 2000/05/27 09:16:18 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(HINFile, "$Id: HINFile_test.C,v 1.1 2000/05/27 09:16:18 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(HINFile::HINFile())
  //BAUSTELLE
RESULT


CHECK(HINFile::HINFile(const String& filename, File::OpenMode open_mode = std::ios::in))
  //BAUSTELLE
RESULT


CHECK(HINFile::~HINFile())
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
  //BAUSTELLE
RESULT


CHECK(HINFile::getPeriodicBoundary() const )
  //BAUSTELLE
RESULT


CHECK(HINFile::getTemperature() const )
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
