// $Id: HashGrid3_test.C,v 1.7 2002/01/16 02:16:56 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/DATATYPE/hashGrid.h>
#include "HashGrid3_test.h"

///////////////////////////

START_TEST(HashGrid, "$Id: HashGrid3_test.C,v 1.7 2002/01/16 02:16:56 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class HashGridBox3::

HashGridBox3<int>* hbox3;

CHECK(HashGridBox3::HashGridBox3() throw())
	hbox3 = new HashGridBox3<int>;
	TEST_NOT_EQUAL(hbox3, 0)
RESULT


CHECK(HashGridBox3::HashGridBox3(const HashGridBox3& grid_box, bool deep = true) throw())
  // ?????
	// uses set(), which isn't implemented
RESULT


CHECK(HashGridBox3::~HashGridBox3() throw())
	delete hbox3;
RESULT


CHECK(HashGridBox3::clear() throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	hbox.clear();
	// TODO: did it really clear?
RESULT


CHECK(HashGridBox3::destroy() throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	hbox.destroy();
	// TODO: did it really destroy?
RESULT


CHECK(HashGridBox3::set(const HashGridBox3& box, bool /* deep */ = true) throw())
  //?????
RESULT


CHECK(HashGridBox3::HashGridBox3& operator = (const HashGridBox3& box) throw())
  //?????
RESULT


CHECK(HashGridBox3::get(HashGridBox3& box, bool deep = true) const  throw())
  //?????
RESULT


CHECK(HashGridBox3::find(const Item &item) throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	int* found = hbox.find(3);
	TEST_NOT_EQUAL(found, 0)
	bool test = (*found == 3);
	TEST_EQUAL(test, true)
	found = hbox.find(9);
	TEST_EQUAL(found, 0)
RESULT


CHECK(HashGridBox3::find(const Item& item) const  throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	const int* found = hbox.find(3);
	TEST_NOT_EQUAL(found, 0)
	bool test = (*found == 3);
	TEST_EQUAL(test, true)
	found = hbox.find(9);
	TEST_EQUAL(found, 0)
RESULT


CHECK(HashGridBox3::getSize() const  throw())
	HashGridBox3<int> hbox;
	int size = hbox.getSize();
	TEST_EQUAL(size, 0)
	int test_int = 5;
	hbox.insert(test_int);
	size = hbox.getSize();
	TEST_EQUAL(size, 1)
RESULT


CHECK(HashGridBox3::insert(const Item& item) throw())
	HashGridBox3<int> hbox;
	hbox.insert(567);
	bool test = (*hbox.find(567) == 567);
	TEST_EQUAL(test, true)
RESULT


CHECK(HashGridBox3::remove(const Item& item) throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	hbox.remove(3);
	int size = hbox.getSize();
	TEST_EQUAL(size, 4)
	// TODO:
	// check if really the item "3" was removed.
RESULT


CHECK(HashGridBox3::removeAll(const Item& item) throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	int size = hbox.getSize();
	hbox.removeAll(3);
	int new_size = hbox.getSize();
	TEST_EQUAL(size - new_size, 2)
RESULT


CHECK(HashGridBox3::host(Visitor<HashGridBox3> &visitor) throw())
  //?????
RESULT


CHECK(HashGridBox3::bool operator == (const HashGridBox3& box) const  throw())
	HashGridBox3<int> hbox1;
	for (int i = 0; i < 5; i++)
	{
		hbox1.insert(i);
	}
	HashGridBox3<int> hbox2;
	bool test = (hbox1 == hbox2);
	TEST_NOT_EQUAL(test, true);
	for (int i = 0; i < 5; i++)
	{
		hbox2.insert(i);
	}
	test = (hbox1 == hbox2);
	TEST_EQUAL(test, true);
RESULT


CHECK(HashGridBox3::bool operator != (const HashGridBox3& box) const  throw())
	HashGridBox3<int> hbox1;
	for (int i = 0; i < 5; i++)
	{
		hbox1.insert(i);
	}
	HashGridBox3<int> hbox2;
	bool test = (hbox1 != hbox2);
	TEST_EQUAL(test, true);
	for (int i = 0; i < 5; i++)
	{
		hbox2.insert(i);
	}
	test = (hbox1 != hbox2);
	TEST_NOT_EQUAL(test, true);
RESULT


CHECK(HashGridBox3::has(const Item& item) const  throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	bool test = hbox.has(8);
	TEST_NOT_EQUAL(test, true)
	test = hbox.has(0);
	TEST_EQUAL(test, true)
	test = hbox.has(4);
	TEST_EQUAL(test, true)
	test = hbox.has(3);
	TEST_EQUAL(test, true)
RESULT


CHECK(HashGridBox3::isEmpty() const  throw())
	HashGridBox3<int> hbox;
	bool test = hbox.isEmpty();
	TEST_EQUAL(test, true)
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	test = hbox.isEmpty();
	TEST_EQUAL(test, false)
RESULT


CHECK(HashGridBox3::isValid() const  throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	bool test = hbox.isValid();
	TEST_EQUAL(test, true)
RESULT


CHECK(HashGridBox3::dump(std::ostream& s = std::cout, Size depth = 0) const  throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	std::ofstream dump_stream(tmp_filename.c_str(), std::ios::out);
	hbox.dump(dump_stream);
	STATUS(tmp_filename)
	dump_stream.clear();
	dump_stream.close();
	TEST_FILE(tmp_filename.c_str(), "data/hashgrid3_test_dump0.txt", true)
RESULT


CHECK(HashGridBox3::apply(UnaryProcessor<Item>& processor) throw())
	TestProcessor proc;
	HashGridBox3<int> hbox;
	hbox.insert(5);
	hbox.apply(proc);
	int* result = hbox.find(6);
	TEST_NOT_EQUAL(result, 0)
	bool test = (*result == 6);
	TEST_EQUAL(test, true)
RESULT


CHECK(HashGridBox3::apply(UnaryProcessor< HashGridBox3<Item> >& processor) throw())
	TestProcessor2 proc;
	HashGridBox3<int> hbox;
	hbox.insert(5);
	hbox.apply(proc);
	int* result = hbox.find(6);
	TEST_NOT_EQUAL(result, 0)
	ABORT_IF(result == 0)
	bool test = (*result == 6);
	TEST_EQUAL(test, true)
	// TODO: more complex example
RESULT


// tests for class HashGridBox3::BoxIteratorTraits_::

HashGridBox3<int>::BoxIteratorTraits_* it_tr_ptr;
CHECK(HashGridBox3::BoxIteratorTraits_::BoxIteratorTraits_() throw())
	it_tr_ptr = new HashGridBox3<int>::BoxIteratorTraits_;
	TEST_NOT_EQUAL(it_tr_ptr, 0)
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::BoxIteratorTraits_(const HashGridBox3& box) throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true) throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits) throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::getContainer() const  throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::getPosition() throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::bool operator == (const BoxIteratorTraits_& traits) const  throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::isValid() const  throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::toBegin() throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::toEnd() throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::getData() throw())
  //?????
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::forward() throw())
  //?????
RESULT


// tests for class DataIteratorTraits_::

CHECK(DataIteratorTraits_::(DataIteratorTraits_)() throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::DataIteratorTraits_(const HashGridBox3& box) throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::DataIteratorTraits_(const DataIteratorTraits_& traits, bool /* deep */ = true) throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::DataIteratorTraits_& operator = (const DataIteratorTraits_ &traits) throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::getContainer() const  throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::getPosition() throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::bool operator == (const DataIteratorTraits_ &traits) const  throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::isValid() const  throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::toBegin() throw())
RESULT


CHECK(DataIteratorTraits_::toEnd() throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::getData() throw())
  //?????
RESULT


CHECK(DataIteratorTraits_::forward() throw())
  //?????
RESULT


// tests for class HashGrid3::

HashGrid3<int>* hg_ptr;

CHECK(HashGrid3::HashGrid3() throw())
	hg_ptr = new HashGrid3<int>;
	TEST_NOT_EQUAL(hg_ptr, 0)
RESULT


CHECK(HashGrid3::HashGrid3(const Vector3& origin, Size dimension_x, Size dimension_y, Size dimension_z, float spacing_x, float spacing_y, float spacing_z) throw())
  //?????
RESULT


CHECK(HashGrid3::HashGrid3(const Vector3& origin, Size dimension_x, Size dimension_y, Size dimension_z, float spacing) throw())
  //?????
RESULT


CHECK(HashGrid3::HashGrid3(const Vector3& origin, const Vector3& size, float spacing) throw())
  //?????
RESULT


CHECK(HashGrid3::HashGrid3(const HashGrid3& grid, bool deep = true) throw())
  //?????
RESULT


CHECK(HashGrid3::~HashGrid3() throw())
  //?????
RESULT


CHECK(HashGrid3::countNonEmptyBoxes() const  throw())
  //?????
RESULT


CHECK(HashGrid3::getSize() const  throw())
  //?????
RESULT


CHECK(HashGrid3::getOrigin() throw())
  //?????
RESULT


CHECK(HashGrid3::getOrigin() const  throw())
  //?????
RESULT


CHECK(HashGrid3::getUnit() throw())
  //?????
RESULT


CHECK(HashGrid3::getUnit() const  throw())
  //?????
RESULT


CHECK(HashGrid3::getSizeX() const  throw())
  //?????
RESULT


CHECK(HashGrid3::getSizeY() const  throw())
  //?????
RESULT


CHECK(HashGrid3::getSizeZ() const  throw())
  //?????
RESULT


CHECK(HashGrid3::getBox(Position x, Position y, Position z) throw())
  //?????
RESULT


CHECK(HashGrid3::getBox(Position x, Position y, Position z) const  throw())
  //?????
RESULT


CHECK(HashGrid3::getBox(const Vector3& vector) throw())
  //?????
RESULT


CHECK(HashGrid3::getBox(const Vector3 &vector) const  throw())
  //?????
RESULT


CHECK(HashGrid3::getIndices(const HashGridBox3<Item>& box, Position& x, Position& y, Position& z) const  throw())
  //?????
RESULT


CHECK(HashGrid3::insert(Position x, Position y, Position z, const Item& item) throw())
  //?????
RESULT


CHECK(HashGrid3::insert(const Vector3& vector, const Item& item) throw())
  //?????
RESULT


CHECK(HashGrid3::remove(Position x, Position y, Position z, const Item& item) throw())
  //?????
RESULT


CHECK(HashGrid3::remove(const Vector3& vector, const Item& item) throw())
  //?????
RESULT


CHECK(HashGrid3::host(Visitor<HashGrid3>& visitor) throw())
  //?????
RESULT


CHECK(HashGrid3::bool operator == (const HashGrid3& grid) const  throw())
  //?????
RESULT


CHECK(HashGrid3::bool operator != (const HashGrid3& grid) const  throw())
  //?????
RESULT


CHECK(HashGrid3::isEmpty() const  throw())
  //?????
RESULT


CHECK(HashGrid3::isValid() const  throw())
  //?????
RESULT


CHECK(HashGrid3::dump(std::ostream& s = std::cout, Size depth = 0) const  throw())
  //?????
RESULT


CHECK(HashGrid3::apply(UnaryProcessor<Item> &processor) throw())
  //?????
RESULT


CHECK(HashGrid3::apply(UnaryProcessor< HashGridBox3<Item> > &processor) throw())
  //?????
RESULT


// tests for class HashGrid3::BoxIteratorTraits_::

CHECK(HashGrid3::BoxIteratorTraits_::(BoxIteratorTraits_)() throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::BoxIteratorTraits_(const HashGrid3 &grid) throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true) throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits) throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::getContainer() const  throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::getPosition() throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::bool operator == (const BoxIteratorTraits_& traits) const  throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::isValid() const  throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::toBegin() throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::toEnd() throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::getData() throw())
  //?????
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::forward() throw())
  //?????
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
