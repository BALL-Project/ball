// $Id: HashMap_test.C,v 1.6 2000/09/01 11:11:29 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/CONCEPT/visitor.h>
///////////////////////////

using namespace BALL;
using namespace std;

class MyVisitor 
	: public Visitor <pair<int, int> >
{
	public:
	MyVisitor()
		: value_sum(0),
			key_sum(0)
	{
	}
	int value_sum;
	int key_sum;
	void visit(pair<int, int>& v)
	{
		key_sum += v.first;
		value_sum += v.second;
	}
};



START_TEST(HashMap, "$Id: HashMap_test.C,v 1.6 2000/09/01 11:11:29 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

HashMap<int, int>* map_ptr;
CHECK(HashMap::HashMap())
	map_ptr = new HashMap<int, int>;
	TEST_NOT_EQUAL(map_ptr, 0)
RESULT

CHECK(HashMap::~HashMap())
	delete map_ptr;
RESULT

CHECK(HashMap::size())
	HashMap<int, int> hm;
	TEST_EQUAL(hm.size(), 0)
RESULT

CHECK(HashMap::getSize())
	HashMap<int, int> hm;
	TEST_EQUAL(hm.getSize(), 0)
RESULT

CHECK(HashMap::getBucketSize() const)
	HashMap<int, int> hm;
	TEST_EQUAL(hm.getBucketSize(), 3)
RESULT

CHECK(HashMap::getCapacity())
	HashMap<int, int> hm;
	TEST_EQUAL(hm.getCapacity(), 4)
RESULT

CHECK(HashMap::insert(const ValueType& entry))
	HashMap<int, int> hm;
	TEST_EQUAL(hm.getSize(), 0)
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	TEST_EQUAL(hm.getSize(), 1)
	hm.insert(HashMap<int, int>::ValueType(0, 1));
	TEST_EQUAL(hm.getSize(), 1)
	hm.insert(HashMap<int, int>::ValueType(1, 0));
	TEST_EQUAL(hm.getSize(), 2)
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL(hm.getSize(), 2)
RESULT

CHECK(HashMap::find(const int& key))
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

CHECK(HashMap::find(const int& key) const)
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

CHECK(HashMap::erase(const KeyType& key))
	HashMap<int, int> hm;
	TEST_EQUAL(hm.getSize(), 0)
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	TEST_EQUAL(hm.getSize(), 1)
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL(hm.getSize(), 2)
	hm.insert(HashMap<int, int>::ValueType(2, 2));
	TEST_EQUAL(hm.find(1) == hm.end(), false)
	hm.erase(1);
	TEST_EQUAL(hm.find(1) == hm.end(), true)
	TEST_EQUAL(hm.find(0) == hm.end(), false)
	TEST_EQUAL(hm.find(2) == hm.end(), false)
	TEST_EQUAL(hm.find(5) == hm.end(), true)
	TEST_EQUAL(hm.getSize(), 2)
	hm.erase(5);
	TEST_EQUAL(hm.find(1) == hm.end(), true)
	TEST_EQUAL(hm.find(0) == hm.end(), false)
	TEST_EQUAL(hm.find(2) == hm.end(), false)
	TEST_EQUAL(hm.find(5) == hm.end(), true)
	TEST_EQUAL(hm.getSize(), 2)
RESULT

CHECK(HashMap::erase(Iterator first, Iterator last))/*
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	hm.insert(HashMap<int, int>::ValueType(2, 2));
	hm.insert(HashMap<int, int>::ValueType(3, 3));
	hm.erase((hm.begin()+1), (hm.end()-1));
	TEST_EQUAL(hm.getSize(), 2)
	hm.erase(hm.begin(), hm.end());
	hm.erase(hm.find(1), hm.find(2));
	TEST_EQUAL(hm.getSize(), 0)*/
	// Not yet implemented BAUSTELLE
RESULT

CHECK(HashMap::operator [] (const int& key))
	HashMap<int, int> hm;
	hm[0] = 0;
	hm[0] = 1;
	hm[1] = 2;
	hm[2] = 4;
	hm[3] = 8;
	hm[4] = 16;
	hm[5] = 32;
	TEST_EQUAL(hm.getSize(), 6)
	TEST_EQUAL(hm[0], 1)
	TEST_EQUAL(hm[1], 2)
	TEST_EQUAL(hm[2], 4)
	TEST_EQUAL(hm[3], 8)
	TEST_EQUAL(hm[4], 16)
	TEST_EQUAL(hm[5], 32)
RESULT

CHECK(HashMap::operator [] (const int& key) const)
	HashMap<int, int> hm;
	hm[0] = 0;
	hm[0] = 1;
	hm[1] = 2;
	hm[2] = 4;
	hm[3] = 8;
	hm[4] = 16;
	hm[5] = 32;
	const HashMap<int, int>& const_map = const_cast<const HashMap<int, int>&>(hm);
	TEST_EQUAL(const_map.getSize(), 6)
	TEST_EQUAL(const_map[0], 1)
	TEST_EQUAL(const_map[1], 2)
	TEST_EQUAL(const_map[2], 4)
	TEST_EQUAL(const_map[3], 8)
	TEST_EQUAL(const_map[4], 16)
	TEST_EQUAL(const_map[5], 32)
RESULT

CHECK(HashMap::HashMap(const HashMap&, bool))
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

CHECK(HashMap::clear())
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.clear();
	TEST_EQUAL(hm.getSize(), 0)
	TEST_EQUAL(hm.getCapacity(), 4)
	TEST_EQUAL(hm.getBucketSize(), 3)
RESULT

CHECK(HashMap::destroy())
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.destroy();
	TEST_EQUAL(hm.getSize(), 0)
	TEST_EQUAL(hm.getCapacity(), 4)
	TEST_EQUAL(hm.getBucketSize(), 3)
RESULT

CHECK(HashMap::set(const HashMap&, bool))
	HashMap<int, int> hm1;
	hm1.insert(pair<int, int>(12, 34));
	hm1.insert(pair<int, int>(44, 55));
	HashMap<int, int> hm2;
	TEST_EQUAL(hm1.getSize(), 2)
	TEST_EQUAL(hm2.getSize(), 0)
	TEST_EQUAL(hm1.has(12), true)
	TEST_EQUAL(hm1.has(44), true)
	hm2.set(hm1);
	TEST_EQUAL(hm2.getSize(), 2)
	TEST_EQUAL(hm2.has(12), true)
	TEST_EQUAL(hm2.has(44), true)
	TEST_EQUAL(hm1.getSize(), 2)
	TEST_EQUAL(hm1.has(12), true)
	TEST_EQUAL(hm1.has(44), true)
RESULT

CHECK(HashMap::get(HashMap&, bool) const)
	HashMap<int, int> hm, hm2;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm.get(hm2);
	TEST_EQUAL(hm2[0], 0)
	TEST_EQUAL(hm2[1], 1)
RESULT

CHECK(HashMap::swap(HashMap&, bool))
	HashMap<int, int> hm, hm2;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm2.insert(HashMap<int, int>::ValueType(10, 10));
	hm2.insert(HashMap<int, int>::ValueType(11, 11));

	TEST_EQUAL(hm.getSize(), 2)
	TEST_EQUAL(hm2.getSize(), 2)
	TEST_EQUAL(hm2[10], 10)
	TEST_EQUAL(hm2[11], 11)
	TEST_EQUAL(hm[0], 0)
	TEST_EQUAL(hm[1], 1)
	hm.swap(hm2);
	TEST_EQUAL(hm.getSize(), 2)
	TEST_EQUAL(hm2.getSize(), 2)
	TEST_EQUAL(hm[10], 10)
	TEST_EQUAL(hm[11], 11)
	TEST_EQUAL(hm2[0], 0)
	TEST_EQUAL(hm2[1], 1)
RESULT

CHECK(HashMap::operator = (const HashMap&))
	HashMap<int, int> hm, hm2;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm2 = hm;
	TEST_EQUAL(hm2[0], 0)
	TEST_EQUAL(hm2[1], 1)
RESULT

CHECK(HashMap::host(Visitor<pair<key, value> >& visitor))
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(100, 200));
	hm.insert(HashMap<int, int>::ValueType(11, 22));

	MyVisitor mv;
	hm.host(mv);
	TEST_EQUAL(mv.key_sum, 111)
	TEST_EQUAL(mv.value_sum, 222)
RESULT

CHECK(HashMap::has(const Key&))
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));
	TEST_EQUAL(hm.has(0), true)
	TEST_EQUAL(hm.has(1), true)
	TEST_EQUAL(hm.has(2), false)
RESULT

CHECK(HashMap::isEmpty())
	HashMap<int, int> hm;
	TEST_EQUAL(hm.isEmpty(), true)
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	TEST_EQUAL(hm.isEmpty(), false)
	hm.clear();
	TEST_EQUAL(hm.isEmpty(), true)
RESULT

CHECK(HashMap::operator == (const HashMap&) const)
	HashMap<int, int> hm;
	HashMap<int, int> hm2;
	TEST_EQUAL(hm == hm2, true)

	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

	hm2.insert(HashMap<int, int>::ValueType(0, 0));
	hm2.insert(HashMap<int, int>::ValueType(1, 1));

	TEST_EQUAL(hm == hm2, true)
	hm2.insert(HashMap<int, int>::ValueType(2, 1));
	TEST_EQUAL(hm == hm2, false)
RESULT

CHECK(HashMap::operator != (const HashMap&) const)
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

CHECK(HashMap::dump(std::ostream&, Size) const)
	HashMap<int, int> hm;
	hm.insert(HashMap<int, int>::ValueType(0, 0));
	hm.insert(HashMap<int, int>::ValueType(1, 1));

  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	hm.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/HashMap_test.txt", true)
RESULT

CHECK(HashMap::isValid() const)
	HashMap<int, int> hm;
	TEST_EQUAL(hm.isValid(), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
