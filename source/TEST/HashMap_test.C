// $Id: HashMap_test.C,v 1.1 1999/12/28 18:14:27 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashMap.h>

///////////////////////////

START_TEST(HashMap<T>, "$Id: HashMap_test.C,v 1.1 1999/12/28 18:14:27 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

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

CHECK(HashMap::erase(Iterator pos))
// BAUSTELLE
RESULT

CHECK(HashMap::erase(Iterator first, Iterator last))
// BAUSTELLE
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
// BAUSTELLE
RESULT

CHECK(HashMap::clear())
// BAUSTELLE
RESULT

CHECK(HashMap::destroy())
// BAUSTELLE
RESULT

CHECK(HashMap::set(const HashMap&, bool))
// BAUSTELLE
RESULT

CHECK(HashMap::get(HashMap&, bool) const)
// BAUSTELLE
RESULT

CHECK(HashMap::swap(HashMap&, bool))
// BAUSTELLE
RESULT

CHECK(HashMap::operator = (const HashMap&))
// BAUSTELLE
RESULT


CHECK(HashMap::host(Visitor<int>&))
// BAUSTELLE
RESULT

CHECK(HashMap::has(const Key&))
// BAUSTELLE
RESULT

CHECK(HashMap::isEmpty())
// BAUSTELLE
RESULT

CHECK(HashMap::operator == (const HashMap&) const)
// BAUSTELLE
RESULT

CHECK(HashMap::operator != (const HashMap&) const)
// BAUSTELLE
RESULT

CHECK(HashMap::dump(std::ostream&, Size) const)
// BAUSTELLE
RESULT

CHECK(HashMap::isValid() const)
HashMap<int, int> hm;
TEST_EQUAL(hm.isValid(), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
