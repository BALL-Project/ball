// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/atomBijection.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/FORMAT/HINFile.h>


///////////////////////////


using namespace BALL;
String dumpBijection(AtomBijection& ab)
{
	String s;
	AtomBijection::iterator it(ab.begin());
	for (; it != ab.end(); ++it)
	{
		s += " - " + it->first->getFullName() + " - " + it->second->getFullName() + "\n";
	}
	return s;
}


START_TEST(AtomBijection)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


PRECISION(0.001)
AtomBijection* ab_ptr = 0;
CHECK(AtomBijection())
	ab_ptr = new AtomBijection;
	TEST_NOT_EQUAL(ab_ptr, 0)
RESULT

CHECK(~AtomBijection())
	delete ab_ptr;
RESULT

CHECK(double calculateRMSD() const)
	Atom a1;
	Atom a2;
	a2.setPosition(Vector3(0.0, 0.0, 0.0));
	a1.setPosition(Vector3(1.0, 0.0, 0.0));
	AtomBijection ab;
	double rmsd = ab.calculateRMSD();
	TEST_EQUAL(ab.size(), 0)
	TEST_EQUAL(rmsd, 0.0)
	ab.push_back(AtomBijection::AtomPair(&a1, &a2));
	TEST_EQUAL(ab.size(), 1)
	rmsd = ab.calculateRMSD();
	TEST_EQUAL(rmsd, 1.0);
	a1.setPosition(Vector3(1.0, 0.0, 0.0));
	rmsd = ab.calculateRMSD();
	TEST_EQUAL(rmsd, 1.0);
	a1.setPosition(Vector3(0.0, 2.0, 0.0));
	rmsd = ab.calculateRMSD();
	TEST_EQUAL(rmsd, 2.0);
	a1.setPosition(Vector3(1.0, 1.0, 1.0));
	rmsd = ab.calculateRMSD();
	TEST_REAL_EQUAL(rmsd, sqrt((float)3.));
RESULT

Molecule benzene;
HINFile f(BALL_TEST_DATA_PATH(benzene.hin));
f >> benzene;
f.close();

Molecule pyrrole;
f.open(BALL_TEST_DATA_PATH(pyrrole.hin));
f >> pyrrole;
f.close();

Molecule indole;
f.open(BALL_TEST_DATA_PATH(indole.hin));
f >> indole;
f.close();

CHECK(AtomBijection(AtomContainer& A, AtomContainer& B))
	AtomBijection ab(indole, benzene);
	STATUS("Bijection: \n" << dumpBijection(ab))
	TEST_EQUAL(ab.size(), 12)
	double rmsd = ab.calculateRMSD();
	TEST_REAL_EQUAL(rmsd, 14.3947)
RESULT

CHECK(AtomBijection(AtomContainer& A, AtomContainer& B,  bool limit_to_selection))
	AtomIterator at = benzene.beginAtom();
	at->select();
	(++at)->select();
	(++at)->select();
	(++at)->select();
	//((benzene.beginAtom())++)->select();
	AtomBijection ab(indole, benzene, true);
	STATUS("Bijection: \n" << dumpBijection(ab))
	TEST_EQUAL(ab.size(), 4)
	double rmsd = ab.calculateRMSD();
	TEST_REAL_EQUAL(rmsd, 14.3404)
	(++at)->select();
	(++at)->select();
RESULT

CHECK(Size assignBackboneAtoms(AtomContainer& A, AtomContainer& B))
  // ???
RESULT

CHECK(Size assignByName(AtomContainer& A, AtomContainer& B))
	AtomBijection ab;
	ab.assignByName(indole, benzene);
	STATUS("Bijection: \n" << dumpBijection(ab))
	TEST_EQUAL(ab.size(), 12)
	double rmsd = ab.calculateRMSD();
	TEST_REAL_EQUAL(rmsd, 14.3947)
RESULT

CHECK(Size assignByName(AtomContainer& A, AtomContainer& B, bool limit_to_selection))
	AtomBijection ab;
	ab.assignByName(indole, benzene, true);
	STATUS("Bijection: \n" << dumpBijection(ab))
	TEST_EQUAL(ab.size(), 6)
	double rmsd = ab.calculateRMSD();
	TEST_REAL_EQUAL(rmsd, 14.3332)
RESULT


CHECK(Size assignCAlphaAtoms(AtomContainer& A, AtomContainer& B))
  // ???
RESULT

CHECK(Size assignTrivial(AtomContainer& A, AtomContainer& B))
	AtomBijection ab;
	ab.assignTrivial(indole, benzene);
	STATUS("Bijection: \n" << dumpBijection(ab))
	TEST_EQUAL(ab.size(), 12)
	double rmsd = ab.calculateRMSD();
	TEST_REAL_EQUAL(rmsd, 14.1103)
RESULT

CHECK (Size assignAtomsByProperty(AtomContainer& A, AtomContainer& B))
	AtomIterator at = benzene.beginAtom();
	at->setProperty("ATOMBIJECTION_RMSD_SELECTION", true);
	(++at)->setProperty("ATOMBIJECTION_RMSD_SELECTION", true);
	(++at)->setProperty("ATOMBIJECTION_RMSD_SELECTION", true);
	(++at)->setProperty("ATOMBIJECTION_RMSD_SELECTION", true);

	AtomBijection ab;
	ab.assignAtomsByProperty(indole, benzene);
	STATUS("Bijection: \n" << dumpBijection(ab))
	TEST_EQUAL(ab.size(), 4)
	double rmsd = ab.calculateRMSD();
	TEST_REAL_EQUAL(rmsd,  14.3404)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
