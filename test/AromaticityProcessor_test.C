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

// BUG-539: a fused polycyclic aromatic system (COD molecule 73045, the #539
// reproducer) must have ALL rings of the aromatic system assigned the aromatic
// bond order. Against the unfixed processor only two of the four fused rings are
// aromatized. The aromatic ring system spans atoms 2,3,4,5,12,13,14,15,16,17 plus
// 6,7,8,9,10,11 form an adjacent benzene ring fused via the 5-12 / 5-6 / 6-10 etc.
// Count the aromatic bonds the processor assigns and compare to the fully
// aromatized reference.
CHECK(AromaticityProcessor fused-ring system [BUG-539])
	SDFile bug539(BALL_TEST_DATA_PATH(AromaticityProcessor_test_BUG539.sdf));
	System bug_system;
	bug539 >> bug_system;
	bug539.close();

	AromaticityProcessor ap539;
	bug_system.apply(ap539);

	Molecule* m = bug_system.getMolecule(0);
	TEST_NOT_EQUAL(m, 0)

	Size num_aro_bonds = 0;
	AtomConstIterator a_it = m->beginAtom();
	Atom::BondConstIterator b_it = a_it->beginBond();
	BALL_FOREACH_BOND(*m, a_it, b_it)
	{
		if (b_it->getOrder() == Bond::ORDER__AROMATIC)
		{
			++num_aro_bonds;
		}
	}
	STATUS("BUG-539 aromatic bond count = " << num_aro_bonds)

	// COD molecule 73045 is a Br-substituted pyrene (4 fused 6-rings sharing a
	// common pi system). Fully aromatized, every bond inside the pyrene core
	// carries the aromatic order. Pyrene has 16 ring carbons and 19 ring bonds.
	TEST_EQUAL(num_aro_bonds, 19)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
