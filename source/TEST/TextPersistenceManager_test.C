// $Id: TextPersistenceManager_test.C,v 1.1 2001/05/29 16:36:31 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/CONCEPT/composite.h>

///////////////////////////

START_TEST(TextPersistenceManager, "$Id: TextPersistenceManager_test.C,v 1.1 2001/05/29 16:36:31 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

CHECK(TextPersistenceManager::TextPersistenceManager())
  //BAUSTELLE
RESULT




String filename;
NEW_TMP_FILE(filename)
CHECK(TextPersistenceManager::TextPersistenceManager(std::ostream& os))
	ofstream os(filename.c_str(), std::ios::out);
	Composite comp;
	TextPersistenceManager pm(os);
	comp >> pm;
	os.close();
RESULT


CHECK(TextPersistenceManager::TextPersistenceManager(std::istream& is))
	ifstream is(filename.c_str());
	TextPersistenceManager pm(is);
	PersistentObject* po = pm.readObject();
	is.close();
	TEST_NOT_EQUAL(po, 0)
	TEST_EQUAL(RTTI::isKindOf<Composite>(*po), true)
RESULT


CHECK(TextPersistenceManager::TextPersistenceManager(std::istream& is, std::ostream& os))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeHeader(const char* type_name, const char* name, LongPointerType ptr))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkHeader(const char* type_name, const char* name, LongPointerType& ptr))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeTrailer(const char* name = 0))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkTrailer(const char* name = 0))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeStreamHeader())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeStreamTrailer())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkStreamHeader())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkStreamTrailer())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::getObjectHeader(String& type_name, LongPointerType& ptr))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeName(const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkName(const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeStorableHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkStorableHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writePrimitiveHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkPrimitiveHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeStorableTrailer())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkStorableTrailer())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writePrimitiveTrailer())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkPrimitiveTrailer())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeObjectPointerHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkObjectPointerHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeObjectReferenceHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkObjectReferenceHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size))
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::writeObjectPointerArrayTrailer())
  //BAUSTELLE
RESULT


CHECK(TextPersistenceManager::checkObjectPointerArrayTrailer())
  //BAUSTELLE
RESULT


NEW_TMP_FILE(filename)
std::ofstream outfile(filename.c_str(), std::ios::out);

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


PointerSizeInt psi1 = 0x01234567;
psi1 <<= 32;
psi1 += 0xFEDCBA98;
PointerSizeInt psi2 = 0xFEDCBA98;
psi2 <<= 32;
psi2 += 0x01234567;

CHECK(TextPersistenceManager::put(const PointerSizeInt p))
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
  TEST_REAL_EQUAL(x, (Real)0.0)
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)1.234567)
	pm.get(x);
  TEST_REAL_EQUAL(x, (Real)-9.87654e37)
RESULT


CHECK(TextPersistenceManager::get(DoubleReal& s))
	DoubleReal x;
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)0.0)
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)1.234567)
	pm.get(x);
  TEST_REAL_EQUAL(x, (DoubleReal)-9.87654e300)
RESULT


CHECK(TextPersistenceManager::get(String& s))
	String s;
	pm.get(s);
	TEST_EQUAL(s, "")
	pm.get(s);
	TEST_EQUAL(s, "ABCDEFGHIJKLMNOPQRSTUVWxyz")
RESULT


CHECK(TextPersistenceManager::get(PointerSizeInt& p))
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

