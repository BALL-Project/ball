// $Id: SDFile_test.C,v 1.1 2001/12/18 01:20:13 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/MATHS/vector3.h>

///////////////////////////

START_TEST(SDFile, "$Id: SDFile_test.C,v 1.1 2001/12/18 01:20:13 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


SDFile* mf = 0;
CHECK(SDFile::SDFile())
	mf = new SDFile;
	TEST_NOT_EQUAL(mf, 0)
RESULT


CHECK(SDFile::~SDFile())
	delete mf;
RESULT


CHECK(SDFile::read(System& system))
	SDFile f("data/SDFile_test1.sdf");
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 23)
	TEST_EQUAL(system.countBonds(), 26)
RESULT


CHECK(SDFile::SDFile(const String& filename, File::OpenMode open_mode))
	SDFile f("data/SDFile_test1.sdf", File::IN);
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 23)
	TEST_EQUAL(system.countMolecules(), 1)
	TEST_EQUAL(system.countBonds(), 26)
RESULT


CHECK(SDFile::write(const System& system))
  Molecule* m = new Molecule;
	m->setName("SD");
	System S;
	S.setName("SYSTEM");
	S.insert(*m);
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
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
	SDFile f(filename, File::OUT);
	f.write(S);
	f.close();
	
	TEST_FILE(filename.c_str(), "data/SDFile_test2.sdf", true)
RESULT


CHECK(SDFile::SDFile& operator >> (System& system))
  SDFile f("data/SDFile_test1.sdf");
	System S;
	f >> S;
	f.close();
	TEST_EQUAL(S.countAtoms(), 23)
	TEST_EQUAL(S.countBonds(), 26)
	TEST_EQUAL(S.countMolecules(), 1)
RESULT

CHECK(SDFile::SDFile& operator << (const System& system))
  Molecule* m = new Molecule;
	m->setName("SD");
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
	SDFile f(filename, File::OUT);
	f << S;	
	f.close();
	
	TEST_FILE(filename.c_str(), "data/SDFile_test.sdf", true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
