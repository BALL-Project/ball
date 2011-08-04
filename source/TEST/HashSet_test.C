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

class MyVisitor 
	: public Visitor<HashSet<int> >
{
	public:
	MyVisitor()
		: value_sum(0)
	{
	}

	int value_sum;

	void visit(HashSet<int>& set)
	{
		HashSet<int>::Iterator it = set.begin();
		for (; it != set.end(); ++it)
		{
			value_sum += (*it);
		}
	}

	void visit(int& v)
	{
		value_sum += v;
	}
};

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

CHECK(Size getBucketSize() const throw())
	HashSet<int> hs;
	TEST_EQUAL(hs.getBucketSize(), 3)
RESULT

CHECK(Size getCapacity() const throw())
	HashSet<int> hs;
	TEST_EQUAL(hs.getCapacity(), 4)
RESULT

CHECK((std::pair<Iterator, bool> insert(const ValueType& item) throw()))
	HashSet<int> hs;
	TEST_EQUAL(hs.getSize(), 0)
	hs.insert(0);
	TEST_EQUAL(hs.getSize(), 1)
	hs.insert(1);
	TEST_EQUAL(hs.getSize(), 2)
	hs.insert(1);
	TEST_EQUAL(hs.getSize(), 2)
	hs.insert(2);
	TEST_EQUAL(hs.getSize(), 3)
RESULT

CHECK(Iterator find(const Key& key) throw())
	HashSet<int> hs;
	TEST_EQUAL((hs.find(0) == hs.end()), true)
	TEST_EQUAL((hs.find(1) == hs.end()), true)
	TEST_EQUAL((hs.find(2) == hs.end()), true)
	TEST_EQUAL((hs.find(-2) == hs.end()), true)
	hs.insert(0);
	TEST_EQUAL((hs.find(0) == hs.end()), false)
	TEST_EQUAL((hs.find(1) == hs.end()), true)
	TEST_EQUAL((hs.find(2) == hs.end()), true)
	TEST_EQUAL((hs.find(-2) == hs.end()), true)
	hs.insert(1);
	TEST_EQUAL((hs.find(0) == hs.end()), false)
	TEST_EQUAL((hs.find(1) == hs.end()), false)
	TEST_EQUAL((hs.find(2) == hs.end()), true)
	TEST_EQUAL((hs.find(-2) == hs.end()), true)
RESULT

CHECK(ConstIterator find(const Key& key) const throw())
	HashSet<int> h_mutable;
	const HashSet<int>& hs = const_cast<const HashSet<int>&>(h_mutable);
	TEST_EQUAL((hs.find(0) == hs.end()), true)
	TEST_EQUAL((hs.find(1) == hs.end()), true)
	TEST_EQUAL((hs.find(2) == hs.end()), true)
	TEST_EQUAL((hs.find(-2) == hs.end()), true)
	h_mutable.insert(0);
	TEST_EQUAL((hs.find(0) == hs.end()), false)
	TEST_EQUAL((hs.find(1) == hs.end()), true)
	TEST_EQUAL((hs.find(2) == hs.end()), true)
	TEST_EQUAL((hs.find(-2) == hs.end()), true)
	h_mutable.insert(1);
	TEST_EQUAL((hs.find(0) == hs.end()), false)
	TEST_EQUAL((hs.find(1) == hs.end()), false)
	TEST_EQUAL((hs.find(2) == hs.end()), true)
	TEST_EQUAL((hs.find(-2) == hs.end()), true)
RESULT

CHECK(Size erase(const KeyType& key) throw())
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	Size res = hs.erase(0);
	TEST_EQUAL(res, 1)
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), true)
	TEST_EQUAL(hs.getSize(), 3)
	res = hs.erase(0);
	TEST_EQUAL(res, 0)
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), true)
	TEST_EQUAL(hs.getSize(), 3)
	res = hs.erase(4);
	TEST_EQUAL(res, 0)
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), true)
	TEST_EQUAL(hs.getSize(), 3)
	res = hs.erase(3);
	TEST_EQUAL(res, 1)
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), false)
	TEST_EQUAL(hs.getSize(), 2)
RESULT

CHECK(void erase(Iterator f, Iterator l) throw(Exception::IncompatibleIterators))
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);

	HashSet<int>::Iterator it1 = hs.begin();
	HashSet<int>::Iterator it2 = hs.begin();
	++it2;
	++it2;
	++it1;

	hs.erase(it1, it2);
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), true)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.getSize(), 3)

	hs.erase(hs.begin(), hs.end());
	TEST_EQUAL(hs.getSize(), 0)

	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	TEST_EQUAL(hs.getSize(), 4)
	it1 = hs.begin();
	++it1;
	++it1;
	hs.erase(it1, hs.end());
	TEST_EQUAL(hs.getSize(), 2)
	TEST_EQUAL(hs.has(0), true)
	TEST_EQUAL(hs.has(1), false)
	TEST_EQUAL(hs.has(2), false)
	TEST_EQUAL(hs.has(3), true)

	HashSet<int> hs2;
	TEST_EXCEPTION(Exception::IncompatibleIterators, hs.erase(hs.begin(), hs2.end()))
	TEST_EXCEPTION(Exception::IncompatibleIterators, hs.erase(hs2.begin(), hs.end()))
RESULT

CHECK(void erase(Iterator pos) throw(Exception::IncompatibleIterators, Exception::InvalidIterator))
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	HashSet<int>::Iterator it1 = hs.begin();
	++it1;
	TEST_EQUAL(*it1, 0)
	hs.erase(it1);
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), true)
	TEST_EQUAL(hs.getSize(), 3)
	hs.erase(hs.end());
	TEST_EQUAL(hs.getSize(), 3)
	hs.erase(hs.begin());
	TEST_EQUAL(hs.getSize(), 2)
	hs.erase(hs.begin());
	TEST_EQUAL(hs.getSize(), 1)
	hs.erase(hs.begin());
	TEST_EQUAL(hs.getSize(), 0)
	hs.erase(hs.begin());
	TEST_EQUAL(hs.getSize(), 0)

	HashSet<int> hs2;
	TEST_EXCEPTION(Exception::IncompatibleIterators, hs2.erase(hs.begin()))
RESULT

CHECK(void host(Visitor<HashSet<Key> >& visitor) throw())
	HashSet<int> hs;
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	MyVisitor mv;
	hs.host(mv);
	TEST_EQUAL(mv.value_sum, 6)
RESULT

CHECK(bool has(const Key& key) const throw())
	HashSet<int> hs;
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), false)
	TEST_EQUAL(hs.has(117), false)
	TEST_EQUAL(hs.has(-117), false)
	hs.insert(0);
	TEST_EQUAL(hs.has(0), true)
	TEST_EQUAL(hs.has(1), false)
	TEST_EQUAL(hs.has(117), false)
	TEST_EQUAL(hs.has(-117), false)
	hs.insert(1);
	TEST_EQUAL(hs.has(0), true)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(117), false)
	TEST_EQUAL(hs.has(-117), false)
	hs.insert(117);
	TEST_EQUAL(hs.has(0), true)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(117), true)
	TEST_EQUAL(hs.has(-117), false)
RESULT

CHECK(bool isEmpty() const throw())
	HashSet<int> hs;
	TEST_EQUAL(hs.isEmpty(), true)
	hs.insert(0);
	TEST_EQUAL(hs.isEmpty(), false)
	hs.insert(0);
	TEST_EQUAL(hs.isEmpty(), false)
	hs.insert(1);
	TEST_EQUAL(hs.isEmpty(), false)
RESULT

CHECK(bool operator == (const HashSet& hash_set) const throw())
	HashSet<int> hs;
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);

	HashSet<int> hs2;
	hs2.insert(1);
	hs2.insert(2);
	hs2.insert(3);
	TEST_EQUAL(hs == hs2, true)
	hs2.erase(2);
	hs2.insert(4);
	TEST_EQUAL(hs == hs2, false)
RESULT

CHECK(bool operator != (const HashSet& hash_set) const throw())
	HashSet<int> hs;
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);

	HashSet<int> hs2;
	hs2.insert(1);
	hs2.insert(2);
	hs2.insert(3);
	TEST_EQUAL(hs != hs2, false)
	hs2.erase(2);
	hs2.insert(4);
	TEST_EQUAL(hs != hs2, true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);

  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	hs.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(HashSet_test.txt))
RESULT

CHECK(bool isValid() const throw())
	HashSet<int> hs;
	TEST_EQUAL(hs.isValid(), true)
RESULT

CHECK([EXTRA] HashSet::apply(UnaryProcessor))
	HashSet<int> hs;
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	ItemCollector<int> myproc;
	TEST_EQUAL(hs.apply(myproc), true)
	TEST_EQUAL(myproc.getSize(), 3)
	TEST_EQUAL(*myproc.getPointer(), 3) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 1) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 2) myproc.forward();
RESULT

CHECK([EXTRA] HashSet STL compatibility)
	HashSet<int> hs;
	std::vector<int> v;
	v.push_back(4);
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);
	std::copy(v.begin(), v.end(), std::inserter(hs, hs.begin()));
	TEST_EQUAL(hs.size(), 4)
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.has(1), true)
	TEST_EQUAL(hs.has(2), true)
	TEST_EQUAL(hs.has(3), true)
	TEST_EQUAL(hs.has(4), true)

	std::list<int> il;
	std::copy(hs.begin(), hs.end(), std::front_inserter(il));
	TEST_EQUAL(il.size(), 4)
	TEST_EQUAL(std::find(il.begin(), il.end(), 0) == il.end(), true)
	TEST_EQUAL(std::find(il.begin(), il.end(), 1) != il.end(), true)
	TEST_EQUAL(std::find(il.begin(), il.end(), 2) != il.end(), true)
	TEST_EQUAL(std::find(il.begin(), il.end(), 3) != il.end(), true)
	TEST_EQUAL(std::find(il.begin(), il.end(), 4) != il.end(), true)
RESULT

CHECK(ValueType& getData() throw())
  // ???
RESULT

CHECK(bool apply(UnaryProcessor<ValueType>& processor) throw())
  // ???
RESULT


CHECK(ConstIterator begin() const throw())
  // ???
RESULT

CHECK(ConstIterator end() const throw())
  // ???
RESULT

CHECK(Iterator begin() throw())
  // ???
RESULT

CHECK(Iterator end() throw())
  // ???
RESULT

CHECK(Iterator insert(Iterator pos, const ValueType& item) throw())
  // ???
RESULT


CHECK(IllegalKey(const char* file, int line))
  // ???
RESULT

// ===================== Iterators ================================
CHECK(HashSet* getContainer() throw())
  // ???
RESULT

CHECK(IteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(IteratorTraits_& operator = (const IteratorTraits_& traits) throw())
  // ???
RESULT

CHECK(IteratorTraits_() throw())
  // ???
RESULT

CHECK(IteratorTraits_(const HashSet& hash_set) throw())
  // ???
RESULT

CHECK(IteratorTraits_(const IteratorTraits_& traits) throw())
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

CHECK(const HashSet* getContainer() const throw())
  // ???
RESULT

CHECK(const IteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const ValueType& getData() const throw())
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
