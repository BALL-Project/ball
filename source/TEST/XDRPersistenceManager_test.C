// $Id: XDRPersistenceManager_test.C,v 1.1 2000/10/30 21:51:33 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/XDRPersistenceManager.h>
#include <BALL/CONCEPT/composite.h>

///////////////////////////

START_TEST(XDRPersistenceManager, "$Id: XDRPersistenceManager_test.C,v 1.1 2000/10/30 21:51:33 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class XDRPersistenceManager::

CHECK(XDRPersistenceManager::XDRPersistenceManager())
  //BAUSTELLE
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
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeHeader(const char* type_name, const char* name, LongPointerType ptr))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkHeader(const char* type_name, const char* name, LongPointerType& ptr))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeTrailer(const char* name = 0))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkTrailer(const char* name = 0))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeStreamHeader())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeStreamTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkStreamHeader())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkStreamTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::getObjectHeader(String& type_name, LongPointerType& ptr))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeName(const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkName(const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeStorableHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkStorableHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writePrimitiveHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkPrimitiveHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeStorableTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkStorableTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writePrimitiveTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkPrimitiveTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeObjectPointerHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkObjectPointerHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeObjectReferenceHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkObjectReferenceHeader(const char* type_name, const char* name))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::writeObjectPointerArrayTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::checkObjectPointerArrayTrailer())
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const char c))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const unsigned char c))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const short s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const unsigned short s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const int s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const unsigned int s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const long s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const unsigned long s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const long long s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const unsigned long long s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const bool b))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const float f))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const double d))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const string& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::put(const void* p))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(char& c))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(unsigned char& c))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(short& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(unsigned short& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(int& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(unsigned int& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(long& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(unsigned long& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(long long& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(unsigned long long& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(bool& b))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(float& f))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(double& d))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(string& s))
  //BAUSTELLE
RESULT


CHECK(XDRPersistenceManager::get(void*& p))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

