// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HashGrid3DataItem_test.C,v 1.1.32.1 2007/03/25 21:47:11 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid3:DataItem, "$Id: HashGrid3DataItem_test.C,v 1.1.32.1 2007/03/25 21:47:11 oliver Exp $")

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
