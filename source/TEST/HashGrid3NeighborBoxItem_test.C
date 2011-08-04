// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <set>

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>

///////////////////////////

START_TEST(HashGrid3::NeighborBoxItem)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
//BALL::HashGridBox3::NeighbourBoxItem

CHECK(HashGrid3 boxNeighbours)
	HashGrid3<int> grid(Vector3(0.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 2.0f), 1.0f);
	TEST_EQUAL(grid.getSize(), 27);

	std::set<HashGridBox3<int>*> boxes;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			for(int k = 0; k < 3; ++k)
			{
				boxes.insert(grid.getBox(i, j, k));
			}
		}
	}

	TEST_EQUAL(boxes.size(), (size_t)27);

	for(HashGridBox3<int>::BoxIterator it = grid.getBox(1,1,1)->beginBox(); +it; ++it)
	{
		TEST_EQUAL(boxes.find(&*it) != boxes.end(), true);
		boxes.erase(&*it);
	}

	TEST_EQUAL(boxes.size(), 0);
RESULT

////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
