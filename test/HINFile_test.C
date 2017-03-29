// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(HINFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HINFile* ptr;

CHECK(HINFile() throw())
	ptr = new HINFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(~HINFile() throw())
  delete ptr;
RESULT

HINFile empty;

CHECK(float getTemperature() const)
  TEST_REAL_EQUAL(empty.getTemperature(), 0.0)
RESULT

CHECK(SimpleBox3 getPeriodicBoundary() const)
	SimpleBox3 box3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	TEST_EQUAL(empty.getPeriodicBoundary(), box3)
RESULT


CHECK(HINFile(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
  HINFile hin(BALL_TEST_DATA_PATH(HINFile_test.hin));
  TEST_EQUAL(hin.isValid(), true)
	TEST_EXCEPTION(Exception::FileNotFound, HINFile f2("asddasdcasdasdasddwad"))
RESULT

System system;
HINFile hin(BALL_TEST_DATA_PATH(HINFile_test.hin));
CHECK(bool read(System& system) throw(Exception::ParseError))
  hin.read(system);
	hin.reopen();
	Vector3 position(0.59038, -0.410275, -0.860515);
  TEST_EQUAL(hin.isValid(), true)
  TEST_EQUAL(system.countAtoms(), 648)
  TEST_EQUAL(system.countMolecules(), 216)
	ABORT_IF(system.countAtoms() == 0)
	TEST_EQUAL(system.getAtom(0)->getName(), "O")
  TEST_EQUAL(system.getAtom(0)->getElement(), PTE["O"])
	TEST_REAL_EQUAL(system.getAtom(0)->getCharge(), -0.834)
  TEST_EQUAL(system.getAtom(0)->getPosition(), position)
  TEST_REAL_EQUAL(system.getAtom(0)->getRadius(), 1.4)
  TEST_EQUAL(system.getAtom(0)->countBonds(), 2) 

  TEST_EQUAL(system.getAtom(1)->getRadius() > 0.0, true)
	PRECISION(0.001)
	TEST_REAL_EQUAL(hin.getTemperature(), 297.5626)
	TEST_REAL_EQUAL(hin.getPeriodicBoundary().a.x, -9.35068)
	TEST_REAL_EQUAL(hin.getPeriodicBoundary().a.y, -9.35068)
	TEST_REAL_EQUAL(hin.getPeriodicBoundary().a.z, -9.35068)
	TEST_REAL_EQUAL(hin.getPeriodicBoundary().b.x, 9.35068)
	TEST_REAL_EQUAL(hin.getPeriodicBoundary().b.y, 9.35068)
	TEST_REAL_EQUAL(hin.getPeriodicBoundary().b.z, 9.35068)
	PRECISION(1e-5)

	System S2;
	HINFile hin2(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	hin2 >> S2;
	TEST_EQUAL(S2.countAtoms(), 31)
	TEST_EQUAL(S2.countProteins(), 1)
	TEST_EQUAL(S2.countResidues(), 3)
	TEST_EQUAL(S2.countChains(), 1)
	TEST_EQUAL(S2.countMolecules(), 1)
	TEST_EQUAL(S2.countBonds(), 30)

	TEST_EQUAL(empty.read(S2), false)
RESULT

CHECK(bool write(const System& system) throw(File::CannotWrite))
  String filename;
  NEW_TMP_FILE(filename)
  HINFile hin2(filename, std::ios::out);
	hin2.write(system);
  TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(HINFile_test2.hin))

	TEST_EXCEPTION(File::CannotWrite, empty.write(system))
RESULT

CHECK([EXTRA]HINFile::HINFile& operator >> (System& system))
	System system2;
  hin >> system2;
	TEST_EQUAL(system.countAtoms(), system2.countAtoms())

	empty >> system;
RESULT


CHECK(bool hasPeriodicBoundary() const)
	TEST_EQUAL(hin.hasPeriodicBoundary(), true)
RESULT


CHECK([EXTRA]HINFile::HINFile& operator << (const System& system))
  String filename;
  NEW_TMP_FILE(filename)
  HINFile hin2(filename, std::ios::out);
  hin2 << system;
  TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(HINFile_test2.hin))

	// test whether the name truncation works: it should truncate
	// the name of an atom containing whitespaces to the first field
	NEW_TMP_FILE(filename)
	system.beginAtom()->setName("NAME TEST");
	HINFile hin3(filename, std::ios::out);
	CAPTURE_OUTPUT_LEVEL(LogStream::WARNING_LEVEL)
		hin3 << system;
	COMPARE_OUTPUT("HINFile::write: truncated atom name 'NAME TEST' to 'NAME'.\n")
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(HINFile_test3.hin))

	TEST_EXCEPTION(File::CannotWrite, empty << system)
RESULT

CHECK([EXTRA]robust reading)	
	HINFile f(BALL_TEST_DATA_PATH(HINFile_test4.hin));
	System S;
	TEST_EXCEPTION(Exception::ParseError, f >> S)
	f.close();
RESULT

CHECK(Molecule* read() throw(Exception::ParseError))
	HINFile hin(BALL_TEST_DATA_PATH(HINFile_test.hin));
	Molecule* m = 0;
	m = hin.read();
	TEST_NOT_EQUAL(m, 0)
  TEST_EQUAL(m->countAtoms(), 3)
	TEST_EQUAL(m->getAtom(0)->getPosition(), Vector3(0.590381, -0.410275, -0.860515))
	delete m;
	m = 0;

	m = hin.read();
	TEST_NOT_EQUAL(m, 0)
  TEST_EQUAL(m->countAtoms(), 3)
	TEST_EQUAL(m->getAtom(0)->getPosition(), Vector3(-0.31307, -0.461289, 1.82765))
	delete m;


	HINFile f(BALL_TEST_DATA_PATH(HINFile_test4.hin));
	TEST_EXCEPTION(Exception::ParseError, f.read())
RESULT

CHECK(const HINFile& operator = (const HINFile& rhs) throw(Exception::FileNotFound))
	HINFile f(BALL_TEST_DATA_PATH(HINFile_test4.hin));
	HINFile f2;
	f2 = f;
	TEST_EQUAL(f2.getName(), BALL_TEST_DATA_PATH(HINFile_test4.hin))
	HINFile f3;
	TEST_EXCEPTION(Exception::FileNotFound, f2 = empty)
	f3.setName("asddasddddddasdasdasdasd");
	TEST_EXCEPTION(Exception::FileNotFound, f2 = f3)
RESULT

CHECK(bool write(const Molecule& molecule) throw(File::CannotWrite))
	TEST_EXCEPTION(File::CannotWrite, empty.write(Molecule()))
	String filename;
	NEW_TMP_FILE(filename);
	Molecule m;
	Atom a;
	m.insert(a);
	a.setName("Atom1");
	a.setPosition(Vector3(1,2,3));
	HINFile f(filename, std::ios::out);
	f.write(m);
	f.close();
	TEST_NOT_EQUAL(f.getSize(), 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
