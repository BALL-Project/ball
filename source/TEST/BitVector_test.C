// $Id: BitVector_test.C,v 1.5 2000/07/18 10:47:19 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>

///////////////////////////

START_TEST(BitVector, "$Id: BitVector_test.C,v 1.5 2000/07/18 10:47:19 amoll Exp $")

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

BitVector bv9(9);

CHECK(BitVector::BitVector(const BitVector&, bool))
	bv9.setBit(0, true);
	bv9.setBit(8, true);
	BitVector bv9_2 = BitVector(bv9);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getBit(0), true)
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(8), true)
RESULT

CHECK(BitVector::BitVector(const char*))
	char c[9] = {1, 0, 0, 1, 0, 1, 0, 1, 1};
	BitVector bv9_2 = BitVector(c);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getBit(0), true)
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(8), true)
RESULT

CHECK(BitVector::clear())
	bv9.setBit(0, true);
	bv9.setBit(8, true);
	bv9.clear();
	TEST_EQUAL(bv9.getBit(0), false)
	TEST_EQUAL(bv9.getBit(8), false)
RESULT

CHECK(BitVector::set(const BitVector&, bool))
	BitVector bv9_2;
	bv9_2.set(bv9);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getBit(0), true)
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(8), true)
RESULT

CHECK(BitVector::set(const char*))
	char c[9] = {1, 0, 0, 1, 0, 1, 0, 1, 1};
	BitVector bv9_2;
	bv9_2.set(c);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getBit(0), true)
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(8), true)
RESULT

CHECK(BitVector::BitVector& operator = (const BitVector&))
	BitVector bv9_2;
	bv9_2 = bv9;
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getBit(0), true)
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(8), true)
RESULT

CHECK(BitVector::BitVector& operator = (const char*))
	char c[9] = {1, 0, 0, 1, 0, 1, 0, 1, 1};
	BitVector bv9_2;
	bv9_2 = c;
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getBit(0), true)
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(8), true)
RESULT

CHECK(BitVector::swap(BitVector&))
	BitVector bv10(10);
	bv10.setBit(1, true);
	bv10.setBit(9, true);
	bv10.swap(bv9);
	TEST_EQUAL(bv9.getSize(), 10)
	TEST_EQUAL(bv9.getBit(0), false)
	TEST_EQUAL(bv9.getBit(1), true)
	TEST_EQUAL(bv9.getBit(8), false)
	TEST_EQUAL(bv9.getBit(9), true)

	TEST_EQUAL(bv10.getBit(0), true)
	TEST_EQUAL(bv10.getBit(1), false)
	TEST_EQUAL(bv10.getBit(8), true)
	TEST_EQUAL(bv10.getSize(), 9)

	bv10.swap(bv9);
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
	//bv9[0] = true;
RESULT

CHECK(BitVector::operator [] (Index) const)
	//TEST_EQUAL(bv9[0], true)
RESULT

CHECK(BitVector::get(char*, Size, Index, Index) const)
	// BAUSTELLE
RESULT

CHECK(BitVector::setBit/getBit(Index, bool))
	bv9.setBit(0, false);
	TEST_EQUAL(bv9.getBit(0), false)
	bv9.setBit(0, true);
	TEST_EQUAL(bv9.getBit(0), true)
	bv9.setBit(0, false);
	TEST_EQUAL(bv9.getBit(0), false)
	TEST_EQUAL(bv9.getBit(99), false)
	TEST_EQUAL(bv9.getSize(), 100)
RESULT

CHECK(BitVector::getBit(Index))
	/*Bit b = true;
	TEST_EQUAL(bv9.getBit(0), b)*/
RESULT

CHECK(BitVector::toggleBit(Index))
	bv9.toggleBit(0);
	TEST_EQUAL(bv9.getBit(0), true)
RESULT

CHECK(BitVector::fill(bool, Index, Index))
	bv9.fill(true, 2, 3);
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(2), true)
	TEST_EQUAL(bv9.getBit(3), true)
	TEST_EQUAL(bv9.getBit(4), false)
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
	TEST_EQUAL(bv9.isValid(), true)
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
