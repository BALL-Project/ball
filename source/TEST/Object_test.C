// $Id: Object_test.C,v 1.1 2001/05/28 11:51:41 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/CONCEPT/object.h>

///////////////////////////

START_TEST(class_name, "$Id: Object_test.C,v 1.1 2001/05/28 11:51:41 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class Object::

Object* ptr;

CHECK(Object::Object() throw())
	ptr = new Object;
	TEST_NOT_EQUAL(ptr, 0)
	TEST_EQUAL(ptr->getHandle(), 1)
RESULT


CHECK(Object::Object(const Object& object, bool deep = true) throw())
	Object object1;
	Object object2(object1);
	bool test = (object1 == object1);
	TEST_EQUAL(test, true)
	test = (object1 == object2);
	TEST_NOT_EQUAL(test, true)
RESULT


CHECK(Object::~Object() throw())
	delete ptr;
RESULT


CHECK(Object::clear() throw())
  // BAUSTELLE
	// clear() doesn't do anything.
RESULT


CHECK(Object::Object& operator = (const Object& /* object */) throw())
  // BAUSTELLE
	// operator () doesn't do anything.
RESULT


CHECK(Object::getHandle() const  throw())
	Object object3;
	TEST_EQUAL(object3.getHandle(), 4)
RESULT


CHECK(Object::getNextHandle() throw())
	Object object4;
	TEST_EQUAL(object4.getNextHandle(), 6)
RESULT


CHECK(Object::getNewHandle() throw())
	Object object5;
	TEST_EQUAL(object5.getNewHandle(), 7)
RESULT


CHECK(Object::bool operator == (const Object& object) const  throw())
	Object object6;
	Object object7;
	bool test = (object6 == object6);
	TEST_EQUAL(test, true)
	test = (object6 == object7);
	TEST_NOT_EQUAL(test, true)
RESULT


CHECK(Object::bool operator != (const Object& object) const  throw())
	Object object8;
	Object object9;
	bool test = (object8 != object8);
	TEST_NOT_EQUAL(test, true)
	test = (object8 != object9);
	TEST_EQUAL(test, true)
RESULT


CHECK(Object::bool operator < (const Object& object) const  throw())
	Object object10;
	Object object11;
	bool test = (object10 < object10);
	TEST_NOT_EQUAL(test, true)
	test = (object10 < object11);
	TEST_EQUAL(test, true)
	test = (object11 < object10);
	TEST_NOT_EQUAL(test, true)
RESULT


CHECK(Object::bool operator <= (const Object& object) const  throw())
	Object object12;
	Object object13;
	bool test = (object12 <= object12);
	TEST_EQUAL(test, true)
	test = (object12 <= object13);
	TEST_EQUAL(test, true)
	test = (object13 <= object12);
	TEST_NOT_EQUAL(test, true)
RESULT


CHECK(Object::bool operator >= (const Object& object) const  throw())
	Object object14;
	Object object15;
	bool test = (object14 >= object14);
	TEST_EQUAL(test, true)
	test = (object14 >= object15);
	TEST_NOT_EQUAL(test, true)
	test = (object15 >= object14);
	TEST_EQUAL(test, true)
RESULT


CHECK(Object::bool operator > (const Object& object) const  throw())
	Object object16;
	Object object17;
	bool test = (object16 > object16);
	TEST_NOT_EQUAL(test, true)
	test = (object16 > object17);
	TEST_NOT_EQUAL(test, true)
	test = (object17 > object16);
	TEST_EQUAL(test, true)
RESULT


CHECK(Object::compare(const Object& object) const  throw())
	Object object18;
	Object object19;
	Object object20;
	int test = (object19.compare(object19));
	TEST_EQUAL(test, 0)
	test = (object19.compare(object18));
	TEST_EQUAL(test, 1)
	test = (object19.compare(object20));
	TEST_EQUAL(test, -1)
RESULT


CHECK(Object::read(::std::istream& s) throw())
  // BAUSTELLE
	// not implemented yet
RESULT


CHECK(Object::write(::std::ostream& s) const  throw())
  // BAUSTELLE
	// not implemented yet
RESULT


CHECK(Object::friend::std::istream& operator >> (::std::istream& s, Object& object) throw())
  // BAUSTELLE
	// read() not implemented yet
RESULT


CHECK(Object::friend::std::ostream& operator << (::std::ostream& s, const Object& object) throw())
  // BAUSTELLE
	// write() not implemented yet
RESULT


CHECK(Object::isValid() const  throw())
	Object object21;
	TEST_EQUAL(object21.isValid(), true)
RESULT


CHECK(Object::dump(::std::ostream& s = std::cout, Size depth = 0) const  throw())
  // BAUSTELLE
RESULT


#ifdef BALL_SUPPORT_OBJECT_MANAGER

CHECK(Object::getPrevious() throw())
	Object object22;
	Object object23;
	ptr = object23.getPrevious();
	TEST_NOT_EQUAL(ptr, 0)
	bool test = (*ptr == object22);
	TEST_EQUAL(test, true)
RESULT


const Object* const_ptr;

CHECK(Object::getPrevious() const  throw())
	Object object24;
	Object object25;
	const_ptr = object25.getPrevious();
	TEST_NOT_EQUAL(const_ptr, 0)
	bool test = (*const_ptr == object24);
	TEST_EQUAL(test, true)
RESULT


CHECK(Object::getNext() throw())
	Object object26;
	Object object27;
	ptr = object26.getNext();
	TEST_NOT_EQUAL(ptr, 0)
	bool test = (*ptr == object27);
	TEST_EQUAL(test, true)
RESULT


CHECK(Object::getNext() const  throw())
	Object object28;
	Object object29;
	const_ptr = object28.getNext();
	TEST_NOT_EQUAL(const_ptr, 0)
	bool test = (*const_ptr == object29);
	TEST_EQUAL(test, true)
RESULT

#endif

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
