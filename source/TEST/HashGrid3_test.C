// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HashGrid3_test.C,v 1.16 2003/06/19 19:57:02 oliver Exp $
//
// This line is for testing purposes. Remove me.
//
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>
#include "HashGrid3_test.h"

///////////////////////////

START_TEST(HashGrid, "$Id: HashGrid3_test.C,v 1.16 2003/06/19 19:57:02 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class HashGridBox3::

HashGridBox3<int>* hbox3 = 0;

CHECK(HashGrid3() throw())
	hbox3 = new HashGridBox3<int>;
	TEST_NOT_EQUAL(hbox3, 0)
RESULT


CHECK(~HashGridBox3() throw())
	delete hbox3;
RESULT

CHECK(HashGrid3(const HashGrid3& grid, bool deep = true) throw())
  // ?????
	// uses set(), which isn't implemented
RESULT


CHECK(void clear() throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	hbox.clear();
	TEST_EQUAL(hbox.getSize(), 0)
RESULT


CHECK(void destroy() throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	hbox.destroy();
	// TODO: did it really destroy?
RESULT


CHECK(Item* find(const Item &item) throw())
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


CHECK(const Item* find(const Item& item) const throw())
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


CHECK(Size getSize() const throw())
	HashGridBox3<int> hbox;
	int size = hbox.getSize();
	TEST_EQUAL(size, 0)
	int test_int = 5;
	hbox.insert(test_int);
	size = hbox.getSize();
	TEST_EQUAL(size, 1)
RESULT


CHECK(void insert(const Item& item) throw())
	HashGridBox3<int> hbox;
	hbox.insert(567);
	bool test = (*hbox.find(567) == 567);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool remove(const Item& item) throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	hbox.remove(3);
	int size = hbox.getSize();
	TEST_EQUAL(size, 4)
	TEST_EQUAL(hbox.find(3), 0)
RESULT


CHECK(bool removeAll(const Item& item) throw())
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


CHECK(void host(Visitor<HashGrid3>& visitor) throw())
  //?????
RESULT


CHECK(bool operator == (const HashGridBox3& box) const throw())
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


CHECK(bool operator != (const HashGridBox3& box) const throw())
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


CHECK(bool has(const Item& item) const throw())
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


CHECK(bool isEmpty() const throw())
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


CHECK(bool isValid() const throw())
	HashGridBox3<int> hbox;
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	bool test = hbox.isValid();
	TEST_EQUAL(test, true)
RESULT


CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
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
	TEST_FILE_REGEXP(tmp_filename.c_str(), "data/hashgrid3_test_dump0.txt")
RESULT


CHECK(bool apply(UnaryProcessor<Item>& processor) throw())
	TestProcessor proc;
	HashGridBox3<int> hbox;
	hbox.insert(5);
	hbox.apply(proc);
	int* result = hbox.find(6);
	TEST_NOT_EQUAL(result, 0)
	bool test = (*result == 6);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool apply(UnaryProcessor< HashGridBox3<Item> >& processor) throw())
	// ?????
RESULT

CHECK(HashGridBox3(const HashGridBox3& grid_box, bool deep = true) throw())
  // ??? Not implemented
RESULT

CHECK(HashGridBox3* getContainer() throw())
  // ???
RESULT

CHECK(HashGridBox3<Item>& getData() throw())
  // ???
RESULT

CHECK(HashGridBox3<Item>* getBox(Position x, Position y, Position z) throw())
  // ???
RESULT

CHECK(HashGridBox3<Item>* getBox(const Vector3& vector) throw())
  // ???
RESULT

CHECK(Item& getData() throw())
  // ???
RESULT



// tests for class HashGrid3::

HashGrid3<int>* hg_ptr;

CHECK(HashGridBox3() throw())
	hg_ptr = new HashGrid3<int>;
	TEST_NOT_EQUAL(hg_ptr, 0)
RESULT


CHECK(~HashGrid3() throw())
	delete hg_ptr;
RESULT


CHECK(HashGrid3(const Vector3& origin, Size dimension_x, Size dimension_y, Size dimension_z, float spacing_x, float spacing_y, float spacing_z) throw())
	Vector3 origin(1.0, 2.0, 3.0);
	Size dimension_x = 1;
	Size dimension_y = 4;
	Size dimension_z = 7;
	float spacing_x = 1.23;
	float spacing_y = 4.56;
	float spacing_z = 7.89;
	HashGrid3<int> hg(origin, dimension_x, dimension_y, dimension_z,
			spacing_x, spacing_y, spacing_z);
	TEST_EQUAL(hg.getOrigin(), origin)
	TEST_EQUAL(hg.getSizeX(), dimension_x)
	TEST_EQUAL(hg.getSizeY(), dimension_y)
	TEST_EQUAL(hg.getSizeZ(), dimension_z)
RESULT


CHECK(HashGrid3(const Vector3& origin, Size dimension_x, Size dimension_y, Size dimension_z, float spacing) throw())
	Vector3 origin(1.0, 2.0, 3.0);
	Size dimension_x = 1;
	Size dimension_y = 4;
	Size dimension_z = 7;
	float spacing = 3.14;
	HashGrid3<int> hg(origin, dimension_x, dimension_y, dimension_z, spacing);
	TEST_EQUAL(hg.getOrigin(), origin)
	TEST_EQUAL(hg.getSizeX(), dimension_x)
	TEST_EQUAL(hg.getSizeY(), dimension_y)
	TEST_EQUAL(hg.getSizeZ(), dimension_z)
RESULT


CHECK(HashGrid3(const Vector3& origin, const Vector3& size, float spacing) throw())
	Vector3 origin(1.0, 2.0, 3.0);
	Vector3 size(4.0, 5.0, 6.0);
	float spacing = 3.14;
	HashGrid3<int> hg(origin, size, spacing);
	TEST_EQUAL(hg.getOrigin(), origin)
	TEST_EQUAL(hg.getSizeX(), 2)
	TEST_EQUAL(hg.getSizeY(), 2)
	TEST_EQUAL(hg.getSizeZ(), 2)
RESULT


CHECK(HashGrid3* getContainer() throw())
  // ???
RESULT

CHECK(BALL_CREATE(HashGrid3))
  // ???
RESULT

// tests for class HashGridBox3::BoxIteratorTraits_::

HashGridBox3<int>::BoxIteratorTraits_* it_tr_ptr = 0;
CHECK(BoxIteratorTraits_() throw())
	it_tr_ptr = new HashGridBox3<int>::BoxIteratorTraits_;
	TEST_NOT_EQUAL(it_tr_ptr, 0)
RESULT

CHECK([EXTRA]~BoxIteratorTraits_() throw())
	delete it_tr_ptr;
RESULT


CHECK(BALL_CREATE_DEEP(BoxIteratorTraits_))
  // ???
RESULT

CHECK(BoxIteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true) throw())
  // ???
RESULT

CHECK(BoxIteratorTraits_(const HashGrid3 &grid) throw())
  // ???
RESULT

CHECK(BoxIteratorTraits_(const HashGridBox3& box) throw())
  // ???
RESULT

CHECK(DataItem_(const Item& item, DataItem_* next) throw())
  // ???
RESULT

CHECK(DataIteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(DataIteratorTraits_() throw())
  // ???
RESULT

CHECK(DataIteratorTraits_(const DataIteratorTraits_& traits, bool /* deep */ = true) throw())
  // ???
RESULT

CHECK(DataIteratorTraits_(const HashGridBox3& box) throw())
  // ???
RESULT

CHECK(BALL_CREATE_DEEP(DataIteratorTraits_))
  // ???
RESULT

CHECK(NeighbourBoxItem_(HashGridBox3* box, NeighbourBoxItem_* next) throw())
  // ???
RESULT

CHECK(Size countNonEmptyBoxes() const throw())
  // ???
RESULT

CHECK(Size getSizeX() const throw())
  // ???
RESULT

CHECK(Size getSizeY() const throw())
  // ???
RESULT

CHECK(Size getSizeZ() const throw())
  // ???
RESULT

CHECK(Vector3& getOrigin() throw())
  // ???
RESULT

CHECK(Vector3& getUnit() throw())
  // ???
RESULT

CHECK(bool apply(UnaryProcessor< HashGridBox3<Item> > &processor) throw())
  // ???
RESULT

CHECK(bool apply(UnaryProcessor<Item> &processor) throw())
  // ???
RESULT

CHECK(bool getIndices(const HashGridBox3<Item>& box, Position& x, Position& y, Position& z) const throw())
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

CHECK(bool operator != (const BoxIteratorTraits_& traits) const throw())
  // ???
RESULT

CHECK(bool operator != (const DataIteratorTraits_ &traits) const throw())
  // ???
RESULT

CHECK(bool operator != (const HashGrid3& grid) const throw())
  // ???
RESULT

CHECK(bool operator == (const BoxIteratorTraits_& traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const DataIteratorTraits_ &traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const HashGrid3& grid) const throw())
  // ???
RESULT

CHECK(bool remove(Position x, Position y, Position z, const Item& item) throw())
  // ???
RESULT

CHECK(bool remove(const Vector3& vector, const Item& item) throw())
  // ???
RESULT

CHECK(const BoxIteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits) throw())
  // ???
RESULT

CHECK(const DataIteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(const DataIteratorTraits_& operator = (const DataIteratorTraits_ &traits) throw())
  // ???
RESULT

CHECK(const HashGrid3& operator = (const HashGrid3& grid) throw())
  // ???
RESULT

CHECK(const HashGrid3* getContainer() const throw())
  // ???
RESULT

CHECK(const HashGridBox3& operator = (const HashGridBox3& box) throw(Exception::NotImplemented))
  // ???
RESULT

CHECK(const HashGridBox3* getContainer() const throw())
  // ???
RESULT

CHECK(const HashGridBox3<Item>& getData() const throw())
  // ???
RESULT

CHECK(const HashGridBox3<Item>* getBox(Position x, Position y, Position z) const throw())
  // ???
RESULT

CHECK(const HashGridBox3<Item>* getBox(const Vector3 &vector) const throw())
  // ???
RESULT

CHECK(const Item& getData() const throw())
  // ???
RESULT

CHECK(const Vector3& getOrigin() const throw())
  // ???
RESULT

CHECK(const Vector3& getUnit() const throw())
  // ???
RESULT

CHECK(void clear(Position x, Position y, Position z) throw())
  // ???
RESULT

CHECK(void clear(const Vector3 &vector) throw())
  // ???
RESULT

CHECK(void destroy(Position x, Position y, Position z) throw())
  // ???
RESULT

CHECK(void destroy(const Vector3& vector) throw())
  // ???
RESULT

CHECK(void forward() throw())
  // ???
RESULT

CHECK(void get(HashGrid3& grid, bool deep = true) const throw())
  // ???
RESULT

CHECK(void get(Vector3& origin, Vector3& unit, Size& dimension_x, Size& dimension_y, Size& dimension_z) const throw())
  // ???
RESULT

CHECK(void host(Visitor<HashGridBox3> &visitor) throw())
  // ???
RESULT

CHECK(void insert(Position x, Position y, Position z, const Item& item) throw())
  // ???
RESULT

CHECK(void insert(const Vector3& vector, const Item& item) throw())
  // ???
RESULT

CHECK(void invalidate() throw())
  // ???
RESULT

CHECK(void set(const HashGrid3& grid, bool deep = true) throw())
  // ???
RESULT

CHECK(void set(const HashGridBox3& box, bool /* deep */ = true) throw(Exception::NotImplemented))
  // ???
RESULT

CHECK(void set(const Vector3& origin, const Vector3& unit, Size dimension_x, Size dimension_y, Size dimension_z) throw())
  // ???
RESULT

CHECK(void set(const Vector3& origin, float unit, Size size) throw())
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
