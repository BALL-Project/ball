// $Id: HashGrid3_test.C,v 1.1 2001/04/27 15:20:05 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here

///////////////////////////

START_TEST(class_name, "$Id: HashGrid3_test.C,v 1.1 2001/04/27 15:20:05 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class HashGridBox3::

CHECK(HashGridBox3::HashGridBox3() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::HashGridBox3(const HashGridBox3& grid_box, bool deep = true) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::~HashGridBox3() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::clear() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::destroy() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::set(const HashGridBox3& box, bool /* deep */ = true) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::HashGridBox3& operator = (const HashGridBox3& box) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::get(HashGridBox3& box, bool deep = true) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::find(const Item &item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::find(const Item& item) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::getSize() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::insert(const Item& item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::remove(const Item& item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::removeAll(const Item& item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::host(Visitor<HashGridBox3> &visitor) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::bool operator == (const HashGridBox3& box) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::bool operator != (const HashGridBox3& box) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::has(const Item& item) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::isEmpty() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::isValid() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::dump(std::ostream& s = std::cout, Size depth = 0) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::apply(UnaryProcessor<Item>& processor) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::apply(UnaryProcessor< HashGridBox3<Item> >& processor) throw())
  //BAUSTELLE
RESULT


// tests for class HashGridBox3::BoxIteratorTraits_::

CHECK(HashGridBox3::BoxIteratorTraits_::(BoxIteratorTraits_)() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::BoxIteratorTraits_(const HashGridBox3& box) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits) throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::getContainer() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::getPosition() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::bool operator == (const BoxIteratorTraits_& traits) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::isValid() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::toBegin() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::toEnd() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::getData() throw())
  //BAUSTELLE
RESULT


CHECK(HashGridBox3::BoxIteratorTraits_::forward() throw())
  //BAUSTELLE
RESULT


// tests for class DataIteratorTraits_::

CHECK(DataIteratorTraits_::(DataIteratorTraits_)() throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::DataIteratorTraits_(const HashGridBox3& box) throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::DataIteratorTraits_(const DataIteratorTraits_& traits, bool /* deep */ = true) throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::DataIteratorTraits_& operator = (const DataIteratorTraits_ &traits) throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::getContainer() const  throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::getPosition() throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::bool operator == (const DataIteratorTraits_ &traits) const  throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::isValid() const  throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::toBegin() throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::toEnd() throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::getData() throw())
  //BAUSTELLE
RESULT


CHECK(DataIteratorTraits_::forward() throw())
  //BAUSTELLE
RESULT


// tests for class HashGrid3::

CHECK(HashGrid3::BALL_CREATE(HashGrid3))
  //BAUSTELLE
RESULT


CHECK(HashGrid3::countNonEmptyBoxes() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getSize() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getOrigin() throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getOrigin() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getUnit() throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getUnit() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getSizeX() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getSizeY() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getSizeZ() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getBox(Position x, Position y, Position z) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getBox(Position x, Position y, Position z) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getBox(const Vector3& vector) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getBox(const Vector3 &vector) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::getIndices(const HashGridBox3<Item>& box, Position& x, Position& y, Position& z) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::insert(Position x, Position y, Position z, const Item& item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::insert(const Vector3& vector, const Item& item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::remove(Position x, Position y, Position z, const Item& item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::remove(const Vector3& vector, const Item& item) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::host(Visitor<HashGrid3>& visitor) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::bool operator == (const HashGrid3& grid) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::bool operator != (const HashGrid3& grid) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::isEmpty() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::isValid() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::dump(std::ostream& s = std::cout, Size depth = 0) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::apply(UnaryProcessor<Item> &processor) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::apply(UnaryProcessor< HashGridBox3<Item> > &processor) throw())
  //BAUSTELLE
RESULT


// tests for class HashGrid3::BoxIteratorTraits_::

CHECK(HashGrid3::BoxIteratorTraits_::(BoxIteratorTraits_)() throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::BoxIteratorTraits_(const HashGrid3 &grid) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::BoxIteratorTraits_(const BoxIteratorTraits_& traits, bool /* deep */ = true) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::BoxIteratorTraits_& operator = (const BoxIteratorTraits_& traits) throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::getContainer() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::getPosition() throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::bool operator == (const BoxIteratorTraits_& traits) const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::isValid() const  throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::toBegin() throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::toEnd() throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::getData() throw())
  //BAUSTELLE
RESULT


CHECK(HashGrid3::BoxIteratorTraits_::forward() throw())
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
