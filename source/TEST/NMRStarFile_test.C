// $Id: NMRStarFile_test.C,v 1.6 2000/10/03 02:00:37 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/NMRStarFile.h>
///////////////////////////

using namespace BALL;

START_TEST(String,"$Id: NMRStarFile_test.C,v 1.6 2000/10/03 02:00:37 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK(NMRStarFile::NMRStarFile(filename))
	NMRStarFile rs("data/bmr4318.str");
	TEST_EQUAL(rs.getData().size(), 1)
	TEST_EQUAL(rs.getNumberOfAtoms(), 1914)
	if (rs.getData().size() == 1 && rs.getNumberOfAtoms() == 1914)
	{
		TEST_EQUAL(rs.getData()[0]->atom_data[1913]->atom_name, "N")
	}

	/*
	for (Position i = 0;  i < rs.getData()[0]->atom_data.size(); i++)
	{
		cout << *(rs.getData()[0]->atom_data[i]) << endl;
	}
	*/
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
