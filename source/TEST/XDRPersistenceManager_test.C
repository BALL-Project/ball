// $Id: XDRPersistenceManager_test.C,v 1.6 2001/12/30 13:29:03 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/XDRPersistenceManager.h>
#include <BALL/CONCEPT/composite.h>

///////////////////////////

START_TEST(XDRPersistenceManager, "$Id: XDRPersistenceManager_test.C,v 1.6 2001/12/30 13:29:03 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

XDRPersistenceManager* pm_ptr = 0;
CHECK(XDRPersistenceManager::XDRPersistenceManager())
	pm_ptr = new XDRPersistenceManager;
	TEST_NOT_EQUAL(pm_ptr, 0)
RESULT

CHECK(XDRPersistenceManager::~XDRPersistenceManager())
	delete pm_ptr;
RESULT

String filename;
NEW_TMP_FILE(filename)
CHECK(XDRPersistenceManager::XDRPersistenceManager(std::ostream& os))
	ofstream os(filename.c_str(), std::ios::out);
	Composite comp;
	XDRPersistenceManager pm(os);
	comp >> pm;
	os.close();
RESULT


CHECK(XDRPersistenceManager::XDRPersistenceManager(std::istream& is))
	ifstream is(filename.c_str());
	XDRPersistenceManager pm(is);
	PersistentObject* po = pm.readObject();
	is.close();
	TEST_NOT_EQUAL(po, 0)
	TEST_EQUAL(RTTI::isKindOf<Composite>(*po), true)
RESULT


CHECK(XDRPersistenceManager::XDRPersistenceManager(std::istream& is, std::ostream& os))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeHeader(const char* type_name, const char* name, LongPointerType ptr))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkHeader(const char* type_name, const char* name, LongPointerType& ptr))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeTrailer(const char* name = 0))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkTrailer(const char* name = 0))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeStreamHeader())
  //?????
RESULT


CHECK(XDRPersistenceManager::writeStreamTrailer())
  //?????
RESULT


CHECK(XDRPersistenceManager::checkStreamHeader())
  //?????
RESULT


CHECK(XDRPersistenceManager::checkStreamTrailer())
  //?????
RESULT


CHECK(XDRPersistenceManager::getObjectHeader(String& type_name, LongPointerType& ptr))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeName(const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkName(const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeStorableHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkStorableHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::writePrimitiveHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkPrimitiveHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeStorableTrailer())
  //?????
RESULT


CHECK(XDRPersistenceManager::checkStorableTrailer())
  //?????
RESULT


CHECK(XDRPersistenceManager::writePrimitiveTrailer())
  //?????
RESULT


CHECK(XDRPersistenceManager::checkPrimitiveTrailer())
  //?????
RESULT


CHECK(XDRPersistenceManager::writeObjectPointerHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkObjectPointerHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeObjectReferenceHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkObjectReferenceHeader(const char* type_name, const char* name))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size))
  //?????
RESULT


CHECK(XDRPersistenceManager::checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size))
  //?????
RESULT


CHECK(XDRPersistenceManager::writeObjectPointerArrayTrailer())
  //?????
RESULT


CHECK(XDRPersistenceManager::checkObjectPointerArrayTrailer())
  //?????
RESULT


NEW_TMP_FILE(filename)
std::ofstream outfile(filename.c_str(), std::ios::out);

XDRPersistenceManager pm;
pm.setOstream(outfile);
pm.initializeOutputStream();
pm.writeStreamHeader();
CHECK(XDRPersistenceManager::put(const char c))
	pm.put((char)0);
	pm.put((char)85);
	pm.put((char)-86);
	pm.put((char)-1);
RESULT


CHECK(XDRPersistenceManager::put(const Byte b))
	pm.put((Byte)0);
	pm.put((Byte)85);
	pm.put((Byte)170);
	pm.put((Byte)255);
RESULT


CHECK(XDRPersistenceManager::put(const Index i))
	pm.put((Index)0);
	pm.put((Index)-1);
	pm.put((Index)0xAA55CC33);
	pm.put((Index)0xCC33AA55);
RESULT


CHECK(XDRPersistenceManager::put(const Size s))
	pm.put((Size)0);
	pm.put((Size)0xAA55CC33);
	pm.put((Size)0xCC33AA55);
	pm.put((Size)0xFFFFFFFF);
RESULT


CHECK(XDRPersistenceManager::put(const bool b))
	pm.put(true);
	pm.put(false);
RESULT


CHECK(XDRPersistenceManager::put(const Real x))
  pm.put((Real)0.0);
	pm.put((Real)1.234567);
	pm.put((Real)-9.87654e37);
RESULT


CHECK(XDRPersistenceManager::put(const DoubleReal x))
  pm.put((DoubleReal)0.0);
	pm.put((DoubleReal)1.234567);
	pm.put((DoubleReal)-9.87654e300);
RESULT


CHECK(XDRPersistenceManager::put(const string& s))
  pm.put(String(""));
	pm.put(String("ABCDEFGHIJKLMNOPQRSTUVWxyz"));
RESULT


PointerSizeInt psi1 = 0x01234567;
psi1 <<= 32;
psi1 += 0xFEDCBA98;
PointerSizeInt psi2 = 0xFEDCBA98;
psi2 <<= 32;
psi2 += 0x01234567;

CHECK(XDRPersistenceManager::put(const PointerSizeInt p))
	pm.put((PointerSizeInt)0);
	pm.put(psi1);
	pm.put(psi2);
RESULT

pm.writeStreamTrailer();
pm.finalizeOutputStream();
outfile.close();

std::ifstream infile(filename.c_str());
pm.setIstream(infile);
pm.initializeInputStream();
pm.checkStreamHeader();

CHECK(XDRPersistenceManager::get(char& c))
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


CHECK(XDRPersistenceManager::get(Byte& c))
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


CHECK(XDRPersistenceManager::get(Index& s))
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


CHECK(XDRPersistenceManager::get(Size& s))
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


CHECK(XDRPersistenceManager::get(bool& s))
  bool b;
	pm.get(b);
	TEST_EQUAL(b, true)
	pm.get(b);
	TEST_EQUAL(b, false)
RESULT


CHECK(XDRPersistenceManager::get(Real& x))
	Real x;
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)0.0)
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)1.234567)
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)-9.87654e37)
RESULT


CHECK(XDRPersistenceManager::get(DoubleReal& s))
	DoubleReal x;
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)0.0)
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)1.234567)
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)-9.87654e300)
RESULT


CHECK(XDRPersistenceManager::get(String& s))
	String s;
	pm.get(s);
	TEST_EQUAL(s, "")
	pm.get(s);
	TEST_EQUAL(s, "ABCDEFGHIJKLMNOPQRSTUVWxyz")
RESULT


CHECK(XDRPersistenceManager::get(PointerSizeInt& p))
	PointerSizeInt p;
	pm.get(p);
	TEST_EQUAL(p, 0)
	pm.get(p);
	TEST_EQUAL(p, psi1)
	pm.get(p);
	TEST_EQUAL(p, psi2)
RESULT

pm.checkStreamTrailer();
pm.finalizeInputStream();
infile.close();

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

