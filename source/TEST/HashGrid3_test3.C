// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HashGrid3_test3.C,v 1.1 2003/06/20 12:38:09 amoll Exp $
//
// This line is for testing purposes. Remove me.
//
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid, "$Id: HashGrid3_test3.C,v 1.1 2003/06/20 12:38:09 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class HashGridBox3::BoxIteratorTraits_::

HashGridBox3<int>::BoxIteratorTraits_* it_tr_ptr = 0;
CHECK(BoxIteratorTraits_() throw())
	it_tr_ptr = new HashGridBox3<int>::BoxIteratorTraits_;
	TEST_NOT_EQUAL(it_tr_ptr, 0)
RESULT

CHECK([EXTRA]~BoxIteratorTraits_() throw())
	delete it_tr_ptr;
RESULT

CHECK(BALL_CREATE_DEEP(BoxIteratorTraits_))
  // ???
RESULT

CHECK(BoxIteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true) throw())
  // ???
RESULT

CHECK(BoxIteratorTraits_(const HashGridBox3& box) throw())
  // ???
RESULT

CHECK(HashGridBox3* getContainer() throw())
  // ???
RESULT

CHECK(HashGridBox3<Item>& getData() throw())
  // ???
RESULT

CHECK(bool isBegin() const throw())
  // ???
RESULT

CHECK(bool isEnd() const throw())
  // ???
RESULT

CHECK(bool isSingular() const throw())
  // ???
RESULT

CHECK(bool isValid() const throw())
  // ???
RESULT

CHECK(bool operator != (const BoxIteratorTraits_& traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const BoxIteratorTraits_& traits) const throw())
  // ???
RESULT

CHECK(const BoxIteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits) throw())
  // ???
RESULT

CHECK(const HashGridBox3* getContainer() const throw())
  // ???
RESULT

CHECK(const HashGridBox3<Item>& getData() const throw())
  // ???
RESULT

CHECK(void forward() throw())
  // ???
RESULT

CHECK(void invalidate() throw())
  // ???
RESULT

CHECK(void toBegin() throw())
  // ???
RESULT

CHECK(void toEnd() throw())
  // ???
RESULT

////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
