// $Id: PersistentObject_test.C,v 1.2 2001/07/15 17:32:40 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/CONCEPT/persistentObject.h>
#include <BALL/CONCEPT/persistenceManager.h>

///////////////////////////

START_TEST(PersistentObject, "$Id: PersistentObject_test.C,v 1.2 2001/07/15 17:32:40 amoll Exp $")

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
