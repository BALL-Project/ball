// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/MOLFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/MATHS/vector3.h>

///////////////////////////

START_TEST(MOLFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


MOLFile* mf = 0;
CHECK(MOLFile() throw())
	mf = new MOLFile;
	TEST_NOT_EQUAL(mf, 0)
RESULT


CHECK(~MOLFile() throw())
	delete mf;
RESULT


CHECK(bool read(System& system) throw(Exception::ParseError))
	MOLFile f(BALL_TEST_DATA_PATH(MOLFile_test1.mol));
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 23)
	TEST_EQUAL(system.countBonds(), 26)
	ABORT_IF(system.countAtoms() == 0)
	Atom& atom = *system.beginAtom();
	TEST_REAL_EQUAL(atom.getPosition().x,  1.5737)
	TEST_REAL_EQUAL(atom.getPosition().y, -3.1475)
	TEST_REAL_EQUAL(atom.getPosition().z,  0.0000)
	system.clear();
	MOLFile f2(BALL_TEST_DATA_PATH(../MOLFile_test.C));	
	TEST_EXCEPTION(Exception::ParseError, f2.read(system))
	MOLFile f3(BALL_TEST_DATA_PATH(Selectable_test.txt));
	bool result = f3.read(system);
	TEST_EQUAL(result, false)
RESULT


CHECK(MOLFile(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	MOLFile f(BALL_TEST_DATA_PATH(MOLFile_test1.mol), std::ios::in);
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 23)
	TEST_EQUAL(system.countMolecules(), 1)
	TEST_EQUAL(system.countBonds(), 26)
	TEST_EXCEPTION(Exception::FileNotFound, MOLFile f2("MOLFile_test1.mol"))
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
	MOLFile f(filename, std::ios::out);
	f.write(S);
	f.close();
	
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(MOLFile_test2.mol))

	MOLFile f2(BALL_TEST_DATA_PATH(../MOLFile_test.C), std::ios::in);
	TEST_EXCEPTION(File::CannotWrite, f2.write(S))
RESULT


CHECK([EXTRA]MOLFile::MOLFile& operator >> (System& system))
  MOLFile f(BALL_TEST_DATA_PATH(MOLFile_test1.mol));
	System S;
	f >> S;
	f.close();
	TEST_EQUAL(S.countAtoms(), 23)
	TEST_EQUAL(S.countBonds(), 26)
	TEST_EQUAL(S.countMolecules(), 1)

	MOLFile f2(BALL_TEST_DATA_PATH(MOLFile_test3.mol));
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
	MOLFile f3(BALL_TEST_DATA_PATH(MOLFile_test4.mol));
	S.destroy();
	f3 >> S;
	TEST_EQUAL(S.countAtoms(), 23)
	TEST_EQUAL(S.countBonds(), 26)
	TEST_EQUAL(S.countMolecules(), 1)
RESULT

CHECK(Molecule* read() throw(Exception::ParseError))
  MOLFile f(BALL_TEST_DATA_PATH(MOLFile_test1.mol));
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

CHECK([EXTRA]MOLFile::MOLFile& operator << (const System& system))
	NEW_TMP_FILE(filename)
	MOLFile f(filename, std::ios::out);
	f << S;	
	f.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(MOLFile_test2.mol))
RESULT

CHECK(bool write(const Molecule& molecule) throw(File::CannotWrite))
	NEW_TMP_FILE(filename)
	MOLFile f(filename, std::ios::out);
	f.write(*m);
	f.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(MOLFile_test2.mol))
RESULT

CHECK([EXTRA]bool read(System& system) throw(Exception::ParseError))
	MOLFile f(BALL_TEST_DATA_PATH(MOLFile_test5.mol));
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 30)
	TEST_EQUAL(system.countBonds(), 32)
	ABORT_IF(system.countAtoms() == 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
