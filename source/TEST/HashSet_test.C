// $Id: HashSet_test.C,v 1.1 1999/12/28 18:14:28 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashSet.h>

///////////////////////////

START_TEST(HashSet<T>, "$Id: HashSet_test.C,v 1.1 1999/12/28 18:14:28 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HashSet<int>* set_ptr;
CHECK(HashSet::HashSet())
set_ptr = new HashSet<int>;
TEST_NOT_EQUAL(set_ptr, 0)
RESULT

CHECK(HashSet::~HashSet())
delete set_ptr;
RESULT

CHECK(HashSet::size())
HashSet<int> hs;
TEST_EQUAL(hs.size(), 0)
RESULT

CHECK(HashSet::getSize())
HashSet<int> hs;
TEST_EQUAL(hs.getSize(), 0)
RESULT

CHECK(HashSet::HashSet(const HashSet&, bool))
// BAUSTELLE
RESULT

CHECK(HashSet::clear())
// BAUSTELLE
RESULT

CHECK(HashSet::destroy())
// BAUSTELLE
RESULT

CHECK(HashSet::set(const HashSet&, bool))
// BAUSTELLE
RESULT

CHECK(HashSet::get(HashSet&, bool) const)
// BAUSTELLE
RESULT

CHECK(HashSet::swap(HashSet&, bool))
// BAUSTELLE
RESULT

CHECK(HashSet::operator = (const HashSet&))
// BAUSTELLE
RESULT

CHECK(HashSet::getBucketSize() const)
// BAUSTELLE
RESULT

CHECK(HashSet::getCapacity())
// BAUSTELLE
RESULT

CHECK(HashSet::insert(const ValueType& entry))
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

CHECK(HashSet::find(const int& key))
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

CHECK(HashSet::find(const int& key) const)
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

CHECK(HashSet::erase(const ValueType& entry))
// BAUSTELLE
RESULT

CHECK(HashSet::erase(Iterator first, Iterator last))
// BAUSTELLE
RESULT

CHECK(HashSet::erase(Iterator first, Iterator last))
// BAUSTELLE
RESULT

CHECK(HashSet::host(Visitor<int>&))
// BAUSTELLE
RESULT

CHECK(HashSet::has(const Key&))
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

CHECK(HashSet::isEmpty())
HashSet<int> hs;
TEST_EQUAL(hs.isEmpty(), true)
hs.insert(0);
TEST_EQUAL(hs.isEmpty(), false)
hs.insert(0);
TEST_EQUAL(hs.isEmpty(), false)
hs.insert(1);
TEST_EQUAL(hs.isEmpty(), false)
RESULT

CHECK(HashSet::operator == (const HashSet&) const)
// BAUSTELLE
RESULT

CHECK(HashSet::operator != (const HashSet&) const)
// BAUSTELLE
RESULT

CHECK(HashSet::dump(std::ostream&, Size) const)
// BAUSTELLE
RESULT

CHECK(HashSet::isValid() const)
HashSet<int> hs;
TEST_EQUAL(hs.isValid(), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
