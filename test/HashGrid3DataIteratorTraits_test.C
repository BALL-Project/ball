// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid3::DataIteratorTraits)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BALL_CREATE_DEEP(DataIteratorTraits))
  // ???
RESULT

CHECK(DataIteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(DataIteratorTraits() throw())
  // ???
RESULT

CHECK(DataIteratorTraits(const DataIteratorTraits& traits, bool /* deep */ = true) throw())
  // ???
RESULT

CHECK(DataIteratorTraits(const HashGridBox3& box) throw())
  // ???
RESULT

CHECK(HashGridBox3* getContainer() throw())
  // ???
RESULT

CHECK(Item& getData() throw())
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

CHECK(bool operator != (const DataIteratorTraits &traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const DataIteratorTraits &traits) const throw())
  // ???
RESULT

CHECK(const DataIteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const DataIteratorTraits& operator = (const DataIteratorTraits &traits) throw())
  // ???
RESULT

CHECK(const HashGridBox3* getContainer() const throw())
  // ???
RESULT

CHECK(const Item& getData() const throw())
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
