// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////
START_TEST(AromaticityProcessor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile(BALL_TEST_DATA_PATH(descriptors_test.sdf));
System S;
infile >> S;
infile.close();

Molecule * molecule;
Size limit = S.countMolecules();

CHECK(AromaticityProcessor)	

	AromaticityProcessor ap;
	S.apply(ap);

	
	Size results[] = {0, 6, 0, 6, 6, 6, 0, 6, 0};
	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);

		// atoms
		Size num_aro = 0;
		AtomConstIterator a_it = molecule->beginAtom();
		for (;a_it!=molecule->endAtom();++a_it)
		{
			if(a_it->hasProperty("IsAromatic"))
			{
				if (a_it->getProperty("IsAromatic").getBool())
				{
					++num_aro;
				}
			}
		}
		TEST_EQUAL(num_aro, results[i])

		// bonds
		a_it = molecule->beginAtom();
		Atom::BondConstIterator b_it = a_it->beginBond();
		num_aro = 0;
		BALL_FOREACH_BOND(*molecule, a_it, b_it)
		{
			if (b_it->getOrder() == Bond::ORDER__AROMATIC)
			{
				++num_aro;
			}
		}
		TEST_EQUAL(num_aro, results[i])		
	}
		
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
