// $Id: MOL2File_test.C,v 1.1 2000/05/15 19:17:06 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/MOL2File.h>

///////////////////////////

START_TEST(MOL2File, "$Id: MOL2File_test.C,v 1.1 2000/05/15 19:17:06 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


CHECK(MOL2File::MOL2File())
  //BAUSTELLE
RESULT


CHECK(MOL2File::~MOL2File())
  //BAUSTELLE
RESULT


CHECK(MOL2File::MOL2File(const String& filename, File::OpenMode open_mode))
  //BAUSTELLE
RESULT


CHECK(MOL2File::write(const System& system))
  //BAUSTELLE
RESULT


CHECK(MOL2File::read(System& system))
  //BAUSTELLE
RESULT


CHECK(MOL2File::MOL2File& operator >> (System& system))
  //BAUSTELLE
RESULT

CHECK(MOL2File::MOL2File& operator << (const System& system))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
