// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/KCFFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/MATHS/vector3.h>

///////////////////////////

START_TEST(KCFFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


KCFFile* mf = 0;
CHECK(KCFFile() throw())
	mf = new KCFFile;
	TEST_NOT_EQUAL(mf, 0)
RESULT


CHECK(~KCFFile() throw())
	delete mf;
RESULT


CHECK(bool read(System& system) throw(Exception::ParseError))
	KCFFile f(BALL_TEST_DATA_PATH(KCFFile_test1.kcf));
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 31 + 44 + 44)
	TEST_EQUAL(system.countBonds(), 33 + 48 + 48)
	TEST_EQUAL(system.countMolecules(), 3)
	ABORT_IF(system.countAtoms() == 0)
	Atom& atom = *system.beginAtom();
	TEST_REAL_EQUAL(atom.getPosition().x,  7.4091)
	TEST_REAL_EQUAL(atom.getPosition().y, -11.6324)
	TEST_REAL_EQUAL(atom.getPosition().z,  0.0000)
	system.clear();
	KCFFile f3(BALL_TEST_DATA_PATH(Selectable_test.txt));
	bool result = f3.read(system);
	TEST_EQUAL(result, false)
RESULT


CHECK(KCFFile(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	KCFFile f(BALL_TEST_DATA_PATH(KCFFile_test1.kcf), std::ios::in);
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 31 + 44 + 44)
	TEST_EQUAL(system.countMolecules(), 3)
	TEST_EQUAL(system.countBonds(), 33 + 48 + 48)
	TEST_EXCEPTION(Exception::FileNotFound, KCFFile f2("KCFFile_test_which_does_not_exist.kcf"))
RESULT


CHECK(bool write(const System& system) throw(File::CannotWrite))
  Molecule* m = new Molecule;
	m->setName("MOL");
	System S;
	S.setName("SYSTEM");
	S.insert(*m);
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	m->insert(*a1);
	m->insert(*a2);

	a1->setName("A1");
	a1->setElement(PTE[Element::N]);
	a1->setTypeName("N1a");
	a1->setCharge(0.5);
	a1->setPosition(Vector3(0.1, 0.2, 0.3));

	a2->setName("A2");
	a2->setElement(PTE[Element::O]);
	a2->setCharge(-0.5);
	a2->setTypeName("O2");
	a2->setPosition(Vector3(0.5, 0.6, 0.7));
	
	a1->createBond(*a2);
	a1->getBond(*a2)->setOrder(Bond::ORDER__DOUBLE);
	
	String filename;
	NEW_TMP_FILE(filename)
	KCFFile f(filename, std::ios::out);
	f.write(S);
	f.close();
	
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(KCFFile_test2.kcf))

	KCFFile f2(BALL_TEST_DATA_PATH(../KCFFile_test.C), std::ios::in);
	TEST_EXCEPTION(File::CannotWrite, f2.write(S))

	// Make sure we can read that stuff back again and get the same result
	KCFFile f3(filename);
	System S2;
	f3 >> S2;
	f3.close();

	TEST_EQUAL(S2.countAtoms(), S.countAtoms())
	TEST_EQUAL(S2.countMolecules(), S.countMolecules())
	TEST_EQUAL(S2.countBonds(), S.countBonds())

	AtomConstIterator ai1(S.beginAtom());
	AtomConstIterator ai2(S2.beginAtom());
	for (; +ai1 && +ai2; ++ai1, ++ai2)
	{
		TEST_REAL_EQUAL(ai2->getPosition().x, ai1->getPosition().x)
		TEST_REAL_EQUAL(ai2->getPosition().y, ai1->getPosition().y)
		TEST_REAL_EQUAL(ai2->getPosition().z, 0.0) // z-coordinates are not stored!
		TEST_EQUAL(ai2->getTypeName(), ai1->getTypeName())
		TEST_EQUAL(ai2->getElement(), ai1->getElement())
	}
RESULT


CHECK(Molecule* read() throw(Exception::ParseError))
  KCFFile f(BALL_TEST_DATA_PATH(KCFFile_test1.kcf));
	Molecule* m = f.read();
	f.close();
	TEST_EQUAL(m->countAtoms(), 31)
	TEST_EQUAL(m->countBonds(), 33)
	delete m;
RESULT

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
a1->setTypeName("N1a");
a1->setCharge(0.5);
a1->setPosition(Vector3(0.1, 0.2, 0.3));

a2->setName("A2");
a2->setElement(PTE[Element::O]);
a2->setTypeName("O2");
a2->setCharge(-0.5);
a2->setPosition(Vector3(0.5, 0.6, 0.7));

a1->createBond(*a2);
a1->getBond(*a2)->setOrder(Bond::ORDER__DOUBLE);

String filename;

CHECK([EXTRA]KCFFile::KCFFile& operator << (const System& system))
	NEW_TMP_FILE(filename)
	KCFFile f(filename, std::ios::out);
	f << S;	
	f.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(KCFFile_test2.kcf))
RESULT

CHECK(bool write(const Molecule& molecule) throw(File::CannotWrite))
	NEW_TMP_FILE(filename)
	KCFFile f(filename, std::ios::out);
	f.write(*m);
	f.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(KCFFile_test2.kcf))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
