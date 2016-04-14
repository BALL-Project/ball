// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/CONCEPT/XDRPersistenceManager.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/bond.h>

///////////////////////////

START_TEST(XDRPersistenceManager)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

XDRPersistenceManager* pm_ptr = 0;
CHECK(XDRPersistenceManager() throw())
	pm_ptr = new XDRPersistenceManager;
	TEST_NOT_EQUAL(pm_ptr, 0)
RESULT

CHECK(~XDRPersistenceManager())
	delete pm_ptr;
RESULT

String filename;
NEW_TMP_FILE(filename)
CHECK(XDRPersistenceManager(std::ostream& os) throw())
	ofstream os(filename.c_str(), std::ios::out|std::ios::binary);
	Composite comp;
	XDRPersistenceManager pm(os);
	comp >> pm;
	os.close();
RESULT


CHECK(XDRPersistenceManager(std::istream& is) throw())
ifstream is(filename.c_str(), std::ios::in|std::ios::binary);
	XDRPersistenceManager pm(is);
	PersistentObject* po = pm.readObject();
	is.close();
	TEST_NOT_EQUAL(po, 0)
    TEST_EQUAL(RTTI::isKindOf<Composite>(po), true)
	delete po;
RESULT


CHECK(XDRPersistenceManager(std::istream& is, std::ostream& os) throw())
	String outfilename;
	NEW_TMP_FILE(outfilename);
	ifstream is(filename.c_str(), std::ios::in|std::ios::binary);
	ofstream os(outfilename.c_str(), std::ios::out|std::ios::binary);
	XDRPersistenceManager pm(is, os);
	PersistentObject* po = pm.readObject();
	is.close();
	TEST_NOT_EQUAL(po, 0)
    TEST_EQUAL(RTTI::isKindOf<Composite>(po), true)
	// *po >> pm;
	os.close();
	delete po;
RESULT


CHECK(void writeHeader(const char* type_name, const char* name, PointerSizeUInt ptr) throw())
	String outfilename;
	NEW_TMP_FILE(outfilename);
	ofstream os(outfilename.c_str(), std::ios::out|std::ios::binary);
	XDRPersistenceManager pm(os);
	Composite composite;
	// composite >> pm;
	os.close();
	TEST_FILE_REGEXP(outfilename.c_str(), BALL_TEST_DATA_PATH(XDRPersistenceManager_test1.txt))
RESULT


CHECK(bool checkHeader(const char* type_name, const char* name, PointerSizeUInt& ptr) throw())
  //?????
RESULT


CHECK(void writeTrailer(const char* name = 0) throw())
  //?????
RESULT


CHECK(bool checkTrailer(const char* name = 0) throw())
  //?????
RESULT


CHECK(void writeStreamHeader() throw())
  //?????
RESULT


CHECK(void writeStreamTrailer() throw())
  //?????
RESULT


CHECK(bool checkStreamHeader() throw())
  //?????
RESULT


CHECK(bool checkStreamTrailer() throw())
  //?????
RESULT


CHECK(bool getObjectHeader(String& type_name, PointerSizeUInt& ptr) throw())
  //?????
RESULT


CHECK(void writeName(const char* name) throw())
  //?????
RESULT


CHECK(bool checkName(const char* name) throw())
  //?????
RESULT


CHECK(void writeStorableHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(bool checkStorableHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(void writePrimitiveHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(bool checkPrimitiveHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(void writeStorableTrailer() throw())
  //?????
RESULT


CHECK(bool checkStorableTrailer() throw())
  //?????
RESULT


CHECK(void writePrimitiveTrailer() throw())
  //?????
RESULT


CHECK(bool checkPrimitiveTrailer() throw())
  //?????
RESULT


CHECK(void writeObjectPointerHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(bool checkObjectPointerHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(void writeObjectReferenceHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(bool checkObjectReferenceHeader(const char* type_name, const char* name) throw())
  //?????
RESULT


CHECK(void writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size) throw())
  //?????
RESULT


CHECK(bool checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size) throw())
  //?????
RESULT


CHECK(void writeObjectPointerArrayTrailer() throw())
  //?????
RESULT


CHECK(bool checkObjectPointerArrayTrailer() throw())
  //?????
RESULT


NEW_TMP_FILE(filename)
std::ofstream outfile(filename.c_str(), std::ios::out|std::ios::binary);

XDRPersistenceManager pm;
pm.setOstream(outfile);
pm.initializeOutputStream();
pm.writeStreamHeader();
CHECK(void put(const char c) throw())
	pm.put((char)0);
	pm.put((char)85);
	pm.put((char)-86);
	pm.put((char)-1);
RESULT


CHECK(void put(const Byte b) throw())
	pm.put((Byte)0);
	pm.put((Byte)85);
	pm.put((Byte)170);
	pm.put((Byte)255);
RESULT


CHECK(void put(const Index i) throw())
	pm.put((Index)0);
	pm.put((Index)-1);
	pm.put((Index)0xAA55CC33);
	pm.put((Index)0xCC33AA55);
RESULT


CHECK(void put(const Size s) throw())
	pm.put((Size)0);
	pm.put((Size)0xAA55CC33);
	pm.put((Size)0xCC33AA55);
	pm.put((Size)0xFFFFFFFF);
RESULT


CHECK(void put(const bool b) throw())
	pm.put(true);
	pm.put(false);
RESULT


CHECK(void put(const Real f) throw())
  pm.put((Real)0.0);
	pm.put((Real)1.234567);
	pm.put((Real)-9.87654e37);
RESULT


CHECK(void put(const DoubleReal d) throw())
  pm.put((DoubleReal)0.0);
	pm.put((DoubleReal)1.234567);
	pm.put((DoubleReal)-9.87654e300);
RESULT


CHECK(void put(const string& s) throw())
  pm.put(String(""));
	pm.put(String("ABCDEFGHIJKLMNOPQRSTUVWxyz"));
RESULT


LongSize psi1 = 0x01234567;
psi1 <<= 32;
psi1 += 0xFEDCBA98;
LongSize psi2 = 0xFEDCBA98;
psi2 <<= 32;
psi2 += 0x01234567;

CHECK(void put(const LongSize p) throw())
	pm.put((LongSize)0);
	pm.put(psi1);
	pm.put(psi2);
RESULT

pm.writeStreamTrailer();
pm.finalizeOutputStream();
outfile.close();

std::ifstream infile(filename.c_str(), std::ios::in|std::ios::binary);
pm.setIstream(infile);
pm.initializeInputStream();
pm.checkStreamHeader();

CHECK(void get(char& c) throw())
	char c;
	pm.get(c);
	TEST_EQUAL((Index)(signed char)c, 0)
	pm.get(c);
	TEST_EQUAL((Index)(signed char)c, 85)
	pm.get(c);
	TEST_EQUAL((Index)(signed char)c, -86)
	pm.get(c);
	TEST_EQUAL((Index)(signed char)c, -1)
RESULT


CHECK(void get(Byte& c) throw())
	Byte c;
	pm.get(c);
	TEST_EQUAL((Size)c, 0)
	pm.get(c);
	TEST_EQUAL((Size)c, 85)
	pm.get(c);
	TEST_EQUAL((Size)c, 170)
	pm.get(c);
	TEST_EQUAL((Size)c, 255)
RESULT


CHECK(void get(Index& s) throw())
	Index i;
	pm.get(i);
	TEST_EQUAL(i, (Index)0)
	pm.get(i);
	TEST_EQUAL(i, (Index)-1)
	pm.get(i);
	TEST_EQUAL(i, (Index)0xAA55CC33)
	pm.get(i);
	TEST_EQUAL(i, (Index)0xCC33AA55)
RESULT


CHECK(void get(Size& s) throw())
	Size s;
	pm.get(s);
	TEST_EQUAL(s, (Size)0)
	pm.get(s);
	TEST_EQUAL(s, (Size)0xAA55CC33)
	pm.get(s);
	TEST_EQUAL(s, (Size)0xCC33AA55)
	pm.get(s);
	TEST_EQUAL(s, (Size)0xFFFFFFFF)
RESULT


CHECK(void get(bool& b) throw())
  bool b;
	pm.get(b);
	TEST_EQUAL(b, true)
	pm.get(b);
	TEST_EQUAL(b, false)
RESULT


CHECK(void get(Real& f) throw())
	Real x;
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)0.0)
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)1.234567)
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)-9.87654e37)
RESULT


CHECK(void get(DoubleReal& d) throw())
	DoubleReal x;
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)0.0)
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)1.234567)
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)-9.87654e300)
RESULT


CHECK(void get(string& s) throw())
	string s;
	pm.get(s);
	TEST_EQUAL(s, "")
	pm.get(s);
	TEST_EQUAL(s, "ABCDEFGHIJKLMNOPQRSTUVWxyz")
RESULT


CHECK(void get(LongSize& p) throw())
	LongSize p;
	pm.get(p);
	TEST_EQUAL(p, 0)
	pm.get(p);
	TEST_EQUAL(p, psi1)
	pm.get(p);
	TEST_EQUAL(p, psi2)
RESULT

pm.finalizeInputStream();
infile.close();

CHECK(void finalizeInputStream() throw())
  // ???
RESULT

CHECK(void finalizeOutputStream() throw())
	// ???
RESULT

CHECK(void initializeInputStream() throw())
	// ???
RESULT

CHECK(void initializeOutputStream() throw())
	// ???
RESULT

CHECK([Extra] full_test0)
	String filename;
	Atom a1;
	Atom a2;
	Bond b1;

	b1.setFirstAtom(&a1);
	b1.setSecondAtom(&a2);

	NEW_TMP_FILE(filename);
	ofstream os(filename.c_str(), std::ios::out|std::ios::binary);
	XDRPersistenceManager pm(os);
	b1 >> pm;
	os.close();

	ifstream is(filename.c_str(), std::ios::in|std::ios::binary);
	XDRPersistenceManager pm2(is);

	PersistentObject* po =  0;
	STATUS("Before readObject...")
    po = pm2.readObject();
    STATUS("After readObject...")
	is.close();
    TEST_EQUAL(RTTI::isKindOf<Bond>(po), true)

	// Cleanup
	Bond* bond = dynamic_cast<Bond*>(po);
	delete bond->getFirstAtom();
	delete bond->getSecondAtom();

	bond->setFirstAtom(NULL);
	bond->setSecondAtom(NULL);

	delete po;
RESULT
	

CHECK([Extra] full_test1)
	String filename;

	System s1;
	Protein* p1 = new Protein;
	Chain* c1 = new Chain;
	SecondaryStructure* ss1 = new SecondaryStructure;
	Residue* r1  = new Residue;
	PDBAtom* a1 = new PDBAtom;
	PDBAtom* a2 = new PDBAtom;
	s1.insert(*p1);
	p1->insert(*c1);
	c1->insert(*ss1);
	ss1->insert(*r1);
	r1->insert(*a1);
	r1->insert(*a2);
	TEST_NOT_EQUAL(a1->createBond(*a2), 0)

	NEW_TMP_FILE(filename);
	ofstream os(filename.c_str(), std::ios::out | std::ios::binary);
	XDRPersistenceManager pm(os);
	s1 >> pm;
	os.close();

	ifstream is(filename.c_str(), std::ios::in | std::ios::binary);
	XDRPersistenceManager pm2(is);
	System* s2 = (System*) pm2.readObject();
	is.close();

	TEST_NOT_EQUAL(s2, 0)
	ABORT_IF(s2 == 0)

	TEST_EQUAL(s1.countAtoms(), s2->countAtoms())
	delete s2;
RESULT

CHECK([Extra] full_test1)
	String filename;

	Fragment f;
	f.insert(*new Atom);

	NEW_TMP_FILE(filename);
	ofstream os(filename.c_str(), std::ios::out | std::ios::binary);
	XDRPersistenceManager pm(os);
	f >> pm;
	os.close();
	STATUS("wrote file")

	ifstream is(filename.c_str(), std::ios::in|std::ios::binary);
	XDRPersistenceManager pm2(is);
	Fragment* f2 = (Fragment*) pm2.readObject();
	is.close();

	TEST_NOT_EQUAL(f2, 0)
	ABORT_IF(f2 == 0)

	TEST_EQUAL(f.countAtoms(), f2->countAtoms())
	delete f2;
RESULT


CHECK([EXTRA] full_test2)
	String filename;
	PDBFile in(BALL_TEST_DATA_PATH(bpti.pdb));
	System s;
	in >> s;

	NEW_TMP_FILE(filename);
	ofstream os(filename.c_str(), std::ios::out | std::ios::binary);
	XDRPersistenceManager pm(os);
	s >> pm;
	os.close();

	ifstream is(filename.c_str(), std::ios::in | std::ios::binary);
	XDRPersistenceManager pm2(is);
	System* s2 = (System*) pm2.readObject();
	is.close();

	TEST_NOT_EQUAL(s2, 0)
	ABORT_IF(s2 == 0)

	TEST_EQUAL(s.countAtoms(), s2->countAtoms())
	AtomIterator ai = s.beginAtom();
	AtomIterator ai2 = s2->beginAtom();
	for (; +ai; ai++, ai2++)
	{
		TEST_EQUAL((*ai).getPosition(), (*ai2).getPosition())
		TEST_EQUAL((*ai).getFullName(), (*ai2).getFullName())
		TEST_EQUAL((*ai).getType(), 		(*ai2).getType())
		TEST_EQUAL((*ai).getTypeName(), (*ai2).getTypeName())
		TEST_EQUAL((*ai).getVelocity(), (*ai2).getVelocity())
		TEST_EQUAL((*ai).getForce(), 		(*ai2).getForce())
		TEST_EQUAL((*ai2).isValid(), 		true)
	}

	delete s2;
RESULT


// this time with bonds and all properties !
CHECK([Extra] full_test3)
	String filename;
	HINFile hin(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	System s;
	hin >> s;

	AtomIterator ai = s.beginAtom();
	Vector3 v(1.1, 2.2, 3.3);
	(*ai).setVelocity(v);
	v.set(4.4, 5.5, 6.6);
	(*ai).setForce(v);
	(*ai).setCharge(3.456);
	(*ai).setTypeName("blub");
	(*ai).setRadius(4.56);
	(*ai).setType(2);

	NEW_TMP_FILE(filename);
	ofstream os(filename.c_str(), std::ios::out | std::ios::binary);
	XDRPersistenceManager pm(os);
	s >> pm;
	os.close();

	ifstream is(filename.c_str(), std::ios::in | std::ios::binary);
	XDRPersistenceManager pm2(is);
	System* s2 = (System*) pm2.readObject();
	is.close();

	TEST_NOT_EQUAL(s2, 0)
	ABORT_IF(s2 == 0)

	TEST_EQUAL(s.countAtoms(), s2->countAtoms())
	TEST_EQUAL(s.countBonds(), s2->countBonds())

	AtomIterator ai2 = s2->beginAtom();
	for (; +ai; ai++, ai2++)
	{
		TEST_EQUAL((*ai).getPosition(), (*ai2).getPosition())
		TEST_EQUAL((*ai).getFullName(), (*ai2).getFullName())
		TEST_EQUAL((*ai).getType(), 		(*ai2).getType())
		TEST_EQUAL((*ai).getTypeName(), (*ai2).getTypeName())
		TEST_EQUAL((*ai).getVelocity(), (*ai2).getVelocity())
		TEST_EQUAL((*ai).getForce(), 		(*ai2).getForce())
		TEST_EQUAL((*ai).getCharge(), 	(*ai2).getCharge())
		TEST_EQUAL((*ai).getRadius(), 	(*ai2).getRadius())
		TEST_EQUAL((*ai2).isValid(), 		true)

		// sometimes bonds get switched
		for (Position p = 0; p < (*ai).countBonds(); p++)
		{
			if ((*ai ).getBond(p)->getFirstAtom()->getFullName() ==
					(*ai2).getBond(p)->getFirstAtom()->getFullName())
			{
				TEST_EQUAL((*ai).getBond(p)->getFirstAtom()->getFullName(), 	(*ai2).getBond(p)->getFirstAtom()->getFullName())
				TEST_EQUAL((*ai).getBond(p)->getSecondAtom()->getFullName(), 	(*ai2).getBond(p)->getSecondAtom()->getFullName())
			}
			else
			{
				TEST_EQUAL((*ai).getBond(p)->getFirstAtom()->getFullName(), 	(*ai2).getBond(p)->getSecondAtom()->getFullName())
				TEST_EQUAL((*ai).getBond(p)->getSecondAtom()->getFullName(), 	(*ai2).getBond(p)->getFirstAtom()->getFullName())
			}
		}
	}

 	delete s2;
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

