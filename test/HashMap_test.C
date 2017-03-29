// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/CONCEPT/visitor.h>
#include "ItemCollector.h"
///////////////////////////

using namespace BALL;
using namespace std;

class MyVisitor 
	: public Visitor <HashMap<int, int> >
{
	public:
	MyVisitor()
		: value_sum(0),
			key_sum(0)
	{
	}

	int value_sum;
	int key_sum;

	void visit(HashMap<int, int>& v)
	{
		HashMap<int, int>::Iterator it = v.begin();
		for (; it != v.end(); ++it)
		{
			key_sum += (*it).first;
			value_sum += (*it).second;
		}
	}
};

START_TEST(HashMap)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK(T& operator [] (const Key& key) throw())
	HashMap<int, int> hm;
	hm[0] = 0;
	hm[0] = 1;
	hm[1] = 2;
	hm[2] = 4;
	hm[3] = 8;
	hm[4] = 16;
	hm[5] = 32;
	TEST_EQUAL(hm.size(), 6)
	TEST_EQUAL(hm[0], 1)
	TEST_EQUAL(hm[1], 2)
	TEST_EQUAL(hm[2], 4)
	TEST_EQUAL(hm[3], 8)
	TEST_EQUAL(hm[4], 16)
	TEST_EQUAL(hm[5], 32)
RESULT

CHECK(const T& operator [] (const Key& key) const throw(typename HashMap<Key, T>::IllegalKey))
	HashMap<int, int> hm;
	hm[0] = 0;
	hm[0] = 1;
	hm[1] = 2;
	hm[2] = 4;
	hm[3] = 8;
	hm[4] = 16;
	hm[5] = 32;
	const HashMap<int, int>& const_map = const_cast<const HashMap<int, int>&>(hm);
	TEST_EQUAL(const_map.size(), 6)
	TEST_EQUAL(const_map[0], 1)
	TEST_EQUAL(const_map[1], 2)
	TEST_EQUAL(const_map[2], 4)
	TEST_EQUAL(const_map[3], 8)
	TEST_EQUAL(const_map[4], 16)
	TEST_EQUAL(const_map[5], 32)
RESULT

CHECK(bool has(const Key& key) const throw())
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL(hm.has(0), true)
	TEST_EQUAL(hm.has(1), true)
	TEST_EQUAL(hm.has(2), false)
RESULT

CHECK(bool operator == (const HashMap& hash_map) const throw())
	HashMap<int, int> hm;
	HashMap<int, int> hm2;
	TEST_EQUAL(hm == hm2, true)

	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm2.insert(HashMap<int, int>::ValueType(0, 0));
	hm2.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL(hm == hm2, true)

	hm2.insert(HashMap<int, int>::ValueType(1, 2));
	hm2.insert(HashMap<int, int>::ValueType(1, 1));
	hm2.insert(HashMap<int, int>::ValueType(2, 1));
	TEST_EQUAL(hm == hm2, false)
RESULT


CHECK(bool operator != (const HashMap& hash_map) const throw())
	HashMap<int, int> hm;
	HashMap<int, int> hm2;

	TEST_EQUAL(hm != hm2, false)
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm2.insert(HashMap<int, int>::ValueType(0, 0));
	hm2.insert(HashMap<int, int>::ValueType(1, 1));

	TEST_EQUAL(hm != hm2, false)
	hm2.insert(HashMap<int, int>::ValueType(2, 1));
	TEST_EQUAL(hm != hm2, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
