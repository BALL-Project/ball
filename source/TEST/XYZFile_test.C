// $Id: XYZFile_test.C,v 1.1 2000/05/15 19:17:14 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/XYZFile.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(XYZFile, "$Id: XYZFile_test.C,v 1.1 2000/05/15 19:17:14 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


XYZFile*	file_ptr;
CHECK(XYZFile::XYZFile())
	file_ptr = new XYZFile;
	TEST_NOT_EQUAL(file_ptr, 0)
RESULT


CHECK(XYZFile::~XYZFile())
	delete file_ptr;
RESULT


CHECK(XYZFile::XYZFile(const String& filename, File::OpenMode open_mode))
	// checking for default mode: reading
	XYZFile f("data/XYZFile_test1.xyz");
	TEST_EQUAL(f.isValid(), true)
RESULT


CHECK(XYZFile::read(System& system))
  XYZFile f("data/XYZFile_test1.xyz");
	System S;
	f.read(S);
	f.close();

	TEST_EQUAL(S.countAtoms(), 2)
	TEST_EQUAL(S.getName(), "TEST TEST")
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


CHECK(XYZFile::write(const System& system))
  //BAUSTELLE
RESULT


CHECK(XYZFile::XYZFile& operator >> (System& system))
  XYZFile f("data/XYZFile_test1.xyz");
	System S;
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 2)
	TEST_EQUAL(S.getName(), "TEST TEST")
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

CHECK(XYZFile::XYZFile& operator << (const System& system))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
