// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HashGrid3_test5.C,v 1.1 2003/06/20 12:38:09 amoll Exp $
//
// This line is for testing purposes. Remove me.
//
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid, "$Id: HashGrid3_test5.C,v 1.1 2003/06/20 12:38:09 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
// BALL::HashGridBox3::DataItem_

CHECK(DataItem_(const Item& item, DataItem_* next) throw())
  // ???
RESULT

////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
