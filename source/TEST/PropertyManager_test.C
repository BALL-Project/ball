// $Id: PropertyManager_test.C,v 1.9 2000/08/25 20:09:58 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/property.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/protein.h>

///////////////////////////

START_TEST(class_name, "$Id: PropertyManager_test.C,v 1.9 2000/08/25 20:09:58 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

NamedProperty* np;

CHECK(NamedProperty::NamedProperty())
	np = new NamedProperty();
	TEST_NOT_EQUAL(np, 0)
RESULT

CHECK(NamedProperty::~NamedProperty())
	delete np;
RESULT

CHECK(NamedProperty::NamedProperty(const string& name))
	np = new NamedProperty("name");
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getName(), "name")
RESULT

CHECK(NamedProperty::NamedProperty(const string& name, bool value))
	bool x = true;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::BOOL)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getBool(), x)
RESULT

CHECK(NamedProperty::BALL_CREATE_NODEEP(NamedProperty) const  const )
	NamedProperty* np2 = (NamedProperty*)np->create();
	TEST_EQUAL(np2->getType(), NamedProperty::BOOL)
	TEST_EQUAL(np2->getName(), "test")
	TEST_EQUAL(np2->getBool(), true)

	delete np;
RESULT

CHECK(NamedProperty::NamedProperty(const string& name, int value))
	int x = -99;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::INT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getInt(), x)
	delete np;
RESULT

CHECK(NamedProperty::NamedProperty(const string& name, unsigned int value))
	unsigned int x = 99;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::UNSIGNED_INT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getUnsignedInt(), x)
	delete np;
RESULT

CHECK(NamedProperty::NamedProperty(const string& name, double value))
	double x = -99.9;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::DOUBLE)
	TEST_EQUAL(np->getName(), "test")
	TEST_REAL_EQUAL(np->getDouble(), x)
	delete np;
RESULT

CHECK(NamedProperty::NamedProperty(const string& name, string& str))
	string x = "xxx";
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::STRING)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getString(), x)
	delete np;
RESULT

CHECK(NamedProperty::NamedProperty(const string& name, PersistentObject& po))
	PersistentObject x;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::OBJECT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getObject(), &x)
	delete np;
RESULT

CHECK(NamedPr operty::NamedProperty(const string& name, float value))
	float x = -99.9;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::FLOAT)
	TEST_EQUAL(np->getName(), "test")
	TEST_REAL_EQUAL(np->getFloat(), x)
RESULT

CHECK(NamedProperty::NamedProperty(const NamedProperty&))
	float x = -99.9;
	NamedProperty* np2;
  np2 = new NamedProperty(*np);
	TEST_NOT_EQUAL(np2, 0)
	TEST_EQUAL(np2->getType(), NamedProperty::FLOAT)
	TEST_EQUAL(np2->getName(), "test")
	TEST_REAL_EQUAL(np2->getFloat(), x)
RESULT

String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;
TextPersistenceManager pm;

CHECK(NamedProperty::persistentWrite(PersistenceManager& pm, const char* name = "") const )
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), File::OUT);
	pm.setOstream(ofile);
	np->persistentWrite(pm, "TEST");
	ofile.close();	
	TEST_FILE(filename.c_str(), "data/NamedProperty_test.txt", true)
	delete np;

	Protein protein("PROTEIN1");
	np = new NamedProperty("test2", protein);
	NEW_TMP_FILE(filename)
	ofile.open(filename.c_str());
	pm.setOstream(ofile);
	np->persistentWrite(pm, "TEST");
	ofile.close();	
	TEST_FILE(filename.c_str(), "data/NamedProperty_test0.txt", true)
RESULT

CHECK(NamedProperty::persistentRead(PersistenceManager& pm))
	NamedProperty* np;
	ifstream  ifile("data/NamedProperty_test.txt");
	pm.setIstream(ifile);
	np->persistentRead(pm);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::FLOAT)
	TEST_EQUAL(np->getName(), "test")
	TEST_REAL_EQUAL(np->getFloat(), -99.9)
	ifile.close();
	delete np;

	ifile.open("data/NamedProperty_test0.txt");
	pm.setIstream(ifile);
	np->persistentRead(pm);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::OBJECT)
	TEST_EQUAL(np->getName(), "test2")
	TEST_NOT_EQUAL(np->getObject(), 0)
	ifile.close();
RESULT

CHECK(NamedProperty::getType() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getName() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getBool() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getInt() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getFloat() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getDouble() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getUnsignedInt() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getObject() const )
  //TESTED ABOVE
RESULT

CHECK(NamedProperty::getString() const )
  //TESTED ABOVE
RESULT

CHECK(friend std::ostream& operator << (std::ostream& s, const NamedProperty& property))
	PersistentObject po;
	string str("test");
	Protein protein("PROTEIN1");
	NamedProperty np1("NP1", true);
	NamedProperty np2("NP2", (int)-1234);
	NamedProperty np3("NP3", (unsigned int) 2345);
	NamedProperty np4("NP4", (float) 1.234);
	NamedProperty np5("NP5", (double) 2.34);
	NamedProperty np6("NP6", str);
	NamedProperty np7("NP7", protein);
	NamedProperty np8("NP8");

	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << np1;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test1.txt", false)

	NEW_TMP_FILE(filename)
	outstr.open(filename.c_str());
	outstr << np2;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test2.txt", false)

	NEW_TMP_FILE(filename)
	outstr.open(filename.c_str());
	outstr << np3;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test3.txt", false)

	NEW_TMP_FILE(filename)
	outstr.open(filename.c_str());
	outstr << np4;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test4.txt", false)

	NEW_TMP_FILE(filename)
	outstr.open(filename.c_str());
	outstr << np5;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test5.txt", false)

	NEW_TMP_FILE(filename)
	outstr.open(filename.c_str());
	outstr << np6;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test6.txt", false)

	NEW_TMP_FILE(filename)
	outstr.open(filename.c_str());
	outstr << np7;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test7.txt", false)

	NEW_TMP_FILE(filename)
	outstr.open(filename.c_str());
	outstr << np8;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/NamedProperty_test8.txt", false)
RESULT

CHECK(friend std::ostream& operator << (std::ostream& s, const NamedProperty& property))
	PersistentObject po;
	NamedProperty np;

	std::ifstream instr("data/NamedProperty_test1.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::BOOL)
	TEST_EQUAL(np.getBool(), true)
	TEST_EQUAL(np.getName(), "NP1")

	instr.open("data/NamedProperty_test2.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::INT)
	TEST_EQUAL(np.getInt(), -1234)
	TEST_EQUAL(np.getName(), "NP2")

	instr.open("data/NamedProperty_test3.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::UNSIGNED_INT)
	TEST_EQUAL(np.getUnsignedInt(), 2345)
	TEST_EQUAL(np.getName(), "NP3")

	instr.open("data/NamedProperty_test4.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::FLOAT)
	TEST_REAL_EQUAL(np.getFloat(), 1.234)
	TEST_EQUAL(np.getName(), "NP4")

	instr.open("data/NamedProperty_test5.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::DOUBLE)
	TEST_EQUAL(np.getDouble(), 2.34)
	TEST_EQUAL(np.getName(), "NP5")

	instr.open("data/NamedProperty_test6.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::STRING)
	TEST_EQUAL(np.getString(), "test")
	TEST_EQUAL(np.getName(), "NP6")

	instr.open("data/NamedProperty_test7.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::OBJECT)
	TEST_NOT_EQUAL(np.getObject(), 0)
	TEST_EQUAL(np.getName(), "NP7")

	instr.open("data/NamedProperty_test8.txt");
	instr >> np;
	instr.close();
	TEST_EQUAL(np.getType(), NamedProperty::NONE)
	TEST_EQUAL(np.getName(), "NP8")
RESULT




PropertyManager* p;
CHECK(PropertyManager())
	p = new PropertyManager();
	TEST_NOT_EQUAL(p, 0)
RESULT

CHECK(~PropertyManager())
	delete p;
RESULT

PropertyManager m;

CHECK(setProperty(String&, int))
  TEST_EQUAL(m.hasProperty(0), false) //segfault
  TEST_EQUAL(m.countProperties(), 0)

	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
	m.setProperty("TEST_PROP", (unsigned int)123456);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getUnsignedInt(), 123456)
RESULT

CHECK(PropertyManager::PropertyManager(const PropertyManager& property_manager, bool deep = true))
	PropertyManager* p2;
	p2 = new PropertyManager(m);
	TEST_NOT_EQUAL(p2, 0)
	TEST_EQUAL(p2->hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2->getProperty("TEST_PROP").getUnsignedInt(), 123456)	
RESULT

CHECK(PropertyManager::clear())
	PropertyManager p2(m);
	p2.clear();
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), false)
RESULT

CHECK(PropertyManager::destroy())
	PropertyManager p2(m);
	p2.destroy();
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), false)
RESULT

CHECK(PropertyManager::set(const PropertyManager& property_manager, bool deep = true))
  TEST_EQUAL(m.countProperties(), 1)
	PropertyManager p2;
	p2.set(m);
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2.getProperty("TEST_PROP").getUnsignedInt(), 123456)	
RESULT

CHECK(PropertyManager::PropertyManager& operator = (const PropertyManager& property_manager))
	PropertyManager p2;
	p2 = m;
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2.getProperty("TEST_PROP").getUnsignedInt(), 123456)	
RESULT

CHECK(PropertyManager::get(PropertyManager& property_manager, bool deep = true) const )
	PropertyManager p2;
	m.get(p2);
	TEST_EQUAL(p2.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(p2.getProperty("TEST_PROP").getUnsignedInt(), 123456)	
RESULT

CHECK(PropertyManager::swap(PropertyManager& property_manager))
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

CHECK(PropertyManager::getBitVector())
  TEST_EQUAL(m.getBitVector().getBit(0), false)
	m.getBitVector().setBit(0, true);
  TEST_EQUAL(m.getBitVector().getBit(0), true)
RESULT

CHECK(PropertyManager::getBitVector() const )
	PropertyManager m;
  m.setProperty(5);

  const PropertyManager m2;
	TEST_EXCEPTION(Exception::IndexOverflow, m2.getBitVector().getBit(0))

  const PropertyManager m3(m);
  TEST_EQUAL(m3.getBitVector().getBit(0), false)
RESULT

CHECK(PropertyManager:: operator BitVector& ())
	PropertyManager m;
  m.setProperty(5);

	BitVector b = (BitVector) m;
  TEST_EQUAL(b.getBit(0), false)

	//m.setProperty(0, true); // seg fault ???
  TEST_EQUAL(b.getBit(5), true)
RESULT

CHECK(PropertyManager::setProperty(Property property))
	PropertyManager m;
  m.setProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), true)
RESULT

CHECK(PropertyManager::clearProperty(Property property))
	PropertyManager m;
  m.setProperty(1);
  m.clearProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), false)
RESULT

CHECK(PropertyManager::toggleProperty(Property property))
	PropertyManager m;
  m.toggleProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), true)
  m.toggleProperty(1);
  TEST_EQUAL(m.getBitVector().getBit(1), false)
RESULT

CHECK(PropertyManager::countProperties() const )
  TEST_EQUAL(m.countProperties(), 2)
	m.clear();
  TEST_EQUAL(m.countProperties(), 0)
  TEST_EQUAL(m.countNamedProperties(), 0)
RESULT

CHECK(PropertyManager::setProperty(const NamedProperty& property))
	NamedProperty x("test", 1);
	PropertyManager m;
	m.setProperty(x);
	TEST_EQUAL(m.hasProperty("test"), true)
	TEST_EQUAL(m.getProperty("test").getInt(), 1)
	TEST_EQUAL(m.getProperty("test").getType(), NamedProperty::INT)
RESULT

CHECK(PropertyManager::setProperty(const string& name))
	PropertyManager m;
	m.setProperty("TEST_PROP");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getBool(), false)
	TEST_EQUAL(m.getProperty("TEST_PROP").getType(), NamedProperty::NONE)
RESULT

CHECK(PropertyManager::setProperty(const string& name, bool value))
	PropertyManager m;
	m.setProperty("TEST_PROP", true);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getBool(), true)
RESULT

CHECK(PropertyManager::setProperty(const string& name, int value))
	PropertyManager m;
	m.setProperty("TEST_PROP", (int)-56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getInt(), -56789)
RESULT

CHECK(PropertyManager::setProperty(const string& name, unsigned int value))
	PropertyManager m;
	m.setProperty("TEST_PROP", (unsigned int)56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getUnsignedInt(), 56789)
RESULT

CHECK(setProperty(String&, float))
	PropertyManager m;
	m.setProperty("TEST_PROP", (float)0.56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_REAL_EQUAL(m.getProperty("TEST_PROP").getFloat(), 0.56789)
RESULT

CHECK(PropertyManager::setProperty(const string& name, double value))
	PropertyManager m;
	m.setProperty("TEST_PROP", (double)0.56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_REAL_EQUAL(m.getProperty("TEST_PROP").getDouble(), 0.56789)
RESULT

CHECK(PropertyManager::setProperty(const string& name, const string& value))
	PropertyManager m;
	m.setProperty("TEST_PROP", (string)"xxx");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getString(), "xxx")
RESULT

CHECK(PropertyManager::setProperty(const string& name, const PersistentObject& value))
	PropertyManager m;
	PersistentObject po;
	m.setProperty("TEST_PROP", po);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getObject(), &po)
RESULT

CHECK(PropertyManager::getProperty(const string& name) const )
	PropertyManager m;
	PersistentObject po;
	TEST_EQUAL(m.getProperty("TEST_PROP").getObject(), 0)
	TEST_EQUAL(m.getProperty("TEST_PROP").getType(), NamedProperty::NONE)
	m.setProperty("TEST_PROP", po);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getObject(), &po)
RESULT

CHECK(PropertyManager::clearProperty(const string& name))
	PropertyManager m;
	PersistentObject po;
	m.setProperty("TEST_PROP", po);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	m.clearProperty("");
	m.clearProperty("TEST_PROP");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
RESULT

CHECK(PropertyManager::countNamedProperties() const )
	PropertyManager m;
	TEST_EQUAL(m.countNamedProperties(), 0)
	m.setProperty("TEST_PROP1", 0);
	TEST_EQUAL(m.countNamedProperties(), 1)
	m.setProperty("TEST_PROP2", "ASD");
	TEST_EQUAL(m.countNamedProperties(), 2)
	m.clearProperty("TEST_PROP2");
	TEST_EQUAL(m.countNamedProperties(), 1)
	m.setProperty("TEST_PROP1", "");
	TEST_EQUAL(m.countNamedProperties(), 1)
RESULT

CHECK(PropertyManager::hasProperty(Property property) const )
	PropertyManager m;
	TEST_EQUAL(m.hasProperty(1), false)
	TEST_EQUAL(m.hasProperty(0), false)
	m.setProperty(0);
	TEST_EQUAL(m.hasProperty(0), true)
	m.clear();
	TEST_EQUAL(m.hasProperty(0), false)
RESULT

CHECK(PropertyManager::hasProperty(const string& name) const )
	PropertyManager m;
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
	TEST_EQUAL(m.hasProperty(""), false)
	m.setProperty("TEST_PROP", 0);
	m.clearProperty("TEST_PROP");
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
RESULT

CHECK(PropertyManager::std::ostream& operator << (std::ostream& s, const PropertyManager& property_manager))
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), File::OUT);
	string str("test");
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
	outstr << m;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/PropertyManager_test.txt", false)
RESULT

CHECK(PropertyManager::std::istream& operator >> (std::istream& s, PropertyManager& property_manager))
	PropertyManager m;
	std::ifstream instr("data/PropertyManager_test.txt");
	instr >> m;
	instr.close();
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
RESULT

CHECK(PropertyManager::write(PersistenceManager& pm) const )
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), File::OUT);
	pm.setOstream(ofile);
	m.write(pm);
	ofile.close();	
	TEST_FILE(filename.c_str(), "data/PropertyManager_test2.txt", true)
RESULT

CHECK(PropertyManager::read(PersistenceManager& pm))
	PropertyManager m;
	ifstream  ifile("data/PersistenceManager_test2.txt");
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
	TEST_EQUAL(m.getProperty("PROP4").getFloat(), 1.2345)
	TEST_EQUAL(m.getProperty("PROP5").getDouble(), 2.345)
	TEST_EQUAL(m.getProperty("PROP6").getString(), "test")
	TEST_NOT_EQUAL(m.getProperty("PROP7").getObject(), 0)
	TEST_EQUAL(m.countNamedProperties(), 7);
	TEST_EQUAL(m.getBitVector().getBit(0), true)
	TEST_EQUAL(m.getBitVector().getBit(1), false)
	TEST_EQUAL(m.getBitVector().getBit(2), true)
	ifile.close();
RESULT

CHECK(PropertyManager::isValid() const )
	TEST_EQUAL(m.isValid(), true)
RESULT

CHECK(PropertyManager::dump(std::ostream& s = std::cout, Size depth = 0) const )
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), File::OUT);
	m.dump(outstr);
	TEST_FILE(filename.c_str(), "data/PropertyManager_test3.txt", true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
