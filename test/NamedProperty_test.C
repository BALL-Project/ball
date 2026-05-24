// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/CONCEPT/property.h>
#include <BALL/CONCEPT/persistentObject.h>
#include <fstream>
///////////////////////////

START_TEST(PropertyManager)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

NamedProperty* np;

CHECK(NamedProperty())
	np = new NamedProperty();
	TEST_NOT_EQUAL(np, 0)
RESULT

CHECK(~NamedProperty())
	delete np;
RESULT

CHECK(NamedProperty(const string& name))
	np = new NamedProperty("name");
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getName(), "name")
	delete np;
RESULT

CHECK(NamedProperty(const string& name, bool value))
	bool x = true;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::BOOL)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getBool(), x)
RESULT

CHECK(BALL_CREATE(NamedProperty))
	NamedProperty* np2 = (NamedProperty*)np->create();
	TEST_EQUAL(np2->getType(), NamedProperty::BOOL)
	TEST_EQUAL(np2->getName(), "test")
	TEST_EQUAL(np2->getBool(), true)
	delete np;
	delete np2;
RESULT

CHECK(NamedProperty(const string& name, int value))
	int x = -99;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::INT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getInt(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, unsigned int value))
	unsigned int x = 99;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::UNSIGNED_INT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getUnsignedInt(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, double value))
	double x = -99.9;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::DOUBLE)
	TEST_EQUAL(np->getName(), "test")
	TEST_REAL_EQUAL(np->getDouble(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, const string& str))
	String x = "xxx";
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::STRING)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getString(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, PersistentObject& po))
	PersistentObject x;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::OBJECT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getObject(), &x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, boost::shared_ptr<PersistentObject>& po))
	boost::shared_ptr<PersistentObject> p(new PersistentObject());
  np = new NamedProperty("test", p);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::SMART_OBJECT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getSmartObject(), p)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, float value))
	float x = -99.9;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::FLOAT)
	TEST_EQUAL(np->getName(), "test")
	TEST_REAL_EQUAL(np->getFloat(), x)
RESULT

CHECK(NamedProperty(const NamedProperty&))
	float x = -99.9;
	NamedProperty np2(*np);
	TEST_EQUAL(np2.getType(), NamedProperty::FLOAT)
	TEST_EQUAL(np2.getName(), "test")
	TEST_REAL_EQUAL(np2.getFloat(), x)
	
	NamedProperty np3(String("TESTPROP"), String("STR"));
	TEST_EQUAL(np3.getType(), NamedProperty::STRING)
	TEST_EQUAL(np3.getString(), "STR")
	NamedProperty np4(np3);
	TEST_EQUAL(np4.getType(), NamedProperty::STRING)
	TEST_EQUAL(np4.getString(), "STR")
	np3.clear();
	TEST_EQUAL(np3.getType(), NamedProperty::NONE)
	TEST_EQUAL(np4.getType(), NamedProperty::STRING)
	TEST_EQUAL(np4.getString(), "STR")
	np4.clear();
	TEST_EQUAL(np4.getType(), NamedProperty::NONE)
RESULT

CHECK(void clear())
	//
RESULT

String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;

CHECK(Type getType() const)
  //TESTED ABOVE
RESULT

CHECK(string getName() const)
  //TESTED ABOVE
RESULT

CHECK(bool getBool() const)
  //TESTED ABOVE
RESULT

CHECK(int getInt() const)
  //TESTED ABOVE
RESULT

CHECK(float getFloat() const)
  //TESTED ABOVE
RESULT

CHECK(double getDouble() const)
  //TESTED ABOVE
RESULT

CHECK(unsigned int getUnsignedInt() const)
  //TESTED ABOVE
RESULT

CHECK(PersistentObject* getObject() const)
  //TESTED ABOVE
RESULT

CHECK(string getString() const)
  //TESTED ABOVE
RESULT

CHECK(bool operator != (const NamedProperty& np) const)
  // ???
RESULT

CHECK(bool operator == (const NamedProperty& np) const)
	// ???
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
