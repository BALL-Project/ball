// $Id: NMRStarFile_test.C,v 1.2 2000/09/18 16:46:57 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/NMRStarFile.h>
///////////////////////////

using namespace BALL;

START_TEST(String,"$Id: NMRStarFile_test.C,v 1.2 2000/09/18 16:46:57 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK(NMRStarFile::NMRStarFile(filename))
	NMRStarFile rs("data/bmr4318.str");
	TEST_EQUAL(rs.getData().size(), 1)
	TEST_EQUAL(rs.getNumberOfAtoms(), 1914)
	if (rs.getData().size() == 1 && rs.getNumberOfAtoms() == 1914)
	{
		TEST_EQUAL(rs.getData()[0]->atomData[1913]->atomName, "N")
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
