// $Id: FragmentDB_test.C,v 1.8 2001/12/13 02:43:42 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>

using namespace BALL;
///////////////////////////

START_TEST(Fragment, "$Id: FragmentDB_test.C,v 1.8 2001/12/13 02:43:42 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

FragmentDB db;
CHECK(getResidue(const String&))
	const Residue* res = db.getResidue("GLY");
	TEST_NOT_EQUAL(res, 0)
	if (res != 0)
	{
		TEST_EQUAL(res->isNTerminal(), false)
		TEST_EQUAL(res->isCTerminal(), false)
	}
	
	// check the atoms
	AtomConstIterator atom_it = res->beginAtom();
	Position i = 0;
	for (; +atom_it; ++atom_it, ++i)
	{
		switch (i)
		{
			case 5: 
				TEST_EQUAL(atom_it->getName(), "N")
				TEST_EQUAL(atom_it->getElement().getSymbol(), "N")
				TEST_EQUAL(atom_it->countBonds(), 2)
				break;
			case 2: 
				TEST_EQUAL(atom_it->getName(), "H")
				TEST_EQUAL(atom_it->getElement().getSymbol(), "H")
				TEST_EQUAL(atom_it->countBonds(), 1)
				break;
			case 4: 
				TEST_EQUAL(atom_it->getName(), "CA")
				TEST_EQUAL(atom_it->getElement().getSymbol(), "C")
				TEST_EQUAL(atom_it->countBonds(), 4)
				break;
			case 0: 
				TEST_EQUAL(atom_it->getName(), "1HA")
				TEST_EQUAL(atom_it->getElement().getSymbol(), "H")
				TEST_EQUAL(atom_it->countBonds(), 1)
				break;
			case 1: 
				TEST_EQUAL(atom_it->getName(), "2HA")
				TEST_EQUAL(atom_it->getElement().getSymbol(), "H")
				TEST_EQUAL(atom_it->countBonds(), 1)
				break;
			case 3: 
				TEST_EQUAL(atom_it->getName(), "C")
				TEST_EQUAL(atom_it->getElement().getSymbol(), "C")
				TEST_EQUAL(atom_it->countBonds(), 2)
				break;
			case 6: 
				TEST_EQUAL(atom_it->getName(), "O")
				TEST_EQUAL(atom_it->getElement().getSymbol(), "O")
				TEST_EQUAL(atom_it->countBonds(), 1)
				break;
			default:
				STATUS("Too many atoms")
				TEST_EQUAL(0, 1)
		}
	}
	
	// check the bonds
	Atom::BondConstIterator bond_it;
	i =0;
	BALL_FOREACH_BOND(*res, atom_it, bond_it)
	{
		switch (i)
		{
			case 0: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 1: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 2: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 3: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 4: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__AROMATIC) break;
			case 5: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			default:
				STATUS("Too many bonds")
				TEST_EQUAL(0, 1)
		}
		i++;
	}	
RESULT

CHECK(FragmentDB::BuildBondsProcessor::operator () )
	Residue res(*db.getResidue("GLY"));
	
	AtomIterator atom_it = res.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		atom_it->destroyBonds();
	}

	// build the bonds
	res.apply(db.build_bonds);
	
	// check the bonds
	Atom::BondIterator bond_it;
	Position i = 0;
	BALL_FOREACH_BOND(res, atom_it, bond_it)
	{
		switch (i)
		{
			case 0: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 1: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 2: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 3: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			case 4: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__AROMATIC) break;
			case 5: TEST_EQUAL(bond_it->getOrder(), Bond::ORDER__SINGLE) break;
			default:
				STATUS("Too many bonds")
				TEST_EQUAL(0, 1)
		}
		i++;
	}	

RESULT

CHECK(FragmentDB::BuildBondsProcessor::operator () / Tripeptide)
  HINFile infile("data/AlaGlySer.hin");
	System S;
	infile >> S;
	TEST_EQUAL(S.countAtoms(), 31)

	TEST_EQUAL(S.countResidues(), 3)
	ABORT_IF(S.countResidues() != 3)
				
	Atom::BondIterator bond_it;
	AtomIterator atom_it;
	ResidueIterator res_it = S.beginResidue();
	for (; +res_it; ++res_it)
	{
		Size number_of_atoms = 0;;
		BALL_FOREACH_BOND(*res_it, atom_it, bond_it)
		{
			number_of_atoms++;
			STATUS("  " << bond_it->getFirstAtom()->getFullName() << "-" << bond_it->getSecondAtom()->getFullName())
		}
		STATUS("Number of bonds in residue " << res_it->getName() << ": " << number_of_atoms)
	}

	atom_it = S.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		atom_it->destroyBonds();
	}

	// build the bonds
	S.apply(db.build_bonds);
	
	// check the bonds
	for (res_it = S.beginResidue(); +res_it; ++res_it)
	{
		Position i = 0;
		BALL_FOREACH_BOND(*res_it, atom_it, bond_it)
		{
			i++;
			STATUS("  " << bond_it->getFirstAtom()->getFullName() << "-" << bond_it->getSecondAtom()->getFullName())
		}	
		STATUS("Number of bonds in residue " << res_it->getName() << ": " << i)
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
