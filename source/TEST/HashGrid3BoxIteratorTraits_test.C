// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid3::BoxIteratorTraits)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HashGridBox3<int>::BoxIteratorTraits* it_tr_ptr = 0;
CHECK(BoxIteratorTraits() throw())
	it_tr_ptr = new HashGridBox3<int>::BoxIteratorTraits;
	TEST_NOT_EQUAL(it_tr_ptr, 0)
RESULT

CHECK([EXTRA]~BoxIteratorTraits() throw())
	delete it_tr_ptr;
RESULT

CHECK(BALL_CREATE_DEEP(BoxIteratorTraits))
  // ???
RESULT

CHECK(BoxIteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(BoxIteratorTraits(const BoxIteratorTraits& traits, bool /* deep */ = true) throw())
  // ???
RESULT

CHECK(BoxIteratorTraits(const HashGridBox3& box) throw())
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

CHECK(bool operator != (const BoxIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const BoxIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(const BoxIteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const BoxIteratorTraits& operator = (const BoxIteratorTraits& traits) throw())
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
