// $Id: StringHashMap_test.C,v 1.2 2000/12/01 16:10:42 amoll Exp $
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

START_TEST(StringHashMap, "$Id: StringHashMap_test.C,v 1.2 2000/12/01 16:10:42 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

StringHashMap<int>* map_ptr;
CHECK(StringHashMap::StringHashMap())
	map_ptr = new StringHashMap<int>;
	TEST_NOT_EQUAL(map_ptr, 0)
RESULT

CHECK(StringHashMap::~StringHashMap())
	delete map_ptr;
RESULT

CHECK(StringHashMap::getSize())
	StringHashMap<int> hm;
	TEST_EQUAL(hm.getSize(), 0)
RESULT

CHECK(StringHashMap::insert(const ValueType& entry))
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

CHECK(StringHashMap::insert(const String& key, const Value& value))
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
CHECK(HashMap::find(const String& key))
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

CHECK(StringHashMap::erase(const KeyType& key))
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

CHECK(StringHashMap::StringHashMap(const StringHashMap&, bool))
	hm.insert("a", 0);
	hm.insert("b", 1);
	hm.insert("c", 2);

	StringHashMap<int> hm2(hm);
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(StringHashMap::destroy())
	StringHashMap<int> hm;
	hm.insert("a", 0);
	hm.destroy();
	TEST_EQUAL(hm.getSize(), 0)
	TEST_EQUAL(hm.getCapacity(), 4)
	TEST_EQUAL(hm.getBucketSize(), 3)
RESULT

CHECK(StringHashMap::set(const StringHashMap&, bool))
	StringHashMap<int> hm2;
	hm2.set(hm);
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(StringHashMap::get(StringHashMap&, bool) const)
	StringHashMap<int> hm2;
	hm.get(hm2);
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(StringHashMap::swap(StringHashMap&, bool))
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

CHECK(StringHashMap::operator = (const StringHashMap&))
	StringHashMap<int> hm2 = hm;
	TEST_EQUAL(hm2["a"], 0)
	TEST_EQUAL(hm2["b"], 1)
	TEST_EQUAL(hm2["c"], 2)
	TEST_EQUAL(hm2.getSize(), 3)
RESULT

CHECK(StringHashMap::host(Visitor<Value>& visitor))
	StringHashMap<int> hm;
	hm.insert("a", 200);
	hm.insert("b", 22);

	MyVisitor mv;
	hm.host(mv);
	TEST_EQUAL(mv.value_sum, 222)
RESULT

CHECK(StringHashMap::has(const Key&))
	StringHashMap<int> hm;
	hm.insert("a", 0);
	hm.insert("b", 1);
	TEST_EQUAL(hm.has("a"), true)
	TEST_EQUAL(hm.has("b"), true)
	TEST_EQUAL(hm.has("c"), false)
RESULT

CHECK(StringHashMap::isEmpty())
	StringHashMap<int> hm;
	TEST_EQUAL(hm.isEmpty(), true)
	hm.insert("a", 0);
	TEST_EQUAL(hm.isEmpty(), false)
	hm.clear();
	TEST_EQUAL(hm.isEmpty(), true)
RESULT

CHECK(StringHashMap::operator == (const StringHashMap&) const)
	StringHashMap<int> hm;
	StringHashMap<int> hm2;
	TEST_EQUAL(hm == hm2, true)

	hm.insert("a", 0);
	hm.insert("b", 1);

	hm2.insert("a", 0);
	hm2.insert("b", 1);
	TEST_EQUAL(hm == hm2, true)

	hm2.insert("b", 2);
	TEST_EQUAL(hm2["b"], 2)
	TEST_EQUAL(hm == hm2, false)

	hm2.insert("b", 1);
	hm2.insert("c", 1);
	TEST_EQUAL(hm == hm2, false)
RESULT

CHECK(StringHashMap::operator != (const StringHashMap&) const)
	StringHashMap<int> hm;
	StringHashMap<int> hm2;

	TEST_EQUAL(hm != hm2, false)
	hm.insert("a", 0);
	hm.insert("b", 1);

	hm2.insert("a", 0);
	hm2.insert("b", 1);

	TEST_EQUAL(hm != hm2, false)
	hm2.insert("c", 1);
	TEST_EQUAL(hm != hm2, true)
RESULT

CHECK(StringHashMap::isValid() const)
	StringHashMap<int> hm;
	TEST_EQUAL(hm.isValid(), true)
RESULT

CHECK(StringHashMap::getLoadFactor() const)
	StringHashMap<int> hm;
	TEST_EQUAL(hm.getLoadFactor(), 0)
	hm.insert("a", 0);
	hm.insert("b", 1);
	hm.insert("c", 1);
	TEST_EQUAL(hm.getLoadFactor(), 1)
RESULT

// testing inherited method function
CHECK(HashMap::apply(UnaryProcessor))
	StringHashMap<int> hm;
	hm.insert("a", 0);
	hm.insert("b", 1);
	ItemCollector<pair<String,int> > myproc;
	TEST_EQUAL(hm.apply(myproc), true)
	TEST_EQUAL(myproc.getSize(), 2)
	TEST_EQUAL(myproc.getPointer()->first, "b") myproc.forward();
	TEST_EQUAL(myproc.getPointer()->first, "a") myproc.forward();
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
