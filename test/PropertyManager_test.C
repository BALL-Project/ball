// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/CONCEPT/property.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/protein.h>
#include <fstream>

///////////////////////////

START_TEST(PropertyManager)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

PropertyManager* p;
CHECK(PropertyManager() throw())
	p = new PropertyManager();
	TEST_NOT_EQUAL(p, 0)
RESULT

CHECK(~PropertyManager() throw())
	delete p;
RESULT

PropertyManager m;
m.setProperty("TEST_PROP", (unsigned int)123456);

CHECK(PropertyManager(const PropertyManager& property_manager) throw())
	PropertyManager* p2;
	p2 = new PropertyManager(m);
	TEST_NOT_EQUAL(p2, 0)
	TEST_EQUAL(p2->hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2->getProperty("TEST_PROP").getUnsignedInt(), 123456)	
	delete p2;
RESULT

CHECK(void clear() throw())
	PropertyManager p2(m);
	p2.clear();
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), false)
RESULT

CHECK(void set(const PropertyManager& property_manager) throw())
  TEST_EQUAL(m.countProperties(), 1)
	PropertyManager p2;
	p2.set(m);
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2.getProperty("TEST_PROP").getUnsignedInt(), 123456)	
RESULT

CHECK(const PropertyManager& operator = (const PropertyManager& property_manager) throw())
	PropertyManager p2;
	p2 = m;
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2.getProperty("TEST_PROP").getUnsignedInt(), 123456)	
RESULT

CHECK(void get(PropertyManager& property_manager) const throw())
	PropertyManager p2;
	m.get(p2);
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2.getProperty("TEST_PROP").getUnsignedInt(), 123456)	
RESULT

CHECK(void swap(PropertyManager& property_manager) throw())
	PropertyManager m2;
	m2.setProperty("PROP1", (float)4.56);
	m2.setProperty("PROP2", (string)"test");
	m2.swap(m);
	TEST_EQUAL(m2.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m2.getProperty("TEST_PROP").getUnsignedInt(), 123456)	
	TEST_EQUAL(m2.hasProperty("PROP1"), false)
	TEST_EQUAL(m2.hasProperty("PROP2"), false)

	TEST_EQUAL(m.hasProperty("PROP1"), true)
	TEST_EQUAL(m.hasProperty("PROP2"), true)
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
	TEST_REAL_EQUAL(m.getProperty("PROP1").getFloat(), 4.56)	
	TEST_EQUAL(m.getProperty("PROP2").getString(), "test")
	m2.swap(m);
RESULT

CHECK(BitVector& getBitVector() throw())
  TEST_EQUAL(m.getBitVector().getBit(0), false)
	m.getBitVector().setBit(0, true);
  TEST_EQUAL(m.getBitVector().getBit(0), true)
RESULT

CHECK(const BitVector& getBitVector() const throw())
	PropertyManager m;
  m.setProperty(5);

  const PropertyManager m2;
	TEST_EXCEPTION(Exception::IndexOverflow, m2.getBitVector().getBit(0))

  const PropertyManager m3(m);
  TEST_EQUAL(m3.getBitVector().getBit(0), false)
RESULT

CHECK(operator BitVector& () throw())
	PropertyManager m;
  m.setProperty(5);

	BitVector b = (BitVector) m;
  TEST_EQUAL(b.getBit(0), false)
  TEST_EQUAL(b.getBit(5), true)

	m.setProperty(0); 
  TEST_EQUAL(b.getBit(5), true)
  TEST_EQUAL(b.getBit(0), false)
RESULT

CHECK(void setProperty(Property property) throw())
	PropertyManager m;
  m.setProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), true)
RESULT

CHECK(void clearProperty(Property property) throw())
	PropertyManager m;
  m.setProperty(1);
  m.clearProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), false)
RESULT

CHECK(void toggleProperty(Property property) throw())
	PropertyManager m;
  m.toggleProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), true)
  m.toggleProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), false)
RESULT

CHECK(Size countProperties() const throw())
  TEST_EQUAL(m.countProperties(), 2)
	m.clear();
  TEST_EQUAL(m.countProperties(), 0)
  TEST_EQUAL(m.countNamedProperties(), 0)
RESULT

CHECK(void setProperty(const NamedProperty& property) throw())
	NamedProperty x("test", 1);
	PropertyManager m;
	m.setProperty(x);
	TEST_EQUAL(m.hasProperty("test"), true)
	TEST_EQUAL(m.getProperty("test").getInt(), 1)
	TEST_EQUAL(m.getProperty("test").getType(), NamedProperty::INT)
RESULT

CHECK(void setProperty(const string& name) throw())
	PropertyManager m;
	m.setProperty("TEST_PROP");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getBool(), false)
	TEST_EQUAL(m.getProperty("TEST_PROP").getType(), NamedProperty::NONE)
RESULT

CHECK(void setProperty(const string& name, bool value) throw())
	PropertyManager m;
	m.setProperty("TEST_PROP", true);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getBool(), true)
RESULT

CHECK(void setProperty(const string& name, int value) throw())
	PropertyManager m;
	m.setProperty("TEST_PROP", (int)-56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getInt(), -56789)
RESULT

CHECK(void setProperty(const string& name, unsigned int value) throw())
	PropertyManager m;
	m.setProperty("TEST_PROP", (unsigned int)56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getUnsignedInt(), 56789)
RESULT

CHECK(void setProperty(const string& name, float value) throw())
	PropertyManager m;
	m.setProperty("TEST_PROP", (float)0.56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_REAL_EQUAL(m.getProperty("TEST_PROP").getFloat(), 0.56789)
RESULT

CHECK(void setProperty(const string& name, double value) throw())
	PropertyManager m;
	m.setProperty("TEST_PROP", (double)0.56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_REAL_EQUAL(m.getProperty("TEST_PROP").getDouble(), 0.56789)
RESULT

CHECK(void setProperty(const string& name, const string& value) throw())
	PropertyManager m;
	m.setProperty("TEST_PROP", (string)"xxx");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getString(), "xxx")
RESULT

CHECK(void setProperty(const string& name, const PersistentObject& value) throw())
	PropertyManager m;
	PersistentObject po;
	m.setProperty("TEST_PROP", po);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getObject(), &po)
RESULT

CHECK(const NamedProperty& getProperty(const string& name) const throw())
	PropertyManager m;
	PersistentObject po;
	TEST_EQUAL(m.getProperty("TEST_PROP").getObject(), 0)
	TEST_EQUAL(m.getProperty("TEST_PROP").getType(), NamedProperty::NONE)
	m.setProperty("TEST_PROP", po);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getObject(), &po)
RESULT

CHECK(void clearProperty(const string& name) throw())
	PropertyManager m;
	PersistentObject po;
	m.setProperty("TEST_PROP", po);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	m.clearProperty("");
	m.clearProperty("TEST_PROP");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
RESULT

CHECK(Size countNamedProperties() const throw())
	PropertyManager m;
	TEST_EQUAL(m.countNamedProperties(), 0)
	m.setProperty("TEST_PROP1", 0);
	TEST_EQUAL(m.countNamedProperties(), 1)
	m.setProperty("TEST_PROP2", string("ASD"));
	TEST_EQUAL(m.countNamedProperties(), 2)
	m.clearProperty("TEST_PROP2");
	TEST_EQUAL(m.countNamedProperties(), 1)
	m.setProperty("TEST_PROP1", "");
	TEST_EQUAL(m.countNamedProperties(), 1)
RESULT

CHECK(const NamedProperty& getNamedProperty(Position index) const throw(Exception::IndexOverflow))
	PropertyManager m;
	m.setProperty("TEST_PROP1", 123);
	m.setProperty("TEST_PROP2", string("ASD"));
	m.setProperty("TEST_PROP3", true);
	TEST_EQUAL(m.countNamedProperties(), 3)

	const PropertyManager& c_m(m);
	TEST_EQUAL(c_m.getNamedProperty(0).getInt(), 123)
	TEST_EQUAL(c_m.getNamedProperty(1).getString(), "ASD")
	TEST_EQUAL(c_m.getNamedProperty(2).getBool(), true)
	TEST_EXCEPTION(Exception::IndexOverflow, c_m.getNamedProperty(3))
RESULT

CHECK(NamedProperty& getNamedProperty(Position index) throw(Exception::IndexOverflow))
	PropertyManager m;
	m.setProperty("TEST_PROP1", 123);
	m.setProperty("TEST_PROP2", string("ASD"));
	m.setProperty("TEST_PROP3", true);
	TEST_EQUAL(m.countNamedProperties(), 3)

	TEST_EQUAL(m.getNamedProperty(0).getInt(), 123)
	TEST_EQUAL(m.getNamedProperty(1).getString(), "ASD")
	TEST_EQUAL(m.getNamedProperty(2).getBool(), true)
	TEST_EXCEPTION(Exception::IndexOverflow, m.getNamedProperty(3))
RESULT

CHECK(bool hasProperty(Property property) const throw())
	PropertyManager m;
	TEST_EQUAL(m.hasProperty(1), false)
	TEST_EQUAL(m.hasProperty(0), false)
	m.setProperty(0);
	TEST_EQUAL(m.hasProperty(0), true)
	m.clear();
	TEST_EQUAL(m.hasProperty(0), false)
RESULT

CHECK(bool hasProperty(const string& name) const throw())
	PropertyManager m;
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
	TEST_EQUAL(m.hasProperty(""), false)
	m.setProperty("TEST_PROP", 0);
	m.clearProperty("TEST_PROP");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
RESULT

String filename;
TextPersistenceManager pm;

CHECK(void write(PersistenceManager& pm) const throw())
	NEW_TMP_FILE(filename)
	String str("test");
	PersistentObject ob;
	m.setProperty("PROP1", true);
  m.setProperty("PROP2", -12345);
  m.setProperty("PROP3", (unsigned int)12345);
  m.setProperty("PROP4", (float)1.2345);
  m.setProperty("PROP5", (double) 2.345);
  m.setProperty("PROP6", str);
  m.setProperty("PROP7", ob);
  m.setProperty("PROP8");
  m.setProperty(0);
  m.setProperty(2);
	ofstream  ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	m.write(pm);
	ofile.close();	
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(PropertyManager_test/PropertyManager_test_write.txt))
RESULT


CHECK(bool read(PersistenceManager& pm) throw())
	PropertyManager m;
	ifstream  ifile(BALL_TEST_DATA_PATH(PropertyManager_test/PropertyManager_test_read.txt));
	pm.setIstream(ifile);
	TEST_EQUAL(m.read(pm), true)
	TEST_EQUAL(m.hasProperty("PROP1"), true)
	TEST_EQUAL(m.hasProperty("PROP2"), true)
	TEST_EQUAL(m.hasProperty("PROP3"), true)
	TEST_EQUAL(m.hasProperty("PROP4"), true)
	TEST_EQUAL(m.hasProperty("PROP5"), true)
	TEST_EQUAL(m.hasProperty("PROP6"), true)
	TEST_EQUAL(m.hasProperty("PROP7"), true)
	TEST_EQUAL(m.hasProperty("PROP8"), true)
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
	TEST_EQUAL(m.getProperty("PROP1").getBool(), true)	
	TEST_EQUAL(m.getProperty("PROP2").getInt(), -12345)
	TEST_EQUAL(m.getProperty("PROP3").getUnsignedInt(), 12345)
	TEST_REAL_EQUAL(m.getProperty("PROP4").getFloat(), 1.2345)
	TEST_REAL_EQUAL(m.getProperty("PROP5").getDouble(), 2.345)
	TEST_EQUAL(m.getProperty("PROP6").getString(), "test")
	TEST_NOT_EQUAL(m.getProperty("PROP7").getObject(), 0)
	TEST_EQUAL(m.countNamedProperties(), 8);
	TEST_EQUAL(m.getBitVector().getBit(0), true)
	TEST_EQUAL(m.getBitVector().getBit(1), false)
	TEST_EQUAL(m.getBitVector().getBit(2), true)
	ifile.close();
RESULT


CHECK(bool isValid() const throw())
	TEST_EQUAL(m.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	m.dump(outstr); 
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(PropertyManager_test/PropertyManager_test_dump.txt))
RESULT


CHECK(bool operator != (const PropertyManager& pm) const throw())
  // rudimentary tests
	PropertyManager pm1;
	PropertyManager pm2;
	bool result = (pm1 != pm2);
	TEST_NOT_EQUAL(result, true)
	pm1.setProperty("TEST_PROP1", 123);
	result = (pm1 != pm2);
	TEST_EQUAL(result, true)
RESULT

CHECK(bool operator == (const PropertyManager& pm) const throw())
  // rudimentary tests
	PropertyManager pm1;
	PropertyManager pm2;
	bool result = (pm1 == pm2);
	TEST_EQUAL(result, true)
	pm1.setProperty("TEST_PROP1", 123);
	result = (pm1 == pm2);
	TEST_NOT_EQUAL(result, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
