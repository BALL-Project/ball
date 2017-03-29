// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////
START_TEST(RingPerceptionProcessor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile(BALL_TEST_DATA_PATH(descriptors_test.sdf));
System S;
infile >> S;
infile.close();

Molecule * molecule;
Size limit = S.countMolecules();

CHECK(RingPerceptionProcessor)	

	RingPerceptionProcessor rpp;

	Size results_atoms[9] = {6, 6, 6, 6, 6, 13, 0, 6, 4};
	Size results_bonds[9] = {6, 6, 6, 6, 6, 14, 0, 6, 6};

	for (Size i = 0; i != limit; ++i)
	{
		molecule = S.getMolecule(i);
		molecule->apply(rpp);
		Size num_ringed = 0;

		// atoms
		AtomConstIterator a_it = molecule->beginAtom();
		for (; a_it != molecule->endAtom(); ++a_it)
		{
			if (a_it->getProperty("InRing").getBool())
			{		
				++num_ringed;
			}
		}
		TEST_EQUAL(num_ringed, results_atoms[i])

		// bonds
		num_ringed = 0;
		a_it = molecule->beginAtom();
		Atom::BondConstIterator b_it = a_it->beginBond();
		BALL_FOREACH_BOND(*molecule, a_it, b_it)
		{
			if (b_it->getProperty("InRing").getBool())
			{
				++num_ringed;
			}
		}
		TEST_EQUAL(num_ringed, results_bonds[i])
	}
		
RESULT

CHECK(DNA)
	// For some reason this does currently not work!
	// We found that the number of bonds was always lower than the number of
	// atoms in each AtomContainer: thus there could not be any rings.
	// HOWEVER: loading the file in BALLView and using "Count items"
	// always gave the correct result.
	PDBFile pdb(BALL_TEST_DATA_PATH(1BNA.pdb));
	System s2;
	pdb >> s2;

	RingPerceptionProcessor rpp;
	vector<vector<Atom*> > rings;
	for(AtomContainerIterator mit = s2.beginAtomContainer(); +mit; ++mit)
	{
		rings.clear();
		mit->apply(rpp);
		rpp.calculateSSSR(rings, *mit);
	}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
