// $Id: FragmentDB_test.C,v 1.7 2001/07/15 17:32:39 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>

using namespace BALL;
///////////////////////////

START_TEST(Fragment, "$Id: FragmentDB_test.C,v 1.7 2001/07/15 17:32:39 amoll Exp $")

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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
