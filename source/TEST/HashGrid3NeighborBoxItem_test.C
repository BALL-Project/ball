// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HashGrid3NeighborBoxItem_test.C,v 1.1 2003/06/22 10:21:48 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid3::NeighborBoxItem, "$Id: HashGrid3NeighborBoxItem_test.C,v 1.1 2003/06/22 10:21:48 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
//BALL::HashGridBox3::NeighbourBoxItem

CHECK(NeighbourBoxItem(HashGridBox3* box, NeighbourBoxItem* next) throw())
  // ???
RESULT

////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
