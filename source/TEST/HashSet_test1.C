// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/CONCEPT/visitor.h>

#include <list>
#include <vector>
#include <algorithm>

#include "ItemCollector.h"

///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(HashSet<T>)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

HashSet<int>* set_ptr;
CHECK(HashSet(Size initial_capacity = INITIAL_CAPACITY, Size number_of_buckets = INITIAL_NUMBER_OF_BUCKETS) throw())
	set_ptr = new HashSet<int>;
	TEST_NOT_EQUAL(set_ptr, 0)
RESULT

CHECK(~HashSet() throw())
	delete set_ptr;
RESULT

CHECK(Size size() const throw())
	HashSet<int> hs;
	TEST_EQUAL(hs.size(), 0)
RESULT

CHECK(Size getSize() const throw())
	HashSet<int> hs;
	TEST_EQUAL(hs.getSize(), 0)
RESULT

CHECK(HashSet(const HashSet& hash_set) throw())
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);

	HashSet<int> nhs(hs);
	TEST_EQUAL(nhs.getSize(), 3)
	TEST_EQUAL(nhs.has(0), true)
	TEST_EQUAL(nhs.has(1), true)
	TEST_EQUAL(nhs.has(2), true)
	TEST_EQUAL(nhs.has(3), false)
RESULT

CHECK(void clear() throw())
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.clear();
	TEST_EQUAL(hs.getSize(), 0)
	TEST_EQUAL(hs.getCapacity(), 4)
	TEST_EQUAL(hs.getBucketSize(), 3)
RESULT

CHECK(void destroy() throw())
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.destroy();
	TEST_EQUAL(hs.getSize(), 0)
	TEST_EQUAL(hs.getCapacity(), 4)
	TEST_EQUAL(hs.getBucketSize(), 3)
RESULT

CHECK(void set(const HashSet& hash_set) throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs2.set(hs);

	TEST_EQUAL(hs2.getSize(), 3)
	TEST_EQUAL(hs2.getCapacity(), 4)
	TEST_EQUAL(hs2.getBucketSize(), 3)
RESULT

CHECK(void get(HashSet& hash_set) const throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.get(hs2);

	TEST_EQUAL(hs2.getSize(), 3)
	TEST_EQUAL(hs2.getCapacity(), 4)
	TEST_EQUAL(hs2.getBucketSize(), 3)
RESULT

CHECK(void swap(HashSet& hash_set) throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);

	hs2.insert(3);
	hs2.insert(4);
	hs2.insert(5);
	hs2.insert(6);
	hs2.insert(7);

	hs.swap(hs2);

	TEST_EQUAL(hs2.getSize(), 3)
	TEST_EQUAL(hs2.getCapacity(), 4)
	TEST_EQUAL(hs2.getBucketSize(), 3)
	TEST_EQUAL(hs2.has(0), true)
	TEST_EQUAL(hs2.has(3), false)

	TEST_EQUAL(hs.getSize(), 5)
	TEST_EQUAL(hs.getCapacity(), 7)
	TEST_EQUAL(hs.getBucketSize(), 7)
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(3), true)
RESULT

CHECK(const HashSet& operator = (const HashSet& rhs) throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs2 = hs;

	TEST_EQUAL(hs2.getSize(), 3)
	TEST_EQUAL(hs2.getCapacity(), 4)
	TEST_EQUAL(hs2.getBucketSize(), 3)
RESULT

CHECK(const HashSet& operator &= (const HashSet& rhs) throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	hs2 &= hs;
	TEST_EQUAL(hs2.getSize(), 0)
	hs &= hs2;
	TEST_EQUAL(hs.getSize(), 0)
	hs &= hs;
	TEST_EQUAL(hs.getSize(), 0)

	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	hs2.insert(1);
	hs2.insert(3);
	hs &= hs2;
	TEST_EQUAL(hs.getSize(), 2)
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), false)
	TEST_EQUAL(hs.has(3), true)
RESULT

CHECK(const HashSet& operator |= (const HashSet& rhs) throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	hs2 |= hs;
	TEST_EQUAL(hs2.getSize(), 3)
	hs |= hs2;
	TEST_EQUAL(hs.getSize(), 3)
	hs |= hs;
	TEST_EQUAL(hs.getSize(), 3)

	hs.clear();
	hs.insert(0);
	hs.insert(3);
	hs2.clear();
	hs2.insert(1);
	hs2.insert(3);
	hs |= hs2;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs.has(0), true)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), false)
	TEST_EQUAL(hs.has(3), true)
RESULT

CHECK(HashSet operator & (const HashSet& rhs) const throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	HashSet<int> hs3 = hs2 & hs;
	TEST_EQUAL(hs2.getSize(), 0)
	hs3 = hs & hs2;
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs.getSize(), 3)

	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	hs2.insert(1);
	hs2.insert(3);
	hs3 = hs & hs2;
	TEST_EQUAL(hs3.getSize(), 2)
	TEST_EQUAL(hs3.has(0), false)
	TEST_EQUAL(hs3.has(1), true)
	TEST_EQUAL(hs3.has(2), false)
	TEST_EQUAL(hs3.has(3), true)
	TEST_EQUAL(hs.getSize(), 4)
	TEST_EQUAL(hs2.getSize(), 2)
	hs3 = hs2 & hs;
	TEST_EQUAL(hs3.getSize(), 2)
	TEST_EQUAL(hs3.has(0), false)
	TEST_EQUAL(hs3.has(1), true)
	TEST_EQUAL(hs3.has(2), false)
	TEST_EQUAL(hs3.has(3), true)
	TEST_EQUAL(hs.getSize(), 4)
	TEST_EQUAL(hs2.getSize(), 2)
RESULT

CHECK(HashSet operator | (const HashSet& rhs) const throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	HashSet<int> hs3 = hs2 | hs;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs3.getSize(), 3)

	hs3 = hs | hs2;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs3.getSize(), 3)

	hs3 = hs | hs;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs3.getSize(), 3)

	hs3 = hs2 | hs2;
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs3.getSize(), 0)

	hs.clear();
	hs.insert(0);
	hs.insert(3);
	hs2.clear();
	hs2.insert(1);
	hs2.insert(3);
	hs3 = hs | hs2;
	TEST_EQUAL(hs3.getSize(), 3)
	TEST_EQUAL(hs3.has(0), true)
	TEST_EQUAL(hs3.has(1), true)
	TEST_EQUAL(hs3.has(2), false)
	TEST_EQUAL(hs3.has(3), true)

	hs3 = hs2 | hs;
	TEST_EQUAL(hs3.getSize(), 3)
	TEST_EQUAL(hs3.has(0), true)
	TEST_EQUAL(hs3.has(1), true)
	TEST_EQUAL(hs3.has(2), false)
	TEST_EQUAL(hs3.has(3), true)
RESULT

CHECK(const HashSet& operator += (const HashSet& rhs) throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	hs2 += hs;
	TEST_EQUAL(hs2.getSize(), 3)
	hs += hs2;
	TEST_EQUAL(hs.getSize(), 3)
	hs += hs;
	TEST_EQUAL(hs.getSize(), 3)

	hs.clear();
	hs.insert(0);
	hs.insert(3);
	hs2.clear();
	hs2.insert(1);
	hs2.insert(3);
	hs += hs2;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs.has(0), true)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), false)
	TEST_EQUAL(hs.has(3), true)
RESULT

CHECK(HashSet operator + (const HashSet& rhs) const throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	HashSet<int> hs3 = hs2 + hs;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs3.getSize(), 3)

	hs3 = hs + hs2;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs3.getSize(), 3)

	hs3 = hs + hs;
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs3.getSize(), 3)

	hs3 = hs2 + hs2;
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs3.getSize(), 0)

	hs.clear();
	hs.insert(0);
	hs.insert(3);
	hs2.clear();
	hs2.insert(1);
	hs2.insert(3);
	hs3 = hs + hs2;
	TEST_EQUAL(hs3.getSize(), 3)
	TEST_EQUAL(hs3.has(0), true)
	TEST_EQUAL(hs3.has(1), true)
	TEST_EQUAL(hs3.has(2), false)
	TEST_EQUAL(hs3.has(3), true)

	hs3 = hs2 + hs;
	TEST_EQUAL(hs3.getSize(), 3)
	TEST_EQUAL(hs3.has(0), true)
	TEST_EQUAL(hs3.has(1), true)
	TEST_EQUAL(hs3.has(2), false)
	TEST_EQUAL(hs3.has(3), true)
RESULT

CHECK(const HashSet& operator -= (const HashSet& rhs) throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	hs2 -= hs;
	TEST_EQUAL(hs2.getSize(), 0)
	hs -= hs2;
	TEST_EQUAL(hs.getSize(), 3)
	hs -= hs;
	TEST_EQUAL(hs.getSize(), 0)

	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	hs2.insert(1);
	hs2.insert(3);
	hs -= hs2;
	TEST_EQUAL(hs.getSize(), 2)
	TEST_EQUAL(hs.has(0), true)
	TEST_EQUAL(hs.has(1), false)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), false)
RESULT

CHECK(HashSet operator - (const HashSet& rhs) const throw())
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
	TEST_EQUAL(hs2.getSize(), 0)

	HashSet<int> hs3 = hs2 & hs;
	TEST_EQUAL(hs2.getSize(), 0)
	hs3 = hs - hs2;
	TEST_EQUAL(hs2.getSize(), 0)
	TEST_EQUAL(hs.getSize(), 3)

	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	hs2.insert(1);
	hs2.insert(3);
	hs3 = hs - hs2;
	TEST_EQUAL(hs3.getSize(), 2)
	TEST_EQUAL(hs3.has(0), true)
	TEST_EQUAL(hs3.has(1), false)
	TEST_EQUAL(hs3.has(2), true)
	TEST_EQUAL(hs3.has(3), false)
	TEST_EQUAL(hs.getSize(), 4)
	TEST_EQUAL(hs2.getSize(), 2)
	hs3 = hs2 - hs;
	TEST_EQUAL(hs3.getSize(), 0)
	TEST_EQUAL(hs3.has(0), false)
	TEST_EQUAL(hs3.has(1), false)
	TEST_EQUAL(hs3.has(2), false)
	TEST_EQUAL(hs3.has(3), false)
	TEST_EQUAL(hs.getSize(), 4)
	TEST_EQUAL(hs2.getSize(), 2)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
