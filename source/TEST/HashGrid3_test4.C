// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HashGrid3_test4.C,v 1.1 2003/06/20 12:38:09 amoll Exp $
//
// This line is for testing purposes. Remove me.
//
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid, "$Id: HashGrid3_test4.C,v 1.1 2003/06/20 12:38:09 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BALL_CREATE_DEEP(DataIteratorTraits_))
  // ???
RESULT

CHECK(DataIteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(DataIteratorTraits_() throw())
  // ???
RESULT

CHECK(DataIteratorTraits_(const DataIteratorTraits_& traits, bool /* deep */ = true) throw())
  // ???
RESULT

CHECK(DataIteratorTraits_(const HashGridBox3& box) throw())
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

CHECK(bool operator != (const DataIteratorTraits_ &traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const DataIteratorTraits_ &traits) const throw())
  // ???
RESULT

CHECK(const DataIteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const DataIteratorTraits_& operator = (const DataIteratorTraits_ &traits) throw())
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
