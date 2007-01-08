// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/QSAR/connectivityBase.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////
START_TEST(ConnectivityBase, "")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile("data/descriptors_test.sdf");
System S;
infile >> S;
infile.close();

Molecule * molecule;
Size limit = S.countMolecules();

CHECK(ConnectivityBase)	

	ConnectivityBase cb;
	S.apply(cb);

	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("BalabanIndexJ"), true);
	}
		
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
