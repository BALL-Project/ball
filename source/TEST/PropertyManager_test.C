// $Id: PropertyManager_test.C,v 1.3 2000/06/27 07:59:31 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/property.h>
///////////////////////////

START_TEST(class_name, "$Id: PropertyManager_test.C,v 1.3 2000/06/27 07:59:31 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PropertyManager* p;
	CHECK(PropertyManager())
	p = new PropertyManager();
	TEST_NOT_EQUAL(p, 0)
RESULT

CHECK(~PropertyManager())
	delete p;
RESULT

CHECK(setProperty(String&, float))
	PropertyManager m;
	m.setProperty("TEST_PROP", (float)0.56789);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_REAL_EQUAL(m.getProperty("TEST_PROP").getFloat(), 0.56789)
RESULT

CHECK(setProperty(String&, int))
	PropertyManager m;
	TEST_EQUAL(m.hasProperty("TEST_PROP"), false)
	m.setProperty("TEST_PROP", (unsigned int)123456);
	TEST_EQUAL(m.hasProperty("TEST_PROP"), true)
	TEST_EQUAL(m.getProperty("TEST_PROP").getUnsignedInt(), 123456)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

/*
// SECTION: Constructors and Destructors
  PropertyManager(const PropertyManager& property_manager, bool deep = true);
  void clear();
  void destroy();

// SECTION: Assignment 
  void set(const PropertyManager& property_manager, bool deep = true);
  PropertyManager& operator = (const PropertyManager& property_manager);
  void get(PropertyManager& property_manager, bool deep = true) const;
  void swap(PropertyManager& property_manager);

// SECTION: Accessors 
  BitVector& getBitVector();
  const BitVector& getBitVector() const;

// SECTION: Converters 
  operator BitVector& ();

// SECTION: Handling unnamed bit properties 
  void setProperty(Property property);
  void clearProperty(Property property);
  void toggleProperty(Property property);
  Size countProperties() const;

// SECTION: Handling named properties 
  void setProperty(const NamedProperty& property);
  void setProperty(const string& name);
  void setProperty(const string& name, bool value);
  void setProperty(const string& name, int value);
  void setProperty(const string& name, unsigned int value);
  void setProperty(const string& name, float value);
  void setProperty(const string& name, const string& value);
  void setProperty(const string& name, const PersistentObject& value);
  const NamedProperty& getProperty(const string& name) const;
  void clearProperty(const string& name);
  Size countNamedProperties() const;

// SECTION: Predicates 
  bool hasProperty(Property property) const;
  bool hasProperty(const string& name) const;

// SECTION: Storers
  friend ostream& operator << (ostream& s, const PropertyManager& property_manager);
  friend istream& operator >> (istream& s, PropertyManager& property_manager);

// SECTION: Storable Model
  void write(PersistenceManager& pm) const;
  bool read(PersistenceManager& pm);

// SECTION: Debugging and Diagnostics
  bool isValid() const;
  void dump(ostream& s = cout, Size depth = 0) const;
*/
