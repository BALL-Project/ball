// $Id: NMRStarFile_test.C,v 1.1 2000/09/13 01:03:54 amoll Exp $

//#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/NMRStarFile.h>
///////////////////////////

//START_TEST(String,"$Id: NMRStarFile_test.C,v 1.1 2000/09/13 01:03:54 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using std::string;

int main(int argc, char *argv[])
{/*
	if (strcmp(argv[1], "/KM/comp-bio/data/oliver/BMRB/bmr4089.str") == 0)
	{
		exit(1);
	}*/
	cout << argv[1] << "            ";
	NMRStarFile rs(argv[1]);
	cout << rs.getNumberOfAtoms() << endl;
}

/*
CHECK(NMRStarFile::NMRStarFile(filename))
	NMRStarFile rs("NMR/bmr1000.str");
RESULT*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//END_TEST
