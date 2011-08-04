// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

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

START_TEST(MOL2File)

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
	MOL2File f(BALL_TEST_DATA_PATH(AAG.mol2));
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 30)
	TEST_EQUAL(system.countResidues(), 3)
	TEST_EQUAL(system.countBonds(), 29)
RESULT

CHECK(MOL2File::MOL2File(const String& filename, File::OpenMode open_mode))
	MOL2File f(BALL_TEST_DATA_PATH(AAG.mol2), std::ios::in);
	System system;
	f.read(system);
	TEST_EQUAL(system.countAtoms(), 30)
	TEST_EQUAL(system.countResidues(), 3)
	TEST_EQUAL(system.countBonds(), 29)

	MOL2File f2(BALL_TEST_DATA_PATH(chimera-test.mol2));
	System s2;
	f2.read(s2);
	TEST_EQUAL(s2.countAtoms(), 3);
	TEST_EQUAL(s2.countBonds(), 2);
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
	MOL2File f(filename, std::ios::out);
	f.write(S);
	f.close();
	
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(MOL2File_test.mol2))
RESULT


CHECK(MOL2File::MOL2File& operator >> (System& system))
  MOL2File f(BALL_TEST_DATA_PATH(AAG.mol2));
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
	MOL2File f(filename, std::ios::out);
	f << S;	
	f.close();
	
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(MOL2File_test.mol2))
RESULT

CHECK([Extra]Reading of triple bonds)
	MOL2File f(BALL_TEST_DATA_PATH(MOL2File_test2.mol2));
	System S;
	CAPTURE_OUTPUT_LEVEL(2000)
	f >> S;
	COMPARE_OUTPUT("")
	f.close();
	TEST_EQUAL(S.countAtoms(), 21)
	TEST_EQUAL(S.countResidues(), 0)
	TEST_EQUAL(S.countBonds(), 23)
RESULT

CHECK([Extra]Handling of subfragments)
	MOL2File f(BALL_TEST_DATA_PATH(MOL2File_test3.mol2));
	System S;
	CAPTURE_OUTPUT_LEVEL(2000)
	f >> S;
	COMPARE_OUTPUT("")
	f.close();
	TEST_EQUAL(S.countAtoms(), 47)
	TEST_EQUAL(S.countFragments(), 3)
	TEST_EQUAL(S.countBonds(), 51)

	Size n = 1;
	for (FragmentIterator fi(S.beginFragment()); +fi; ++fi, ++n)
	{
		Size na = 0;
		Size nb = 0;
		switch (n)
		{
			case 1:	na = 43; nb = 49; break;
			case 2:
			case 3:	na = 2; nb = 1; break;
			default: break;
		}
		TEST_EQUAL(fi->countAtoms(), na) 
		TEST_EQUAL(fi->countBonds(), nb)
	}
RESULT

CHECK([Extra]Handling of static sets)
	MOL2File f(BALL_TEST_DATA_PATH(MOL2File_test4.mol2));
	System S;
	CAPTURE_OUTPUT_LEVEL(2000)
	f >> S;
	COMPARE_OUTPUT("");
	f.close();
	TEST_EQUAL(f.getNumberOfSets(), 1)
	MOL2File::SetStruct& set = f.getSet(0);

	TEST_EQUAL(set.number_of_members, 16)
	for (Position i=0; i<set.number_of_members; i++)
		TEST_EQUAL((Position)set.static_members[i], i+1)
RESULT

CHECK([Extra]Handling of linebreaks in static sets)
	MOL2File f(BALL_TEST_DATA_PATH(MOL2File_test_set_linebreak.mol2));
	System S;
	f >> S;
	TEST_EQUAL(f.getNumberOfSets(), 1)
	MOL2File::SetStruct& set = f.getSet(0);
	TEST_EQUAL(set.number_of_members, 20)
	for (Position i=0; i<set.number_of_members; i++)
		TEST_EQUAL((Position)set.static_members[i], i+1)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
