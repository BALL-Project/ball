// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: KCFFile_test.C,v 1.2 2005/03/14 21:38:39 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

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

START_TEST(KCFFile, "$Id: KCFFile_test.C,v 1.2 2005/03/14 21:38:39 oliver Exp $")

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
	KCFFile f("data/KCFFile_test1.kcf");
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
	KCFFile f3("data/Selectable_test.txt");
	bool result = f3.read(system);
	TEST_EQUAL(result, false)
RESULT


CHECK(KCFFile(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	KCFFile f("data/KCFFile_test1.kcf", std::ios::in);
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
	KCFFile f(filename, std::ios::out);
	f.write(S);
	f.close();
	
	TEST_FILE_REGEXP(filename.c_str(), "data/KCFFile_test2kcf")

	KCFFile f2("KCFFile_test.C", std::ios::in);
	TEST_EXCEPTION(File::CannotWrite, f2.write(S))
RESULT


CHECK([EXTRA]KCFFile::KCFFile& operator >> (System& system))
  KCFFile f("data/KCFFile_test1.kcf");
	System S;
	f >> S;
	f.close();
	TEST_EQUAL(S.countAtoms(), 23)
	TEST_EQUAL(S.countBonds(), 26)
	TEST_EQUAL(S.countMolecules(), 1)

	KCFFile f2("data/KCFFile_test3.kcf");
	S.destroy();
	f2 >> S;
	TEST_EQUAL(S.countAtoms(), 49)
	TEST_EQUAL(S.countBonds(), 52)
	TEST_EQUAL(S.countMolecules(), 1)
	ABORT_IF(S.countAtoms() == 0)
	Atom& atom = *S.beginAtom();
	TEST_EQUAL(atom.getElement(), PTE[Element::C])
	TEST_REAL_EQUAL(atom.getPosition().x, -2.6970)
	TEST_REAL_EQUAL(atom.getPosition().y, -1.2710)
	TEST_REAL_EQUAL(atom.getPosition().z,  1.4370)

	// check whether we handle file with only 
	// 48 columns in the atom lines correctly
	KCFFile f3("data/KCFFile_test4.kcf");
	S.destroy();
	f3 >> S;
	TEST_EQUAL(S.countAtoms(), 23)
	TEST_EQUAL(S.countBonds(), 26)
	TEST_EQUAL(S.countMolecules(), 1)
RESULT

CHECK(Molecule* read() throw(Exception::ParseError))
  KCFFile f("data/KCFFile_test1.kcf");
	Molecule* m = f.read();
	f.close();
	TEST_EQUAL(m->countAtoms(), 23)
	TEST_EQUAL(m->countBonds(), 26)
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
a1->setCharge(0.5);
a1->setPosition(Vector3(0.1, 0.2, 0.3));

a2->setName("A2");
a2->setElement(PTE[Element::O]);
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
	TEST_FILE_REGEXP(filename.c_str(), "data/KCFFile_test2.kcf")
RESULT

CHECK(bool write(const Molecule& molecule) throw(File::CannotWrite))
	NEW_TMP_FILE(filename)
	KCFFile f(filename, std::ios::out);
	f.write(*m);
	f.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/KCFFile_test2.kcf")
RESULT

CHECK(BALL_CREATE(KCFFile))
  KCFFile f("data/KCFFile_test1.kcf");
	KCFFile* f_ptr = (KCFFile*) f.create();
	TEST_EQUAL(f_ptr->getName(), "data/KCFFile_test1.kcf")
	delete f_ptr;
RESULT

CHECK(KCFFile(const KCFFile& file) throw(Exception::FileNotFound))
  KCFFile f("data/KCFFile_test1.kcf");
	KCFFile f2(f);
	TEST_EQUAL(f2.getName(), "data/KCFFile_test1.kcf")
RESULT

CHECK([EXTRA]bool read(System& system) throw(Exception::ParseError))
	KCFFile f("data/KCFFile_test5.kcf");
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 30)
	TEST_EQUAL(system.countBonds(), 32)
	ABORT_IF(system.countAtoms() == 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
