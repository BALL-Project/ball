// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: NamedProperty_test.C,v 1.2 2004/11/07 08:25:37 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/property.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/protein.h>
#include <fstream>
///////////////////////////

START_TEST(PropertyManager, "$Id: NamedProperty_test.C,v 1.2 2004/11/07 08:25:37 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

NamedProperty* np;

CHECK(NamedProperty() throw())
	np = new NamedProperty();
	TEST_NOT_EQUAL(np, 0)
RESULT

CHECK(~NamedProperty() throw())
	delete np;
RESULT

CHECK(NamedProperty(const string& name) throw())
	np = new NamedProperty("name");
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getName(), "name")
	delete np;
RESULT

CHECK(NamedProperty(const string& name, bool value) throw())
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

CHECK(NamedProperty(const string& name, int value) throw())
	int x = -99;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::INT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getInt(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, unsigned int value) throw())
	unsigned int x = 99;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::UNSIGNED_INT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getUnsignedInt(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, double value) throw())
	double x = -99.9;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::DOUBLE)
	TEST_EQUAL(np->getName(), "test")
	TEST_REAL_EQUAL(np->getDouble(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, const string& str) throw())
	string x = "xxx";
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::STRING)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getString(), x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, PersistentObject& po) throw())
	PersistentObject x;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::OBJECT)
	TEST_EQUAL(np->getName(), "test")
	TEST_EQUAL(np->getObject(), &x)
	delete np;
RESULT

CHECK(NamedProperty(const string& name, float value) throw())
	float x = -99.9;
  np = new NamedProperty("test", x);
	TEST_NOT_EQUAL(np, 0)
	TEST_EQUAL(np->getType(), NamedProperty::FLOAT)
	TEST_EQUAL(np->getName(), "test")
	TEST_REAL_EQUAL(np->getFloat(), x)
RESULT

CHECK(NamedProperty(const NamedProperty&) throw())
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

CHECK(void clear() throw())
	//
RESULT

String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;
TextPersistenceManager pm;

CHECK(void persistentWrite(PersistenceManager& pm, const char* name = "") const throw(Exception::GeneralException))
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	*np >> pm;
	ofile.close();	
	TEST_FILE_REGEXP(filename.c_str(), "data/PropertyManager_test/NamedProperty_test_Float11.txt")
	delete np;

	Protein protein("PROTEIN1");
	np = new NamedProperty("test2", protein);
	NEW_TMP_FILE(filename)
	ofile.open(filename.c_str());
	pm.setOstream(ofile);
	*np >> pm;
	ofile.close();	
	TEST_FILE_REGEXP(filename.c_str(), "data/PropertyManager_test/NamedProperty_test_Object11.txt")
	delete np;

	np = new NamedProperty("test3");
	NEW_TMP_FILE(filename)
	ofile.open(filename.c_str());
	pm.setOstream(ofile);
	*np >> pm;
	ofile.close();	
	TEST_FILE_REGEXP(filename.c_str(), "data/PropertyManager_test/NamedProperty_test_None11.txt")
	delete np;

	string s("titel");
	np = new NamedProperty("test4", s);
	NEW_TMP_FILE(filename)
	ofile.open(filename.c_str());
	pm.setOstream(ofile);
	*np >> pm;
	ofile.close();	
	TEST_FILE_REGEXP(filename.c_str(), "data/PropertyManager_test/NamedProperty_test_String11.txt")
	delete np;
RESULT

CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	NamedProperty np;
	PersistentObject* ptr;
	ifstream  ifile("data/PropertyManager_test/NamedProperty_test_Float1.txt");
	pm.setIstream(ifile);

	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<NamedProperty>(*ptr), true)
		if (isKindOf<NamedProperty>(*ptr))
		{
			NamedProperty* pers_a = castTo<NamedProperty>(*ptr);
			TEST_EQUAL(pers_a->getType(), NamedProperty::FLOAT)
			TEST_EQUAL(pers_a->getName(), "test")
			TEST_REAL_EQUAL(pers_a->getFloat(), (float)-99.9)
		}
		delete ptr;
	}

	// due to some problems in the IRIX/CC fstream implementation....
	ifile.clear();
	ifile.open("data/PropertyManager_test/NamedProperty_test_Object1.txt");
	ptr = pm.readObject();
	ifile.close();

	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<NamedProperty>(*ptr), true)
		if (isKindOf<NamedProperty>(*ptr))
		{
			NamedProperty* pers_a = castTo<NamedProperty>(*ptr);
			TEST_EQUAL(pers_a->getType(), NamedProperty::OBJECT)
			TEST_EQUAL(pers_a->getName(), "test2")
			TEST_NOT_EQUAL(pers_a->getObject(), 0)
			delete pers_a->getObject();
		}
		delete ptr;
	}
	
	// due to some problems in the IRIX/CC fstream implementation....
	ifile.clear();
	ifile.open("data/PropertyManager_test/NamedProperty_test_None1.txt");
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<NamedProperty>(*ptr), true)
		if (isKindOf<NamedProperty>(*ptr))
		{
			NamedProperty* pers_a = castTo<NamedProperty>(*ptr);
			TEST_EQUAL(pers_a->getType(), NamedProperty::NONE)
			TEST_EQUAL(pers_a->getName(), "test3")
		}
		delete ptr;
	}

	// due to some problems in the IRIX/CC fstream implementation....
	ifile.clear();
	ifile.open("data/PropertyManager_test/NamedProperty_test_String1.txt");
	ptr = pm.readObject();
	ifile.close();

	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<NamedProperty>(*ptr), true)
		if (isKindOf<NamedProperty>(*ptr))
		{
			NamedProperty* pers_a = castTo<NamedProperty>(*ptr);
			TEST_EQUAL(pers_a->getType(), NamedProperty::STRING)
			TEST_EQUAL(pers_a->getString(), "titel")
			TEST_EQUAL(pers_a->getName(), "test4")
		}
		delete ptr;
	}
RESULT

CHECK(Type getType() const throw())
  //TESTED ABOVE
RESULT

CHECK(string getName() const throw())
  //TESTED ABOVE
RESULT

CHECK(bool getBool() const throw())
  //TESTED ABOVE
RESULT

CHECK(int getInt() const throw())
  //TESTED ABOVE
RESULT

CHECK(float getFloat() const throw())
  //TESTED ABOVE
RESULT

CHECK(double getDouble() const throw())
  //TESTED ABOVE
RESULT

CHECK(unsigned int getUnsignedInt() const throw())
  //TESTED ABOVE
RESULT

CHECK(PersistentObject* getObject() const throw())
  //TESTED ABOVE
RESULT

CHECK(string getString() const throw())
  //TESTED ABOVE
RESULT

CHECK(bool operator != (const NamedProperty& np) const throw())
  // ???
RESULT

CHECK(bool operator == (const NamedProperty& np) const throw())
	// ???
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
