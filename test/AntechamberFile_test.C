// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/antechamberFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/molFileFactory.h>
///////////////////////////

START_TEST(AntechamberFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


AntechamberFile* file_ptr = 0;
CHECK(AntechamberFile::AntechamberFile())
	file_ptr = new AntechamberFile;
	TEST_NOT_EQUAL(file_ptr, 0)
RESULT


CHECK(AntechamberFile::~AntechamberFile())
	delete file_ptr;
RESULT


CHECK(AntechamberFile::AntechamberFile(const String& filename, File::OpenMode open_mode))
	// checking for default mode: reading
	AntechamberFile f(BALL_TEST_DATA_PATH(AntechamberFile_test1.ac));
	TEST_EQUAL(f.isValid(), true)
RESULT


CHECK(AntechamberFile::read(System& system))
  AntechamberFile f(BALL_TEST_DATA_PATH(AntechamberFile_test2.ac));
	System S;
	f.read(S);
	f.close();

	TEST_EQUAL(S.countAtoms(), 2)	
	AtomIterator it = S.beginAtom();
	TEST_EQUAL(it->getElement().getSymbol(), "H")
	TEST_REAL_EQUAL(it->getPosition().x, 0.0)
	TEST_REAL_EQUAL(it->getPosition().y, 1.0)
	TEST_REAL_EQUAL(it->getPosition().z, 2.0)
	it++;
	TEST_EQUAL(it->getElement().getSymbol(), "O")
	TEST_REAL_EQUAL(it->getPosition().x, 3.0)
	TEST_REAL_EQUAL(it->getPosition().y, 4.0)
	TEST_REAL_EQUAL(it->getPosition().z, 5.0)
RESULT


CHECK(AntechamberFile::read(System& system))
  AntechamberFile f(BALL_TEST_DATA_PATH(AntechamberFile_test1.ac));
	System S;
	f.read(S);
	f.close();

	TEST_EQUAL(S.countAtoms(), 55)
	TEST_EQUAL(S.countBonds(), 58)
RESULT

CHECK(AntechamberFile::read(System& system))
  AntechamberFile f(BALL_TEST_DATA_PATH(current.ac));
	System S;
	f.read(S);
	f.close();

	PRECISION(0.001)
	TEST_EQUAL(S.countAtoms(), 1258)
	AtomIterator it = S.beginAtom();
	TEST_EQUAL(it->getElement().getSymbol(), "C")
	TEST_REAL_EQUAL(it->getPosition().x, 306.379299)
	it++;
	TEST_EQUAL(it->getElement().getSymbol(), "O")
	TEST_REAL_EQUAL(it->getPosition().z, 328.972092)
RESULT


String filename;
NEW_TMP_FILE(filename)
CHECK(AntechamberFile::write(const System& system))
	System S;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	a1->setElement(PTE[Element::C]);
	a2->setElement(PTE[Element::O]);
	a1->setPosition(Vector3(1.0, 2.0, 3.0));
	a2->setPosition(Vector3(4.0, 5.0, 6.0));
	m->insert(*a1);
	m->insert(*a2);
	S.insert(*m);
	
	AntechamberFile f(filename, std::ios::out);
	f.write(S);
	f.close();
		
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(AntechamberFile_test3.ac))
RESULT


CHECK(AntechamberFile::AntechamberFile& operator >> (System& system))
  AntechamberFile f(BALL_TEST_DATA_PATH(AntechamberFile_test2.ac));
	System S;
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 2)	
	AtomIterator it = S.beginAtom();
	TEST_EQUAL(it->getElement().getSymbol(), "H")
	TEST_REAL_EQUAL(it->getPosition().x, 0.0)
	TEST_REAL_EQUAL(it->getPosition().y, 1.0)
	TEST_REAL_EQUAL(it->getPosition().z, 2.0)
	it++;
	TEST_EQUAL(it->getElement().getSymbol(), "O")
	TEST_REAL_EQUAL(it->getPosition().x, 3.0)
	TEST_REAL_EQUAL(it->getPosition().y, 4.0)
	TEST_REAL_EQUAL(it->getPosition().z, 5.0)
RESULT

CHECK(AntechamberFile::AntechamberFile& operator << (const System& system))
	System S;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	a1->setElement(PTE[Element::C]);
	a2->setElement(PTE[Element::O]);
	a1->setPosition(Vector3(1.0, 2.0, 3.0));
	a2->setPosition(Vector3(4.0, 5.0, 6.0));
	m->insert(*a1);
	m->insert(*a2);
	S.insert(*m);
	
	AntechamberFile f(filename, std::ios::out);
	f << S;
	f.close();
		
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(AntechamberFile_test3.ac))
RESULT

CHECK(AntechamberFile read via MolFileFactory)
	GenericMolFile* file = MolFileFactory::open(BALL_TEST_DATA_PATH(AntechamberFile_test2.ac));
	System S;
	*file >> S;
	file->close();
	TEST_EQUAL(S.countAtoms(), 2)	
	AtomIterator it = S.beginAtom();
	TEST_EQUAL(it->getElement().getSymbol(), "H")
	TEST_REAL_EQUAL(it->getPosition().x, 0.0)
	TEST_REAL_EQUAL(it->getPosition().y, 1.0)
	TEST_REAL_EQUAL(it->getPosition().z, 2.0)
	it++;
	TEST_EQUAL(it->getElement().getSymbol(), "O")
	TEST_REAL_EQUAL(it->getPosition().x, 3.0)
	TEST_REAL_EQUAL(it->getPosition().y, 4.0)
	TEST_REAL_EQUAL(it->getPosition().z, 5.0)

	delete file;
RESULT

NEW_TMP_FILE_WITH_SUFFIX(filename, "ac")
CHECK(AntechamberFile write via MolFileFactory)
	System S;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	a1->setElement(PTE[Element::C]);
	a2->setElement(PTE[Element::O]);
	a1->setPosition(Vector3(1.0, 2.0, 3.0));
	a2->setPosition(Vector3(4.0, 5.0, 6.0));
	m->insert(*a1);
	m->insert(*a2);
	S.insert(*m);

	GenericMolFile* f = MolFileFactory::open(filename, std::ios::out);
	if (f)
	{
		f->write(S);
		f->close(); 
		TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(AntechamberFile_test3.ac))
	}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
