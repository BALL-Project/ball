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

HashMap<int, int>* map_ptr;
CHECK(HashMap(Size initial_capacity = INITIAL_CAPACITY, Size number_of_buckets = INITIAL_NUMBER_OF_BUCKETS) throw())
	map_ptr = new HashMap<int, int>;
	TEST_NOT_EQUAL(map_ptr, 0)
RESULT

CHECK(~HashMap() throw())
	delete map_ptr;
RESULT

CHECK(Size size() const throw())
	HashMap<int, int> hm;
	TEST_EQUAL(hm.size(), 0)
RESULT

//CHECK(std::pair<Iterator, bool> insert(const ValueType& entry) throw())
CHECK([EXTRA] otherwise_there_is_a_problem_here)
	HashMap<int, int> hm;
	TEST_EQUAL(hm.size(), 0)
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	TEST_EQUAL(hm.size(), 1)
	hm.insert(HashMap<int, int>::ValueType(0, 1));
	TEST_EQUAL(hm.size(), 1)
	hm.insert(HashMap<int, int>::ValueType(1, 0));
	TEST_EQUAL(hm.size(), 2)
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL(hm.size(), 2)
RESULT

CHECK(Iterator find(const Key& key) throw())
	HashMap<int, int> hm;
	TEST_EQUAL((hm.find(0) == hm.end()), true)
	TEST_EQUAL((hm.find(1) == hm.end()), true)
	TEST_EQUAL((hm.find(2) == hm.end()), true)
	TEST_EQUAL((hm.find(-2) == hm.end()), true)
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	TEST_EQUAL((hm.find(0) == hm.end()), false)
	TEST_EQUAL((hm.find(1) == hm.end()), true)
	TEST_EQUAL((hm.find(2) == hm.end()), true)
	TEST_EQUAL((hm.find(-2) == hm.end()), true)
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL((hm.find(0) == hm.end()), false)
	TEST_EQUAL((hm.find(1) == hm.end()), false)
	TEST_EQUAL((hm.find(2) == hm.end()), true)
	TEST_EQUAL((hm.find(-2) == hm.end()), true)
RESULT

CHECK(ConstIterator find(const Key& key) const throw())
	HashMap<int, int> h_mutable;
	const HashMap<int, int>& hm = const_cast<const HashMap<int, int>&>(h_mutable);
	TEST_EQUAL((hm.find(0) == hm.end()), true)
	TEST_EQUAL((hm.find(1) == hm.end()), true)
	TEST_EQUAL((hm.find(2) == hm.end()), true)
	TEST_EQUAL((hm.find(-2) == hm.end()), true)
	h_mutable.insert(HashMap<int, int>::ValueType(0, 0));
	TEST_EQUAL((hm.find(0) == hm.end()), false)
	TEST_EQUAL((hm.find(1) == hm.end()), true)
	TEST_EQUAL((hm.find(2) == hm.end()), true)
	TEST_EQUAL((hm.find(-2) == hm.end()), true)
	h_mutable.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL((hm.find(0) == hm.end()), false)
	TEST_EQUAL((hm.find(1) == hm.end()), false)
	TEST_EQUAL((hm.find(2) == hm.end()), true)
	TEST_EQUAL((hm.find(-2) == hm.end()), true)
RESULT

CHECK(Size erase(const Key& key) throw())
	HashMap<int, int> hm;
	TEST_EQUAL(hm.size(), 0)
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	TEST_EQUAL(hm.size(), 1)
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL(hm.size(), 2)
	hm.insert(HashMap<int, int>::ValueType(2, 2));
	TEST_EQUAL(hm.find(1) == hm.end(), false)
	hm.erase(1);
	TEST_EQUAL(hm.find(1) == hm.end(), true)
	TEST_EQUAL(hm.find(0) == hm.end(), false)
	TEST_EQUAL(hm.find(2) == hm.end(), false)
	TEST_EQUAL(hm.find(5) == hm.end(), true)
	TEST_EQUAL(hm.size(), 2)
	hm.erase(5);
	TEST_EQUAL(hm.find(1) == hm.end(), true)
	TEST_EQUAL(hm.find(0) == hm.end(), false)
	TEST_EQUAL(hm.find(2) == hm.end(), false)
	TEST_EQUAL(hm.find(5) == hm.end(), true)
	TEST_EQUAL(hm.size(), 2)
RESULT

CHECK(void erase(Iterator first, Iterator last) throw(Exception::IncompatibleIterators))
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	hm.insert(HashMap<int, int>::ValueType(2, 2));
	hm.insert(HashMap<int, int>::ValueType(3, 3));

	HashMap<int, int>::Iterator it1 = hm.begin();
	HashMap<int, int>::Iterator it2 = hm.begin();
	++it2;
	++it2;
	++it1;

	hm.erase(it1, it2);
	TEST_EQUAL(hm.has(0), true)
	TEST_EQUAL(hm.has(2), true)
	TEST_EQUAL(hm.has(3), true)
	TEST_EQUAL(hm.has(1), false)
	TEST_EQUAL(hm.size(), 3)

	hm.erase(hm.begin(), hm.end());
	TEST_EQUAL(hm.size(), 0)

	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	hm.insert(HashMap<int, int>::ValueType(2, 2));
	hm.insert(HashMap<int, int>::ValueType(3, 3));
	TEST_EQUAL(hm.size(), 4)
	it1 = hm.begin();
	++it1;
	++it1;
	hm.erase(it1, hm.end());
	TEST_EQUAL(hm.size(), 2)
	TEST_EQUAL(hm.has(0), true)
	TEST_EQUAL(hm.has(1), true)
	TEST_EQUAL(hm.has(2), false)
	TEST_EQUAL(hm.has(3), false)
RESULT

CHECK(void erase(Iterator pos) throw(Exception::IncompatibleIterators, Exception::InvalidIterator))
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	hm.insert(HashMap<int, int>::ValueType(2, 2));
	hm.insert(HashMap<int, int>::ValueType(3, 3));
	HashMap<int, int>::Iterator it1 = hm.begin();
	++it1;
	TEST_EQUAL(it1->first, 1)
	hm.erase(it1);
	TEST_EQUAL(hm.has(0), true)
	TEST_EQUAL(hm.has(1), false)
	TEST_EQUAL(hm.has(2), true)
	TEST_EQUAL(hm.has(3), true)
	TEST_EQUAL(hm.size(), 3)
	hm.erase(hm.begin());
	TEST_EQUAL(hm.size(), 2)
	hm.erase(hm.begin());
	TEST_EQUAL(hm.size(), 1)
	hm.erase(hm.begin());
	TEST_EQUAL(hm.size(), 0)
RESULT

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

CHECK(HashMap(const HashMap& hash_map) throw())
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	hm.insert(HashMap<int, int>::ValueType(2, 2));
	hm.insert(HashMap<int, int>::ValueType(3, 3));

	HashMap<int, int> hm2(hm);
	TEST_EQUAL(hm2[0], 0)
	TEST_EQUAL(hm2[1], 1)
	TEST_EQUAL(hm2[2], 2)
	TEST_EQUAL(hm2[3], 3)
RESULT

CHECK(void clear() throw())
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.clear();
	TEST_EQUAL(hm.size(), 0)
RESULT

CHECK(void swap(HashMap& hash_map) throw())
	HashMap<int, int> hm, hm2;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm2.insert(HashMap<int, int>::ValueType(10, 10));
	hm2.insert(HashMap<int, int>::ValueType(11, 11));

	TEST_EQUAL(hm.size(), 2)
	TEST_EQUAL(hm2.size(), 2)
	TEST_EQUAL(hm2[10], 10)
	TEST_EQUAL(hm2[11], 11)
	TEST_EQUAL(hm[0], 0)
	TEST_EQUAL(hm[1], 1)
	hm.swap(hm2);
	TEST_EQUAL(hm.size(), 2)
	TEST_EQUAL(hm2.size(), 2)
	TEST_EQUAL(hm[10], 10)
	TEST_EQUAL(hm[11], 11)
	TEST_EQUAL(hm2[0], 0)
	TEST_EQUAL(hm2[1], 1)
RESULT

CHECK(const HashMap& operator = (const HashMap& hash_map) throw())
	HashMap<int, int> hm, hm2;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm2 = hm;
	TEST_EQUAL(hm2[0], 0)
	TEST_EQUAL(hm2[1], 1)
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


// tr1 does not seem to implement != currently...
#ifndef BALL_HAS_UNORDERED_MAP
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
#endif

CHECK(ConstIterator begin() const throw())
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	HashMap<int, int>::ConstIterator it = hm.begin();
	TEST_EQUAL(it->first == 0, true)
	it++;
	TEST_EQUAL(it == hm.end(), true)
RESULT

CHECK(ConstIterator end() const throw())
	HashMap<int, int> hm;
	TEST_EQUAL(hm.begin() ==  hm.end(), true)
RESULT

CHECK(HashMap* getContainer() throw())
  // ???
RESULT

CHECK(Iterator end() throw())
	HashMap<int, int> hm;
	HashMap<int, int>::Iterator it1 = hm.begin();
	HashMap<int, int>::Iterator it2 = hm.end();
	TEST_EQUAL(it1 == it2, true)
RESULT

CHECK(Iterator insert(Iterator pos, const ValueType& entry) throw())
  // ???
RESULT

HashMap<int, int> hm;
hm.insert(HashMap<int, int>::ValueType(0, 0));
hm.insert(HashMap<int, int>::ValueType(1, 1));
HashMap<int, int>::Iterator it = hm.begin();


// IteratorTraits_ tests
CHECK(IteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(IteratorTraits_() throw())
  // ???
RESULT

CHECK(IteratorTraits_(const HashMap& hash_map) throw())
  // ???
RESULT

CHECK(IteratorTraits_(const IteratorTraits_& traits) throw())
  // ???
RESULT

CHECK(ValueType& getData() throw())
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

CHECK(bool operator != (const IteratorTraits_& traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const IteratorTraits_& traits) const throw())
  // ???
RESULT

CHECK(const HashMap* getContainer() const throw())
  // ???
RESULT

CHECK(const IteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const IteratorTraits_& operator = (const IteratorTraits_& traits) throw())
  // ???
RESULT

CHECK(const ValueType& getData() const throw())
  // ???
RESULT

CHECK(friend Iterator begin() throw())
  // ???
RESULT

CHECK((std::pair<Iterator, bool> insert(const ValueType& entry) throw()))
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
