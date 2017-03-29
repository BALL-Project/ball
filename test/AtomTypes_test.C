// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

// insert includes here
#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <BALL/FORMAT/parameters.h>


///////////////////////////

START_TEST(class_name)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
AtomTypes* at_ptr = 0;
Parameters parm(BALL_TEST_DATA_PATH(AtomTypes_test.ini));
parm.init();

CHECK(AtomTypes::AtomTypes)
	at_ptr = new AtomTypes;
	TEST_NOT_EQUAL(at_ptr, 0)
RESULT

CHECK(AtomTypes::~AtomTypes)
	delete at_ptr;
RESULT

AtomTypes at1;
at1.extractSection(parm, "AtomTypes");

CHECK(AtomTypes::AtomTypes(const AtomTypes& atom_types))
	AtomTypes at2(at1);
	bool test = (at1 == at2);
	TEST_EQUAL(test, true)
RESULT

CHECK(AtomTypes::clear())
	AtomTypes at2(at1);
	at2.clear();
	AtomTypes at3;
	bool test = (at2 == at3);
	TEST_EQUAL(test, true)
RESULT

CHECK(AtomTypes::operator = (const AtomTypes& atom_types))
	AtomTypes at2 = at1;
	bool test = (at1 == at2);
	TEST_EQUAL(test, true)
RESULT

CHECK(AtomTypes::extractSection())
	TEST_EQUAL(at1.getNumberOfTypes(), 5)
	TEST_EQUAL(at1.getNumberOfKeys(), 4)
	TEST_EQUAL(at1.getType("BR"), 1)
	TEST_EQUAL(at1.getType("C"), 2)
	TEST_EQUAL(at1.getType("CA"), 3)
	TEST_EQUAL(at1.getType("CB"), 4)
RESULT

CHECK(AtomTypes::hasType())
	TEST_EQUAL(at1.hasType("CB"), true)
	TEST_EQUAL(at1.hasType("XY"), false)
RESULT

CHECK(AtomTypes::getType())
	TEST_EQUAL(at1.getType("BR"), 1)
	TEST_EQUAL(at1.getType("XY"), -1)
RESULT

CHECK(AtomTypes::getTypeName())
	TEST_EQUAL(at1.getTypeName(1), "BR")
	TEST_EQUAL(at1.getTypeName(8), "")
RESULT

CHECK(AtomTypes::getNumberOfTypes())
	TEST_EQUAL(at1.getNumberOfTypes(), 5)
	AtomTypes at2;
	TEST_EQUAL(at2.getNumberOfTypes(), 0)
RESULT

CHECK(AtomTypes::operator == ())
	AtomTypes at2;
	bool test = (at2 == at1);
	TEST_EQUAL(test, false)
	at2 = at1;
	test = (at2 == at1);
	TEST_EQUAL(test, true)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
