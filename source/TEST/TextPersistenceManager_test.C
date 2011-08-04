// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/bond.h>

///////////////////////////

START_TEST(TextPersistenceManager)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;


TextPersistenceManager* pm_ptr = 0;
CHECK(TextPersistenceManager::TextPersistenceManager())
	pm_ptr = new TextPersistenceManager();
	TEST_NOT_EQUAL(pm_ptr, 0)
RESULT

CHECK(TextPersistenceManager::~TextPersistenceManager())
	delete pm_ptr;
RESULT

String filename;
NEW_TMP_FILE(filename)
CHECK(TextPersistenceManager::TextPersistenceManager(ostream& os))
	ofstream os(filename.c_str(), ios::out);
	Composite comp;
	TextPersistenceManager pm(os);
	comp >> pm;
	os.close();
RESULT


CHECK(TextPersistenceManager::TextPersistenceManager(istream& is))
	ifstream is(filename.c_str());
	TextPersistenceManager pm(is);
	PersistentObject* po = pm.readObject();
	is.close();
	TEST_NOT_EQUAL(po, 0)
	TEST_EQUAL(RTTI::isKindOf<Composite>(*po), true)
	delete po;
RESULT


CHECK(TextPersistenceManager::TextPersistenceManager(istream& is, ostream& os))
	ifstream is;
	ofstream os;
	TextPersistenceManager pm(is, os);
	// not much to test there -- either it works or
	// it doesn't. Functionality should be covered by
	// PersistenceManager_test
RESULT


CHECK(TextPersistenceManager::writeHeader(const char* type_name, const char* name, LongPointerType ptr))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeHeader("TYPENAME1", "NAME", (LongSize)12345678);
	pm.writeHeader("TYPENAME2", (const char*)0, (LongSize)34567890);
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeHeader.txt))
RESULT


CHECK(TextPersistenceManager::checkHeader(const char* type_name, const char* name, LongPointerType& ptr))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	LongSize ptr;
	bool result = pm.checkHeader("TYPENAME1", "NAME", ptr);
	TEST_EQUAL(result, true)
	TEST_EQUAL(ptr, 12345678)
	result = pm.checkHeader("TYPENAME2", (const char*)0, ptr);
	TEST_EQUAL(result, true)
	TEST_EQUAL(ptr, 34567890)
	result = pm.checkHeader("TYPENAME2", (const char*)0, ptr);
	TEST_EQUAL(result, false)
	result = pm.checkHeader("TYPENAME2", (const char*)0, ptr);
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeTrailer(const char* name = 0))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	STATUS("stream open: " << filename)
	pm.writeTrailer("TYPENAME");
	STATUS("stream open: " << filename)
	pm.writeTrailer((const char*)0);
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeTrailer.txt))
RESULT


CHECK(TextPersistenceManager::checkTrailer(const char* name = 0))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkTrailer.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkTrailer("TYPENAME");
	TEST_EQUAL(result, true)
	result = pm.checkTrailer((const char*)0);
	TEST_EQUAL(result, true)
	result = pm.checkTrailer((const char*)0);
	TEST_EQUAL(result, false)
	result = pm.checkTrailer("TMP");
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeStreamHeader())
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeStreamHeader();
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeStreamHeader.txt))
RESULT


CHECK(TextPersistenceManager::writeStreamTrailer())
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeStreamTrailer();
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeStreamTrailer.txt))
RESULT


CHECK(TextPersistenceManager::checkStreamHeader())
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkStreamHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkStreamHeader();
	TEST_EQUAL(result, true)
	result = pm.checkStreamHeader();
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::checkStreamTrailer())
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkStreamTrailer.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkStreamTrailer();
	TEST_EQUAL(result, true)
	result = pm.checkStreamTrailer();
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::getObjectHeader(String& type_name, LongPointerType& ptr))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.getObjectHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	LongSize ptr = 0;
	String type_name = "none";
	bool result = pm.getObjectHeader(type_name, ptr);
	TEST_EQUAL(result, true)
	TEST_EQUAL(type_name, "OBJECTNAME1")
	TEST_EQUAL(ptr, 34567890)
	result = pm.getObjectHeader(type_name, ptr);
	TEST_EQUAL(result, true)
	TEST_EQUAL(type_name, "OBJECTNAME2")
	TEST_EQUAL(ptr, 12345678)
	result = pm.getObjectHeader(type_name, ptr);
	TEST_EQUAL(result, false)
RESULT


CHECK(TextPersistenceManager::writeName(const char* name))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeName("TEST1");
	pm.writeName((const char*)0);
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeName.txt))
RESULT


CHECK(TextPersistenceManager::checkName(const char* name))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkName.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkName("TEST1");
	TEST_EQUAL(result, true)
	result = pm.checkName((const char*)0);
	TEST_EQUAL(result, true)
	result = pm.checkName("TEST2");
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeStorableHeader(const char* type_name, const char* name))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeStorableHeader("TEST1", "TEST2");
	pm.writeStorableHeader((const char*)0, (const char*)0);
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeStorableHeader.txt))
RESULT


CHECK(TextPersistenceManager::checkStorableHeader(const char* type_name, const char* name))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkStorableHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkStorableHeader("TEST1", "TEST2");
	TEST_EQUAL(result, true)
	result = pm.checkStorableHeader((const char*)0, (const char*)0);
	TEST_EQUAL(result, true)
	result = pm.checkStorableHeader("TEST6", "TEST7");
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writePrimitiveHeader(const char* type_name, const char* name))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writePrimitiveHeader("TEST1", "TEST2");
	os << std::endl;
	pm.writePrimitiveHeader("TEST4", "TEST5");
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writePrimitiveHeader.txt))
RESULT


CHECK(TextPersistenceManager::checkPrimitiveHeader(const char* type_name, const char* name))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkPrimitiveHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkPrimitiveHeader("TEST1", "TEST2");
	TEST_EQUAL(result, true)
	result = pm.checkPrimitiveHeader("TEST4", "TEST5");
	TEST_EQUAL(result, true)
	result = pm.checkPrimitiveHeader("TEST6", "TEST7");
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeStorableTrailer())
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeStorableTrailer();
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeStorableTrailer.txt))
RESULT


CHECK(TextPersistenceManager::checkStorableTrailer())
 	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkStorableTrailer.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkStorableTrailer();
	TEST_EQUAL(result, true)
	result = pm.checkStorableTrailer();
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writePrimitiveTrailer())
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writePrimitiveTrailer();
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writePrimitiveTrailer.txt))
RESULT


CHECK(TextPersistenceManager::checkPrimitiveTrailer())
 	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkPrimitiveTrailer.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkPrimitiveTrailer();
	TEST_EQUAL(result, true)
	// this method doesn't really do anything, so it should return true 
	// all the time
	result = pm.checkPrimitiveTrailer();
	TEST_EQUAL(result, true)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeObjectPointerHeader(const char* type_name, const char* name))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeObjectPointerHeader("TEST1", "TEST2");
	os << std::endl;
	pm.writeObjectPointerHeader("TEST4", (const char*)0);
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeObjectPointerHeader.txt))
RESULT


CHECK(TextPersistenceManager::checkObjectPointerHeader(const char* type_name, const char* name))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkObjectPointerHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkObjectPointerHeader("TEST1", "TEST2");
	TEST_EQUAL(result, true)
	result = pm.checkObjectPointerHeader("TEST4", (const char*)0);
	TEST_EQUAL(result, true)
	result = pm.checkObjectPointerHeader("TEST6", "TEST7");
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeObjectReferenceHeader(const char* type_name, const char* name))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeObjectReferenceHeader("TEST1", "TEST2");
	os << std::endl;
	pm.writeObjectReferenceHeader("TEST4", (const char*)0);
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeObjectReferenceHeader.txt))
RESULT


CHECK(TextPersistenceManager::checkObjectReferenceHeader(const char* type_name, const char* name))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkObjectReferenceHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkObjectReferenceHeader("TEST1", "TEST2");
	TEST_EQUAL(result, true)
	result = pm.checkObjectReferenceHeader("TEST4", (const char*)0);
	TEST_EQUAL(result, true)
	result = pm.checkObjectReferenceHeader("TEST6", "TEST7");
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size))
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeObjectPointerArrayHeader("TEST1", "TEST2", 123456);
	os << std::endl;
	pm.writeObjectPointerArrayHeader("TEST4", "TEST5", 456789);
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeObjectPointerArrayHeader.txt))
RESULT


CHECK(TextPersistenceManager::checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size))
	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkObjectPointerArrayHeader.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	Size size = 0;
	bool result = pm.checkObjectPointerArrayHeader("TEST1", "TEST2", size);
	TEST_EQUAL(result, true)
	TEST_EQUAL(size, 123456)
	result = pm.checkObjectPointerArrayHeader("TEST4", "TEST5", size);
	TEST_EQUAL(result, true)
	TEST_EQUAL(size, 456789)
	result = pm.checkObjectPointerArrayHeader("TEST6", "TEST7", size);
	TEST_EQUAL(result, false)
	is.close();
RESULT


CHECK(TextPersistenceManager::writeObjectPointerArrayTrailer())
	String filename;
	NEW_TMP_FILE(filename)
	ofstream os(filename.c_str(), ios::out);
	TextPersistenceManager pm;
	pm.setOstream(os);
	pm.writeObjectPointerArrayTrailer();
	os.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(TextPersistenceManager_test.writeObjectPointerArrayTrailer.txt))
RESULT


CHECK(TextPersistenceManager::checkObjectPointerArrayTrailer())
 	ifstream is(BALL_TEST_DATA_PATH(TextPersistenceManager_test.checkObjectPointerArrayTrailer.txt));
	TextPersistenceManager pm;
	pm.setIstream(is);
	bool result = pm.checkObjectPointerArrayTrailer();
	TEST_EQUAL(result, true)
	result = pm.checkObjectPointerArrayTrailer();
	TEST_EQUAL(result, false)
	is.close();
RESULT


NEW_TMP_FILE(filename)
ofstream outfile(filename.c_str(), ios::out);

TextPersistenceManager pm;
pm.setOstream(outfile);
pm.initializeOutputStream();
pm.writeStreamHeader();
CHECK(TextPersistenceManager::put(const char c))
	pm.put((char)0);
	pm.put((char)85);
	pm.put((char)-86);
	pm.put((char)-1);
RESULT


CHECK(TextPersistenceManager::put(const Byte b))
	pm.put((Byte)0);
	pm.put((Byte)85);
	pm.put((Byte)170);
	pm.put((Byte)255);
RESULT


CHECK(TextPersistenceManager::put(const Index i))
	pm.put((Index)0);
	pm.put((Index)-1);
	pm.put((Index)0xAA55CC33);
	pm.put((Index)0xCC33AA55);
RESULT


CHECK(TextPersistenceManager::put(const Size s))
	pm.put((Size)0);
	pm.put((Size)0xAA55CC33);
	pm.put((Size)0xCC33AA55);
	pm.put((Size)0xFFFFFFFF);
RESULT


CHECK(TextPersistenceManager::put(const bool b))
	pm.put(true);
	pm.put(false);
RESULT


CHECK(TextPersistenceManager::put(const Real x))
  pm.put((Real)0.0);
	pm.put((Real)1.234567);
	pm.put((Real)-9.87654e37);
RESULT


CHECK(TextPersistenceManager::put(const DoubleReal x))
  pm.put((DoubleReal)0.0);
	pm.put((DoubleReal)1.234567);
	pm.put((DoubleReal)-9.87654e300);
RESULT


CHECK(TextPersistenceManager::put(const string& s))
  pm.put(String(""));
	pm.put(String("ABCDEFGHIJKLMNOPQRSTUVWxyz"));
RESULT


LongSize psi1 = 0x01234567;
psi1 <<= 32;
psi1 += 0xFEDCBA98;
LongSize psi2 = 0xFEDCBA98;
psi2 <<= 32;
psi2 += 0x01234567;

CHECK(TextPersistenceManager::put(const LongSize p))
	pm.put((LongSize)0);
	pm.put(psi1);
	pm.put(psi2);
RESULT

pm.writeStreamTrailer();
pm.finalizeOutputStream();
outfile.close();

ifstream infile(filename.c_str());
pm.setIstream(infile);
pm.initializeInputStream();
pm.checkStreamHeader();

CHECK(TextPersistenceManager::get(char& c))
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


CHECK(TextPersistenceManager::get(Byte& c))
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


CHECK(TextPersistenceManager::get(Index& s))
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


CHECK(TextPersistenceManager::get(Size& s))
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


CHECK(TextPersistenceManager::get(bool& s))
  bool b;
	pm.get(b);
	TEST_EQUAL(b, true)
	pm.get(b);
	TEST_EQUAL(b, false)
RESULT


CHECK(TextPersistenceManager::get(Real& x))
	Real x;
	pm.get(x);
  TEST_EQUAL(x, (Real)0.0)
	pm.get(x);
	PRECISION(1e-5)
  TEST_REAL_EQUAL(x, (Real)1.234567)
	pm.get(x);
	PRECISION(1e30)
  TEST_REAL_EQUAL(x, (Real)-9.87654e37)
RESULT


CHECK(TextPersistenceManager::get(DoubleReal& s))
	DoubleReal x;
	pm.get(x);
  TEST_EQUAL(x, (DoubleReal)0.0)
	pm.get(x);
	PRECISION(1e-5)
  TEST_REAL_EQUAL(x, (DoubleReal)1.234567)
	pm.get(x);
	PRECISION(1e290)
  TEST_REAL_EQUAL(x, (DoubleReal)-9.87654e300)
RESULT


CHECK(TextPersistenceManager::get(String& s))
	String s;
	pm.get(s);
	TEST_EQUAL(s, "")
	pm.get(s);
	TEST_EQUAL(s, "ABCDEFGHIJKLMNOPQRSTUVWxyz")
RESULT


CHECK(TextPersistenceManager::get(LongSize& p))
	LongSize p;
	pm.get(p);
	TEST_EQUAL(p, 0)
	pm.get(p);
	TEST_EQUAL(p, psi1)
	pm.get(p);
	TEST_EQUAL(p, psi2)
	STATUS(setbase(16) << p << " " << psi2)
RESULT

pm.checkStreamTrailer();
pm.finalizeInputStream();
infile.close();

CHECK([EXTRA] full_test1)
	String filename;
	PDBFile in(BALL_TEST_DATA_PATH(bpti.pdb));
	System s;
	in >> s;

	NEW_TMP_FILE(filename);
	ofstream os(filename.c_str(), std::ios::out | std::ios::binary);
	TextPersistenceManager pm(os);
	s >> pm;
	os.close();

	ifstream is(filename.c_str(), std::ios::in);
	TextPersistenceManager pm2(is);
	System* s2 = (System*) pm2.readObject();
	is.close();

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
CHECK([Extra] full_test2)
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
	TextPersistenceManager pm(os);
	s >> pm;
	os.close();

	ifstream is(filename.c_str(), std::ios::in | std::ios::binary);
	TextPersistenceManager pm2(is);
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
