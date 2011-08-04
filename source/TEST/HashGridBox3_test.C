// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>
#include "HashGrid3_test.h"

///////////////////////////

START_TEST(HashGridBox3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class HashGridBox3::

HashGridBox3<int>* hbox3 = 0;

CHECK(HashGridBox3() throw())
	hbox3 = new HashGridBox3<int>(0);
	TEST_NOT_EQUAL(hbox3, 0)
RESULT


CHECK(~HashGridBox3() throw())
	delete hbox3;
RESULT

CHECK(void clear() throw())
	HashGridBox3<int> hbox(0);
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	hbox.clear();
	TEST_EQUAL(hbox.getSize(), 0)
RESULT


CHECK(void destroy() throw())
	HashGridBox3<int> hbox(0);
	for (int i = 0; i < 5; ++i)
	{
		hbox.insert(i);
	}
	hbox.destroy();
	// TODO: did it really destroy?
RESULT


CHECK(Item* find(const Item &item) throw())
	HashGridBox3<int> hbox(0);
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
	HashGridBox3<int> hbox(0);
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
	HashGridBox3<int> hbox(0);
	int size = hbox.getSize();
	TEST_EQUAL(size, 0)
	int test_int = 5;
	hbox.insert(test_int);
	size = hbox.getSize();
	TEST_EQUAL(size, 1)
RESULT


CHECK(void insert(const Item& item) throw())
	HashGridBox3<int> hbox(0);
	hbox.insert(567);
	bool test = (*hbox.find(567) == 567);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool remove(const Item& item) throw())
	HashGridBox3<int> hbox(0);
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
	HashGridBox3<int> hbox(0);
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


CHECK(bool operator == (const HashGridBox3& box) const throw())
	HashGridBox3<int> hbox1(0);
	for (int i = 0; i < 5; i++)
	{
		hbox1.insert(i);
	}
	HashGridBox3<int> hbox2(0);
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
	HashGridBox3<int> hbox1(0);
	for (int i = 0; i < 5; i++)
	{
		hbox1.insert(i);
	}
	HashGridBox3<int> hbox2(0);
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
	HashGridBox3<int> hbox(0);
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
	HashGridBox3<int> hbox(0);
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
	HashGridBox3<int> hbox(0);
	for (int i = 0; i < 5; i++)
	{
		hbox.insert(i);
	}
	bool test = hbox.isValid();
	TEST_EQUAL(test, true)
RESULT


CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	HashGridBox3<int> hbox(0);
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
	TEST_FILE_REGEXP(tmp_filename.c_str(), BALL_TEST_DATA_PATH(hashgrid3_test_dump0.txt))
RESULT


CHECK(bool apply(UnaryProcessor<Item>& processor) throw())
	TestProcessor proc;
	HashGridBox3<int> hbox(0);
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

CHECK(const HashGridBox3& operator = (const HashGridBox3& box) throw(Exception::NotImplemented))
  // ???
RESULT

CHECK(void host(Visitor<HashGridBox3> &visitor) throw())
  // ???
RESULT

CHECK(void set(const HashGridBox3& box, bool /* deep */ = true) throw(Exception::NotImplemented))
  // ???
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
