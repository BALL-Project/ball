// $Id: PersistentObject_test.C,v 1.1 2001/05/28 15:30:45 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/CONCEPT/persistentObject.h>
#include <BALL/CONCEPT/persistenceManager.h>

///////////////////////////

START_TEST(class_name, "$Id: PersistentObject_test.C,v 1.1 2001/05/28 15:30:45 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PersistentObject* ptr;

CHECK(PersistentObject::PersistentObject() throw())
	ptr = new PersistentObject;
	TEST_NOT_EQUAL(ptr, 0)
RESULT

CHECK(PersistentObject::~PersistentObject() throw())
	delete ptr;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
