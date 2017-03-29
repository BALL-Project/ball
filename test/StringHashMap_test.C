// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/CONCEPT/visitor.h>
#include "ItemCollector.h"
///////////////////////////

using namespace BALL;
using namespace std;

class MyVisitor 
	: public Visitor<StringHashMap<int> >
{
	public:
	MyVisitor()
		: value_sum(0)
	{
	}
	int value_sum;

	void visit(StringHashMap<int>& map)
	{
		StringHashMap<int>::Iterator it = map.begin();
		for (; it != map.end(); ++it)
		{
			value_sum += (*it).second;
		}
	}
};

START_TEST(StringHashMap)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

StringHashMap<int>* map_ptr;
CHECK(StringHashMap() throw())
	map_ptr = new StringHashMap<int>;
	TEST_NOT_EQUAL(map_ptr, 0)
RESULT

CHECK(~StringHashMap() throw())
	delete map_ptr;
RESULT

CHECK(Size getSize() const throw())
	StringHashMap<int> hm;
	TEST_EQUAL(hm.getSize(), 0)
RESULT

CHECK((std::pair<Iterator, bool> insert(const ValueType& obj) throw()))
	StringHashMap<int> hm;
	TEST_EQUAL(hm.getSize(), 0)
	pair<String, int> p("a", 0);
	hm.insert(p);
	TEST_EQUAL(hm.getSize(), 1)
	hm.insert(p);
	TEST_EQUAL(hm.getSize(), 1)
	pair<String, int> p2("b", 1);
	hm.insert(p2);
	TEST_EQUAL(hm.getSize(), 2)
	TEST_EQUAL(hm.has("a"), true)
	TEST_EQUAL(hm.has("b"), true)
RESULT

CHECK((::std::pair<Iterator, bool> insert(const String& key, const Value& value) throw()))
	StringHashMap<int> hm;
	TEST_EQUAL(hm.getSize(), 0)
	hm.insert("a", 0);
	TEST_EQUAL(hm.getSize(), 1)
	hm.insert("a", 1);
	TEST_EQUAL(hm.getSize(), 1)
	hm.insert("b", 0);
	TEST_EQUAL(hm.getSize(), 2)
	hm.insert("b", 1);
	TEST_EQUAL(hm.getSize(), 2)
RESULT

// testing inherited method function
CHECK([EXTRA] find(const String& key))
	StringHashMap<int> hm;
	TEST_EQUAL((hm.find("a") == hm.end()), true)
	hm.insert("a", 0);
	TEST_EQUAL((hm.find("a") == hm.end()), false)
	TEST_EQUAL((hm.find("a") == hm.begin()), true)
	hm.insert("b", 1);
	TEST_EQUAL((hm.find("a") == hm.end()), false)
	TEST_EQUAL((hm.find("b") == hm.end()), false)
	TEST_EQUAL((hm.find("c") == hm.end()), true)
RESULT

CHECK([EXTRA]StringHashMap::erase(const KeyType& key))
	StringHashMap<int> hm;
	hm.insert("a", 0);
	hm.insert("b", 1);
	hm.insert("c", 2);
	hm.erase("a");
	TEST_EQUAL(hm.has("a"), false)
	TEST_EQUAL(hm.has("b"), true)
	TEST_EQUAL(hm.has("c"), true)
	TEST_EQUAL(hm.getSize(), 2)
	hm.erase("sss");
	TEST_EQUAL(hm.has("a"), false)
	TEST_EQUAL(hm.has("b"), true)
	TEST_EQUAL(hm.has("c"), true)
	TEST_EQUAL(hm.getSize(), 2)
RESULT

StringHashMap<int> hm;

CHECK(StringHashMap(const StringHashMap& map) throw())
	hm.insert("a", 0);
	hm.insert("b", 1);
	hm.insert("c", 2);

	StringHashMap<int> hm2(hm);
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(void destroy() throw())
	StringHashMap<int> hm;
	hm.insert("a", 0);
	hm.destroy();
	TEST_EQUAL(hm.getSize(), 0)
RESULT

CHECK(void set(const StringHashMap& hash_map) throw())
	StringHashMap<int> hm2;
	hm2.set(hm);
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(void get(StringHashMap& hash_map) const throw())
	StringHashMap<int> hm2;
	hm.get(hm2);
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(void swap(StringHashMap& hash_map) throw())
	StringHashMap<int> hm, hm2;
	hm.insert("a", 0);
	hm.insert("b", 1);

	hm2.insert("c", 10);
	hm2.insert("d", 11);
	hm2.insert("e", 12);

	hm.swap(hm2);

	TEST_EQUAL(hm2.getSize(), 2)
	TEST_EQUAL(hm.getSize(), 3)
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm["c"], 10)
	TEST_EQUAL(hm["d"], 11)
	TEST_EQUAL(hm["e"], 12)
RESULT

CHECK(const StringHashMap& operator = (const StringHashMap& hash_map) throw())
	StringHashMap<int> hm2 = hm;
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(void host(Visitor<StringHashMap<Value> >& visitor) throw())
	StringHashMap<int> hm;
	hm.insert("a", 200);
	hm.insert("b", 22);

	MyVisitor mv;
	hm.host(mv);
	TEST_EQUAL(mv.value_sum, 222)
RESULT

CHECK(bool has(const String& key) const throw())
	StringHashMap<int> hm;
	hm.insert("a", 0);
	hm.insert("b", 1);
	TEST_EQUAL(hm.has("a"), true)
	TEST_EQUAL(hm.has("b"), true)
	TEST_EQUAL(hm.has("c"), false)
RESULT

CHECK(bool isEmpty() const throw())
	StringHashMap<int> hm;
	TEST_EQUAL(hm.isEmpty(), true)
	hm.insert("a", 0);
	TEST_EQUAL(hm.isEmpty(), false)
	hm.clear();
	TEST_EQUAL(hm.isEmpty(), true)
RESULT

CHECK(bool operator == (const StringHashMap<Value>& hash_map) const throw())
	StringHashMap<int> hm;
	StringHashMap<int> hm2;
	TEST_EQUAL(hm == hm2, true)

	hm.insert("a", 0);
	hm.insert("b", 1);

	hm2.insert("a", 0);
	hm2.insert("b", 1);
	TEST_EQUAL(hm == hm2, true)

	hm2.insert("b", 2);
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm == hm2, true)

	hm2.insert("b", 1);
	hm2.insert("c", 1);
	TEST_EQUAL(hm == hm2, false)
RESULT

CHECK(BALL_CREATE(StringHashMap))
 	StringHashMap<int> hm;
	hm.insert("a", 0);
	StringHashMap<int>* p = (StringHashMap<int>*) hm.create(false, true);
	StringHashMap<int> empty;
	TEST_EQUAL(*p == empty, true)
	delete p;
	p = (StringHashMap<int>*)hm.create();
	bool test = (*p == hm);
	TEST_EQUAL(test, true)
	delete p;
RESULT

CHECK(bool remove(const String& key) throw())
	StringHashMap<int> hm;
	hm.insert("a", 0);
	hm.insert("b", 1);
	hm.insert("c", 2);
	TEST_EQUAL(hm.remove("a"), true)
	TEST_EQUAL(hm.has("a"), false)
	TEST_EQUAL(hm.has("b"), true)
	TEST_EQUAL(hm.has("c"), true)
	TEST_EQUAL(hm.getSize(), 2)
	TEST_EQUAL(hm.remove("sss"), false)
	TEST_EQUAL(hm.has("a"), false)
	TEST_EQUAL(hm.has("b"), true)
	TEST_EQUAL(hm.has("c"), true)
	TEST_EQUAL(hm.getSize(), 2)
RESULT

CHECK([EXTRA] large_entries)
	StringHashMap<String> shm;
	for (Position p = 1; p < 1000; p+= 100)
	{
		String key(p);
		String value;
		for (Position i = p; i < p * 10; i++) value += "X";

		shm[key] = value;
		TEST_EQUAL(shm[key], value)

		shm[value] = key;
		TEST_EQUAL(shm[value], key)
	}
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
