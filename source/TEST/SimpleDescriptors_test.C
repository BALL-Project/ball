// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SimpleDescriptors_test.C,v 1.3 2002/02/27 12:24:55 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(SimpleDescriptors, "$Id: SimpleDescriptors_test.C,v 1.3 2002/02/27 12:24:55 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile("data/SDFile_test1.sdf");
System S;
infile >> S;

Position i;
MoleculeIterator molecule;
CHECK(NumberOfBonds)	
	NumberOfBonds nb;
	S.apply(nb);
	i = 0;
	Size results[11] = {42, 92, 52, 29, 49, 20, 19, 43, 25, 18, 139};
	molecule = S.beginMolecule();
	for (; +molecule; ++molecule, ++i)
	{
		TEST_EQUAL(molecule->hasProperty("NumberOfBonds"), true)
		if (molecule->hasProperty("NumberOfBonds"))
		{
			Size number_of_bonds = (Size)molecule->getProperty("NumberOfBonds").getFloat();
			ABORT_IF(i > 10)
			TEST_EQUAL(number_of_bonds, results[i])
		}
	}
RESULT


// assign vdW radii...
AtomIterator ai;
for (ai = S.beginAtom(); ai != S.endAtom(); ++ai)
{
  ai->setRadius(ai->getElement().getVanDerWaalsRadius());
}

CHECK(PolarSurfaceArea)	
	PolarSurfaceArea psa;
	S.apply(psa);
	i = 0;
	float results[11] = {149.17, 447.974, 74.9747, 153.708, 121.778, 89.2721, 220.392, 138.404, 30.8643, 115.156, 108.148};

	PRECISION(0.0005)
	molecule = S.beginMolecule();
	for (; +molecule; ++molecule, ++i)
	{
		TEST_EQUAL(molecule->hasProperty("PolarSurfaceArea"), true)
		if (molecule->hasProperty("PolarSurfaceArea"))
		{
			float psa = molecule->getProperty("PolarSurfaceArea").getFloat();
			ABORT_IF(i >= 11)
			TEST_REAL_EQUAL(psa, results[i])
		}
	}
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
