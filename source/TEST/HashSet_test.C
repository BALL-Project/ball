// $Id: HashSet_test.C,v 1.3 2000/09/04 12:47:21 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/CONCEPT/visitor.h>
///////////////////////////

using namespace BALL;
using namespace std;

class MyVisitor 
	: public Visitor <int>
{
	public:
	MyVisitor()
		: value_sum(0)
	{
	}
	int value_sum;
	void visit(int& v)
	{
		value_sum += v;
	}
};

START_TEST(HashSet<T>, "$Id: HashSet_test.C,v 1.3 2000/09/04 12:47:21 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

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

CHECK(HashSet::HashSet(const HashSet&))
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);

	HashSet<int>* set_ptr;
	set_ptr = new HashSet<int>(hs);
	TEST_NOT_EQUAL(set_ptr, 0)
	TEST_EQUAL(set_ptr->getSize(), 3)
	TEST_EQUAL(set_ptr->has(0), true)
	TEST_EQUAL(set_ptr->has(1), true)
	TEST_EQUAL(set_ptr->has(2), true)
	TEST_EQUAL(set_ptr->has(3), false)
RESULT

CHECK(HashSet::clear())
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.clear();
	TEST_EQUAL(hs.getSize(), 0)
	TEST_EQUAL(hs.getCapacity(), 4)
	TEST_EQUAL(hs.getBucketSize(), 3)
RESULT

CHECK(HashSet::destroy())
	HashSet<int> hs;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.destroy();
	TEST_EQUAL(hs.getSize(), 0)
	TEST_EQUAL(hs.getCapacity(), 4)
	TEST_EQUAL(hs.getBucketSize(), 3)
RESULT

CHECK(HashSet::set(const HashSet&, bool))
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs2.set(hs);

	TEST_EQUAL(hs2.getSize(), 3)
	TEST_EQUAL(hs2.getCapacity(), 4)
	TEST_EQUAL(hs2.getBucketSize(), 3)
RESULT

CHECK(HashSet::get(HashSet&, bool) const)
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs.get(hs2);

	TEST_EQUAL(hs2.getSize(), 3)
	TEST_EQUAL(hs2.getCapacity(), 4)
	TEST_EQUAL(hs2.getBucketSize(), 3)
RESULT

CHECK(HashSet::swap(HashSet&, bool))
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

CHECK(HashSet::operator = (const HashSet&))
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.insert(1);
	hs.insert(2);
	hs2 = hs;

	TEST_EQUAL(hs2.getSize(), 3)
	TEST_EQUAL(hs2.getCapacity(), 4)
	TEST_EQUAL(hs2.getBucketSize(), 3)
RESULT

CHECK(HashSet::getBucketSize() const)
	HashSet<int> hs;
	TEST_EQUAL(hs.getBucketSize(), 3)
RESULT

CHECK(HashSet::getCapacity())
	HashSet<int> hs;
	TEST_EQUAL(hs.getCapacity(), 4)
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
	HashSet<int> hs, hs2;
	hs.insert(0);
	hs.erase(0);
	TEST_EQUAL(hs.has(0), false)
	TEST_EQUAL(hs.getSize(), 0)
RESULT

CHECK(HashSet::erase(Iterator first, Iterator last))
	// BAUSTELLE
RESULT

CHECK(HashSet::erase(Iterator first, Iterator last))
	// BAUSTELLE not yet implemented
RESULT

CHECK(HashSet::host(Visitor<int>&))
	HashSet<int> hs;
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);
	MyVisitor mv;
	hs.host(mv);
	TEST_EQUAL(mv.value_sum, 6)
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

CHECK(HashSet::operator != (const HashSet&) const)
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

CHECK(HashSet::dump(std::ostream&, Size) const)
	HashSet<int> hs;
	hs.insert(1);
	hs.insert(2);
	hs.insert(3);

  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	hs.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/HashSet_test.txt", true)
RESULT

CHECK(HashSet::isValid() const)
	HashSet<int> hs;
	TEST_EQUAL(hs.isValid(), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
