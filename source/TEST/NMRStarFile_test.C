// $Id: NMRStarFile_test.C,v 1.5 2000/09/24 14:52:02 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/NMRStarFile.h>
///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(String,"$Id: NMRStarFile_test.C,v 1.5 2000/09/24 14:52:02 oliver Exp $")

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

	for (int i = 0;  i < rs.getData()[0]->atomData.size(); i++)
	{
		cout << *(rs.getData()[0]->atomData[i]) << endl;
	}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
