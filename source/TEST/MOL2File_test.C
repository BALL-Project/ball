// $Id: MOL2File_test.C,v 1.6 2001/07/07 02:51:03 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/MATHS/vector3.h>

///////////////////////////

START_TEST(MOL2File, "$Id: MOL2File_test.C,v 1.6 2001/07/07 02:51:03 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


MOL2File* mf = 0;
CHECK(MOL2File::MOL2File())
	mf = new MOL2File;
	TEST_NOT_EQUAL(mf, 0)
RESULT


CHECK(MOL2File::~MOL2File())
	delete mf;
RESULT


CHECK(MOL2File::read(System& system))
	MOL2File f("data/AAG.mol2");
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 30)
	TEST_EQUAL(system.countResidues(), 3)
	Size number_of_bonds = 0;
	Atom::BondIterator bond_it;
	AtomIterator atom_it;
	BALL_FOREACH_BOND(system, atom_it, bond_it)
	{
		number_of_bonds++;
	}
	TEST_EQUAL(number_of_bonds, 29)
RESULT


CHECK(MOL2File::MOL2File(const String& filename, File::OpenMode open_mode))
	MOL2File f("data/AAG.mol2", File::IN);
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 30)
	TEST_EQUAL(system.countResidues(), 3)
	Size number_of_bonds = 0;
	Atom::BondIterator bond_it;
	AtomIterator atom_it;
	BALL_FOREACH_BOND(system, atom_it, bond_it)
	{
		number_of_bonds++;
	}
	TEST_EQUAL(number_of_bonds, 29)

	// writing is tested below...
RESULT


CHECK(MOL2File::write(const System& system))
  Molecule* m = new Molecule;
	m->setName("MOL");
	System S;
	S.setName("SYSTEM");
	S.insert(*m);
	Atom* a1 = new Atom();
	Atom* a2 = new Atom();
	m->insert(*a1);
	m->insert(*a2);

	a1->setName("A1");
	a1->setElement(PTE[Element::N]);
	a1->setCharge(0.5);
	a1->setPosition(Vector3(0.1, 0.2, 0.3));

	a2->setName("A2");
	a2->setElement(PTE[Element::O]);
	a2->setCharge(-0.5);
	a2->setPosition(Vector3(0.5, 0.6, 0.7));
	
	a1->createBond(*a2);
	a1->getBond(*a2)->setOrder(Bond::ORDER__DOUBLE);
	

	String filename;
	NEW_TMP_FILE(filename)
	MOL2File f(filename, File::OUT);
	f.write(S);
	f.close();
	
	TEST_FILE(filename.c_str(), "data/MOL2File_test.mol2", true)
RESULT


CHECK(MOL2File::MOL2File& operator >> (System& system))
  MOL2File f("data/AAG.mol2");
	System S;
	f >> S;
	f.close();
	TEST_EQUAL(S.countAtoms(), 30)
RESULT

CHECK(MOL2File::MOL2File& operator << (const System& system))
  Molecule* m = new Molecule;
	m->setName("MOL");
	System S;
	S.setName("SYSTEM");
	S.insert(*m);
	Atom* a1 = new Atom();
	Atom* a2 = new Atom();
	m->insert(*a1);
	m->insert(*a2);

	a1->setName("A1");
	a1->setElement(PTE[Element::N]);
	a1->setCharge(0.5);
	a1->setPosition(Vector3(0.1, 0.2, 0.3));

	a2->setName("A2");
	a2->setElement(PTE[Element::O]);
	a2->setCharge(-0.5);
	a2->setPosition(Vector3(0.5, 0.6, 0.7));
	
	a1->createBond(*a2);
	a1->getBond(*a2)->setOrder(Bond::ORDER__DOUBLE);
	

	String filename;
	NEW_TMP_FILE(filename)
	MOL2File f(filename, File::OUT);
	f << S;	
	f.close();
	
	TEST_FILE(filename.c_str(), "data/MOL2File_test.mol2", true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
