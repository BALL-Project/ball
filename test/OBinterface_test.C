// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

///////////////////////////

START_TEST(OBinterface)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(1e-4)

/// This Test tests the interface between OpenBabel and BALL.
/// A Molecule is to be converted from BALL to OpenBabel and back to BALL. In the resulting molecule, all atoms must still have the correct position and all bonds must have the correct order (incl. aromaticity)

CHECK(OpenBabel-interface)
	MOL2File f(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMPTRB10_arom_sol0.mol2), std::ios::in);

	Molecule* original_mol = f.read();
	OpenBabel::OBMol* obmol = MolecularSimilarity::createOBMol(*original_mol);
	Molecule* new_mol = MolecularSimilarity::createMolecule(*obmol);
	delete obmol;

	TEST_EQUAL(new_mol->countAtoms(), original_mol->countAtoms())

	AtomIterator a_it1 = original_mol->beginAtom();
	for (AtomIterator a_it2 = new_mol->beginAtom(); +a_it1 && +a_it2; a_it1++, a_it2++)
	{
		TEST_EQUAL(a_it2->getElement().getSymbol(), a_it1->getElement().getSymbol())
		TEST_EQUAL(a_it2->countBonds(), a_it1->countBonds())
		Vector3& pos1 = a_it1->getPosition();
		Vector3& pos2 = a_it2->getPosition();
		TEST_REAL_EQUAL(pos2.x, pos1.x)
		TEST_REAL_EQUAL(pos2.y, pos1.y)
		TEST_REAL_EQUAL(pos2.z, pos1.z)

		Atom::BondIterator b_it1 = a_it1->beginBond();
		for (Atom::BondIterator b_it2 = a_it2->beginBond(); +b_it1 && +b_it2; b_it1++, b_it2++)
		{
			TEST_EQUAL(b_it2->getOrder(), b_it1->getOrder())
			Atom* partner1 = b_it1->getPartner(*a_it1);
			Atom* partner2 = b_it2->getPartner(*a_it2);
			Vector3& pos1 = partner1->getPosition();
			Vector3& pos2 = partner2->getPosition();
			TEST_REAL_EQUAL(pos2.x, pos1.x)
			TEST_REAL_EQUAL(pos2.y, pos1.y)
			TEST_REAL_EQUAL(pos2.z, pos1.z)
		}
	}

	delete original_mol;
	delete new_mol;
RESULT


END_TEST
