// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/SYSTEM/sysinfo.h>
#include "HashGrid3_test.h"

///////////////////////////

START_TEST(HashGrid3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class HashGrid3::

HashGrid3<int>* hg_ptr;

CHECK(HashGrid3() throw())
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


CHECK(BALL_CREATE(HashGrid3))
  // ???
RESULT

CHECK(void clear(Position x, Position y, Position z) throw())
  // ???
RESULT

CHECK(void clear(const Vector3 &vector) throw())
  // ???
RESULT

CHECK(void destroy() throw())
  // ???
RESULT

CHECK(void destroy(Position x, Position y, Position z) throw())
  // ???
RESULT

CHECK(void destroy(const Vector3& vector) throw())
  // ???
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
  // ???
RESULT

CHECK(void get(HashGrid3& grid, bool deep = true) const throw())
  // ???
RESULT

CHECK(void get(Vector3& origin, Vector3& unit, Size& dimension_x, Size& dimension_y, Size& dimension_z) const throw())
  // ???
RESULT

CHECK(void host(Visitor<HashGrid3>& visitor) throw())
  // ???
RESULT

CHECK(void insert(Position x, Position y, Position z, const Item& item) throw())
  // ???
RESULT

CHECK(void insert(const Vector3& vector, const Item& item) throw())
  // ???
RESULT

CHECK(void set(const HashGrid3& grid, bool deep = true) throw())
  // ???
RESULT

CHECK(void set(const Vector3& origin, const Vector3& unit, Size dimension_x, Size dimension_y, Size dimension_z) throw())
  // ???
RESULT

CHECK(bool getIndices(const HashGridBox3<Item>& box, Position& x, Position& y, Position& z) const throw())
  // ???
RESULT

CHECK(bool isEmpty() const throw())
  // ???
RESULT

CHECK(bool isValid() const throw())
  // ???
RESULT

CHECK(bool operator != (const HashGrid3& grid) const throw())
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

CHECK(const HashGrid3& operator = (const HashGrid3& grid) throw())
  // ???
RESULT

CHECK(const HashGridBox3<Item>* getBox(Position x, Position y, Position z) const throw())
  // ???
RESULT

CHECK(const HashGridBox3<Item>* getBox(const Vector3 &vector) const throw())
  // ???
RESULT

CHECK(const Vector3& getOrigin() const throw())
  // ???
RESULT

CHECK(const Vector3& getUnit() const throw())
  // ???
RESULT

CHECK(void clear() throw())
  // ???
RESULT

CHECK(void set(const Vector3& origin, float unit, Size size) throw())
  // ???
RESULT

CHECK(HashGrid3(const HashGrid3& grid, bool deep = true) throw())
  // ???
RESULT

CHECK(HashGridBox3<Item>* getBox(Position x, Position y, Position z) throw())
  // ???
RESULT

CHECK(HashGridBox3<Item>* getBox(const Vector3& vector) throw())
  // ???
RESULT

CHECK(Size countNonEmptyBoxes() const throw())
  // ???
RESULT

CHECK(Size getSize() const throw())
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

CHECK(calculateMinSpacing)
	LongIndex memory = 10000000;
	Vector3 size = Vector3(1000., 1000., 1000.);

	float spacing = HashGrid3<float>::calculateMinSpacing(memory, size);
 	STATUS("spacing: " << spacing)	
	HashGrid3<float> hg(Vector3(0.0), size, spacing);
	TEST_EQUAL((spacing < 35.), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
