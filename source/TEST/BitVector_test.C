// $Id: BitVector_test.C,v 1.2 1999/12/19 17:15:24 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>

///////////////////////////

START_TEST(BitVector, "$Id: BitVector_test.C,v 1.2 1999/12/19 17:15:24 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

BitVector* bv_ptr;
CHECK(BitVector::BitVector())
bv_ptr = new BitVector;
TEST_NOT_EQUAL(bv_ptr, 0)
RESULT

CHECK(BitVector::~BitVector())
delete bv_ptr;
RESULT

CHECK(BitVector::getSize() const)
BitVector bv;
TEST_EQUAL(bv.getSize(), BitVector::BlockSize)
RESULT

CHECK(BitVector::BitVector(Size))
BitVector bv1(1);
TEST_EQUAL(bv1.getSize(), 1)
BitVector bv2(2);
TEST_EQUAL(bv2.getSize(), 2)
BitVector bv9(9);
TEST_EQUAL(bv9.getSize(), 9)
BitVector bv129(129);
TEST_EQUAL(bv129.getSize(), 129)
BitVector bv20000(20000);
TEST_EQUAL(bv20000.getSize(), 20000)
TEST_EXCEPTION(Exception::InvalidRange,BitVector bv0((Size)0);)
RESULT

CHECK(BitVector::BitVector(const BitVector&, bool))
// BAUSTELLE
RESULT

CHECK(BitVector::BitVector(const char*))
// BAUSTELLE
RESULT

CHECK(BitVector::clear())
// BAUSTELLE
RESULT


CHECK(BitVector::set(const BitVector&, bool))
// BAUSTELLE
RESULT

CHECK(BitVector::set(const char*))
// BAUSTELLE
RESULT

CHECK(BitVector::BitVector& operator = (const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::BitVector& operator = (const char*))
// BAUSTELLE
RESULT

CHECK(BitVector::get(BitVector&, bool) const)
// BAUSTELLE
RESULT

CHECK(BitVector::swap(BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator () (Index,Index) const)
// BAUSTELLE
RESULT

CHECK(BitVector::countValue(bool) const)
// BAUSTELLE
RESULT

CHECK(BitVector::getMaxIndex() const)
// BAUSTELLE
RESULT

CHECK(BitVector::getBitSet())
// BAUSTELLE
RESULT

CHECK(BitVector::getBitSet() const)
// BAUSTELLE
RESULT

CHECK(BitVector::operator [] (Index))
// BAUSTELLE
RESULT

CHECK(BitVector::operator [] (Index) const)
// BAUSTELLE
RESULT

CHECK(BitVector::get(char*, Size, Index, Index) const)
// BAUSTELLE
RESULT

CHECK(BitVector::setBit(Index, bool))
// BAUSTELLE
RESULT

CHECK(BitVector::getBit(Index) const)
// BAUSTELLE
RESULT

CHECK(BitVector::getBit(Index))
// BAUSTELLE
RESULT

CHECK(BitVector::toggleBit(Index))
// BAUSTELLE
RESULT

CHECK(BitVector::fill(bool, Index, Index))
// BAUSTELLE
RESULT

CHECK(BitVector::toggle(Index, Index))
// BAUSTELLE
RESULT

CHECK(BitVector::setUnsignedChar(unsigned char))
// BAUSTELLE
RESULT

CHECK(BitVector::getUnsignedChar() const)
// BAUSTELLE
RESULT

CHECK(BitVector::setUnsignedShort(unsigned short))
// BAUSTELLE
RESULT
 
CHECK(BitVector::setUnsignedInt(unsigned int bit_pattern))
// BAUSTELLE
RESULT

CHECK(BitVector::getUnsignedInt() const)
// BAUSTELLE
RESULT

CHECK(BitVector::setUnsignedLong(unsigned long))
// BAUSTELLE
RESULT

CHECK(BitVector::getUnsignedLong() const)
// BAUSTELLE
RESULT

CHECK(BitVector::or(const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::xor(const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::and(const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator | (const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator |= (const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator & (const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator &= (const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator ^ (const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator ^= (const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::operator ~ ())
// BAUSTELLE
RESULT

CHECK(BitVector::operator == (const BitVector&) const)
// BAUSTELLE
RESULT

CHECK(BitVector::operator != (const BitVector&) const)
// BAUSTELLE
RESULT

CHECK(BitVector::isAnyBit(bool value, Index first = 0, Index last = -1) const)
// BAUSTELLE
RESULT

CHECK(BitVector::isEveryBit(bool, Index, Index) const)
// BAUSTELLE
RESULT
 
CHECK(BitVector::isValid() const)
// BAUSTELLE
RESULT

CHECK(BitVector::operator >> (istream&, BitVector&))
// BAUSTELLE
RESULT
 
CHECK(BitVector::operator << (ostream&, const BitVector&))
// BAUSTELLE
RESULT

CHECK(BitVector::read(istream&))
// BAUSTELLE
RESULT

CHECK(BitVector::write(ostream&) const)
// BAUSTELLE
RESULT

CHECK(BitVector::write(PersistenceManager&) const)
// BAUSTELLE
RESULT

CHECK(BitVector::read(PersistenceManager&))
// BAUSTELLE
RESULT

CHECK(BitVector::setSize())
BitVector b(8);
b.setBit(3);
TEST_EQUAL(b.getSize(), 8)
b.setSize(24, true);
TEST_EQUAL(b.getSize(), 24)
b.setSize(24, false);
TEST_EQUAL(b.getSize(), 24)
RESULT

///////////////////////////////////////////////////
END_TEST
