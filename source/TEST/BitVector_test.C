// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: BitVector_test.C,v 1.30 2003/06/11 14:30:10 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

///////////////////////////

START_TEST(BitVector, "$Id: BitVector_test.C,v 1.30 2003/06/11 14:30:10 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

BitVector* bv_ptr;
CHECK(BitVector() throw())
	bv_ptr = new BitVector;
	TEST_NOT_EQUAL(bv_ptr, 0)
RESULT

CHECK(~BitVector() throw())
	delete bv_ptr;
RESULT


CHECK(void setSize(Size size, bool keep = true) throw(Exception::OutOfMemory))
	BitVector bv;
	bv.setSize(12);
	TEST_EQUAL(bv.getSize(), 12)
RESULT

CHECK(Size getSize() const throw())
	BitVector bv;
	TEST_EQUAL(bv.getSize(), 0)
RESULT

CHECK(BALL_CREATE(BitVector))
	BitVector bv;
	bv.setSize(12);
	BitVector* v_ptr = (BitVector*)bv.create(false, true);
	TEST_EQUAL(v_ptr->getSize(), 0)
	delete v_ptr;
	v_ptr = (BitVector*)bv.create();
	TEST_EQUAL(v_ptr->getSize(), 12)
	delete v_ptr;
RESULT

CHECK(BitVector(Size size) throw(Exception::OutOfMemory))
	BitVector bv0((Size)0);
	TEST_EQUAL(bv0.getSize(), 0)

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
RESULT

BitVector bv9(9);

CHECK(BitVector(const BitVector& bit_vector) throw(Exception::OutOfMemory))
	bv9.setBit(0, true);
	bv9.setBit(8, true);
	BitVector bv9_2 = BitVector(bv9);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(2), false)
	TEST_EQUAL(bv9_2.getBit(3), false)
	TEST_EQUAL(bv9_2.getBit(4), false)
	TEST_EQUAL(bv9_2.getBit(5), false)
	TEST_EQUAL(bv9_2.getBit(6), false)
	TEST_EQUAL(bv9_2.getBit(7), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(BitVector(const char* bit_string) throw(Exception::OutOfMemory))
	const char* c = "100101011";
	BitVector bv9_2(c);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), true)
	TEST_EQUAL(bv9_2.getBit(2), false)
	TEST_EQUAL(bv9_2.getBit(3), true)
	TEST_EQUAL(bv9_2.getBit(4), false)
	TEST_EQUAL(bv9_2.getBit(5), true)
	TEST_EQUAL(bv9_2.getBit(6), false)
	TEST_EQUAL(bv9_2.getBit(7), false)
	TEST_EQUAL(bv9_2.getBit(8), true)

	BitVector empty("");
	TEST_EQUAL(empty.getSize(), 0)

	const char* null_ptr = 0;
	BitVector null(null_ptr);
	TEST_EQUAL(null.getSize(), 0)
RESULT

CHECK(void clear() throw())
	bv9.setBit(0, true);
	bv9.setBit(8, true);
	TEST_EQUAL(bv9.getSize(), 9)
	bv9.clear();
	TEST_EQUAL(bv9.getSize(), 0)
RESULT


CHECK(void set(const BitVector& bit_vector) throw(Exception::OutOfMemory))
	BitVector bv9_1("100000001");
	BitVector bv9_2;
	bv9_2.set(bv9_1);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(void set(const char* bit_string) throw(Exception::OutOfMemory))
	const char* c = "100101011";
	BitVector bv9_2;
	bv9_2.set(c);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), true)
	TEST_EQUAL(bv9_2.getBit(2), false)
	TEST_EQUAL(bv9_2.getBit(3), true)
	TEST_EQUAL(bv9_2.getBit(4), false)
	TEST_EQUAL(bv9_2.getBit(5), true)
	TEST_EQUAL(bv9_2.getBit(6), false)
	TEST_EQUAL(bv9_2.getBit(7), false)
	TEST_EQUAL(bv9_2.getBit(8), true)

	BitVector bv9_3;
	bv9_3.set("");
	TEST_EQUAL(bv9_3.getSize(), 0)
RESULT

CHECK(const BitVector& operator = (const BitVector& bit_vector) throw(Exception::OutOfMemory))
	BitVector bv11("100101011"); 
	BitVector bv11_2;
	bv11_2 = bv11;
	TEST_EQUAL(bv11_2.getSize(), 9)
	TEST_EQUAL(bv11_2.getBit(0), true)
	TEST_EQUAL(bv11_2.getBit(1), true)
	TEST_EQUAL(bv11_2.getBit(2), false)
	TEST_EQUAL(bv11_2.getBit(3), true)
	TEST_EQUAL(bv11_2.getBit(4), false)
	TEST_EQUAL(bv11_2.getBit(5), true)
	TEST_EQUAL(bv11_2.getBit(6), false)
	TEST_EQUAL(bv11_2.getBit(7), false)
	TEST_EQUAL(bv11_2.getBit(8), true)
	BitVector bv12("011010100"); 
	BitVector bv12_2;
	bv12_2 = bv12;
	TEST_EQUAL(bv12_2.getSize(), 9)
	TEST_EQUAL(bv12_2.getBit(0), false)
	TEST_EQUAL(bv12_2.getBit(1), false)
	TEST_EQUAL(bv12_2.getBit(2), true)
	TEST_EQUAL(bv12_2.getBit(3), false)
	TEST_EQUAL(bv12_2.getBit(4), true)
	TEST_EQUAL(bv12_2.getBit(5), false)
	TEST_EQUAL(bv12_2.getBit(6), true)
	TEST_EQUAL(bv12_2.getBit(7), true)
	TEST_EQUAL(bv12_2.getBit(8), false)
RESULT

CHECK(const BitVector& operator = (const char *bit_string) throw(Exception::OutOfMemory))
	const char* c = "100101011";
	BitVector bv9_2;
	bv9_2 = c;
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), true)
	TEST_EQUAL(bv9_2.getBit(2), false)
	TEST_EQUAL(bv9_2.getBit(3), true)
	TEST_EQUAL(bv9_2.getBit(4), false)
	TEST_EQUAL(bv9_2.getBit(5), true)
	TEST_EQUAL(bv9_2.getBit(6), false)
	TEST_EQUAL(bv9_2.getBit(7), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(void get(BitVector& bitvector) const throw(Exception::OutOfMemory))
	const char* c = "100101011";
	BitVector bv9_2;
	bv9_2 = c;
	BitVector bv;
	bv9_2.get(bv);
	TEST_EQUAL(bv == bv9_2, true)
RESULT

BitVector bv10(10);
bv10.setBit(1, true);
bv10.setBit(2, true);
bv10.setBit(4, true);
bv10.setBit(9, true);

CHECK(BitVector operator () (Index first, Index last) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	BitVector bv = bv10(1, 4);
	TEST_EQUAL(bv.getBit(0), true)
	TEST_EQUAL(bv.getBit(1), true)
	TEST_EQUAL(bv.getBit(2), false)
	TEST_EQUAL(bv.getBit(3), true)
	TEST_EXCEPTION(Exception::IndexUnderflow, bv10(0, -1111111111))
	TEST_EXCEPTION(Exception::IndexOverflow, bv10(0, 1111111111))
RESULT

CHECK(Size countValue(bool value) const throw())
	TEST_EQUAL(bv10.countValue(true), 4)
	TEST_EQUAL(bv10.countValue(false), 6)
	BitVector bv;
	TEST_EQUAL(bv.countValue(true), 0)
	TEST_EQUAL(bv.countValue(false), 0)
	bv.setBit(5);
	TEST_EQUAL(bv.getSize(), 6)
	TEST_EQUAL(bv.countValue(true), 1)
	TEST_EQUAL(bv.countValue(false), 5)
RESULT

CHECK(VectorType& getBitSet() throw())
	BitVector bv(10);
	BitVector::VectorType& bt = bv.getBitSet();
	bt[0] = 1;
	TEST_EQUAL(bv[0], true)
	TEST_EQUAL(bv[1], false)
	TEST_EQUAL(bv[2], false)
	bv.setBit(1);
	TEST_EQUAL(bt[0], 3)
RESULT

CHECK(const VectorType& getBitSet() const throw())
	const BitVector bv10_2(bv10);
	TEST_EQUAL(bv10_2.getBit(0), false)
	TEST_EQUAL(bv10_2.getBit(1), true)
	const BitVector::VectorType& bt = bv10_2.getBitSet();
	TEST_EQUAL(bt[0], 22)
	TEST_EQUAL(bt[1], 2)
RESULT

CHECK(Bit operator []	(Index index) throw(Exception::OutOfMemory))
	for (Index i = 0; i < 33; i++)
	{
		BitVector bv(33);
		bv[i] = true;
		for (Index j = 0; j < 33; j++)
		{
			TEST_EQUAL(bv[j], (i == j))
		}
	}
	BitVector bv3(3);
	bv3.setBit(1, true);
	TEST_EQUAL(bv3[0], false)
	TEST_EQUAL(bv3[1], true)
	TEST_EQUAL(bv3[3], false)
	TEST_EQUAL(bv3.getSize(), 4)
	bv3[-1111] = 1;
	TEST_EQUAL(bv3.getSize(), 1117)
	// Out of memory not tested
RESULT

CHECK(bool operator [] (Index index) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	const BitVector bv10_2(bv10);
	TEST_EQUAL(bv10_2[0], false)
	TEST_EQUAL(bv10_2[1], true)
	TEST_EXCEPTION(Exception::IndexOverflow, bv10_2[1111])
	TEST_EXCEPTION(Exception::IndexUnderflow, bv10_2[-1111])
	TEST_EQUAL(bv10_2.getSize(), 10)
RESULT

CHECK(void setBit(Index index, bool value = true) throw(Exception::IndexUnderflow, Exception::OutOfMemory))
	for (Index i = 0; i < 33; i++)
	{
		BitVector bv(33);
		bv.setBit(i);
		for (Index j = 0; j < 33; j++)
		{
			TEST_EQUAL(bv.getBit(j), (i == j))
		}
	}
	bv9.setBit(0, false);
	TEST_EQUAL(bv9.getBit(0), false)
	bv9.setBit(0, true);
	TEST_EQUAL(bv9.getBit(0), true)
	bv9.setBit(0, false);
	TEST_EQUAL(bv9.getBit(0), false)
	TEST_EQUAL(bv9.getBit(99), false)
	TEST_EQUAL(bv9.getSize(), 100)

	const BitVector const_bv(8);
	TEST_EXCEPTION(Exception::IndexOverflow, const_bv.getBit(11))
	TEST_EQUAL(const_bv.getSize(), 8)

	BitVector bv;
	bv.setBit(0, true);
	TEST_EQUAL(bv.getBit(0), true)
RESULT

CHECK(bool getBit(Index index) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
		BitVector bv(2);
		bv.setBit(1);
		const BitVector& b(bv);
		TEST_EQUAL(b.getBit(0), false)
		TEST_EQUAL(b.getBit(1), true)
		TEST_EXCEPTION(Exception::IndexOverflow, b.getBit(11111111))
		TEST_EXCEPTION(Exception::IndexUnderflow, b.getBit(-111111111))
RESULT

CHECK(bool getBit(Index index) throw(Exception::IndexUnderflow, Exception::OutOfMemory))
RESULT

CHECK(void toggleBit(Index index) throw(Exception::IndexUnderflow, Exception::OutOfMemory))
	TEST_EQUAL(bv9.getBit(0), false)
	bv9.toggleBit(0);
	TEST_EQUAL(bv9.getBit(0), true)
RESULT

CHECK(void fill(bool value = true, Index first = 0, Index last = -1) throw(Exception::IndexUnderflow, Exception::OutOfMemory))
	bv9.fill(true, 2, 3);
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(2), true)
	TEST_EQUAL(bv9.getBit(3), true)
	TEST_EQUAL(bv9.getBit(4), false)
RESULT

CHECK(void toggle(Index first = 0, Index last = -1) throw(Exception::IndexUnderflow, Exception::OutOfMemory))
	BitVector bv4(4);
	bv4.setBit(1, true);
	bv4.toggle(1, 2);
	TEST_EQUAL(bv4.getBit(0), false)
	TEST_EQUAL(bv4.getBit(1), false)
	TEST_EQUAL(bv4.getBit(2), true)
	TEST_EQUAL(bv4.getBit(3), false)
RESULT

CHECK(void setUnsignedChar(unsigned char bit_pattern) throw())
	BitVector bv8(8);
	bv8.setUnsignedChar((unsigned char)0xaa);
	TEST_EQUAL(bv8.getBit(0), false)
	TEST_EQUAL(bv8.getBit(1), true)
	TEST_EQUAL(bv8.getBit(2), false)
	TEST_EQUAL(bv8.getBit(3), true)
	TEST_EQUAL(bv8.getBit(4), false)
	TEST_EQUAL(bv8.getBit(5), true)
	TEST_EQUAL(bv8.getBit(6), false)
	TEST_EQUAL(bv8.getBit(7), true)
	bv8.setUnsignedChar((char)0);
	TEST_EQUAL(bv8.getBit(0), false)
	TEST_EQUAL(bv8.getBit(1), false)
	TEST_EQUAL(bv8.getBit(2), false)
	TEST_EQUAL(bv8.getBit(3), false)
	TEST_EQUAL(bv8.getBit(4), false)
	TEST_EQUAL(bv8.getBit(5), false)
	TEST_EQUAL(bv8.getBit(6), false)
	TEST_EQUAL(bv8.getBit(7), false)
	bv8.setUnsignedChar((char)2);
	TEST_EQUAL(bv8.getBit(0), false)
	TEST_EQUAL(bv8.getBit(1), true)
	TEST_EQUAL(bv8.getBit(2), false)
	TEST_EQUAL(bv8.getBit(3), false)
	TEST_EQUAL(bv8.getBit(4), false)
	TEST_EQUAL(bv8.getBit(5), false)
	TEST_EQUAL(bv8.getBit(6), false)
	TEST_EQUAL(bv8.getBit(7), false)
RESULT

CHECK(unsigned char getUnsignedChar() const throw())
	BitVector bv8(8);
	bv8.setUnsignedChar((unsigned char)0xaa);
	TEST_EQUAL((int)bv8.getUnsignedChar(), (int)0xaa)
	bv8.setUnsignedChar((char)0);
	TEST_EQUAL((int)bv8.getUnsignedChar(), (int)0x0)
	bv8.setUnsignedChar((char)2);
	TEST_EQUAL((int)bv8.getUnsignedChar(), (int)0x2)
RESULT

CHECK(void setUnsignedShort(unsigned short bit_pattern) throw())
	BitVector bv8(8);
	bv8.setUnsignedShort(1);
	TEST_EQUAL(bv8.getBit(0), true)
	TEST_EQUAL(bv8.getBit(1), false)
	TEST_EQUAL(bv8.getBit(2), false)
	TEST_EQUAL(bv8.getBit(3), false)
	TEST_EQUAL(bv8.getBit(4), false)
	TEST_EQUAL(bv8.getBit(5), false)
	TEST_EQUAL(bv8.getBit(6), false)
	TEST_EQUAL(bv8.getBit(7), false)
RESULT

CHECK(unsigned short getUnsignedShort() const throw())
	BitVector bv8(8);
	bv8.setUnsignedShort(1);
	TEST_EQUAL(bv8.getUnsignedShort(), 1)
RESULT
 
CHECK(void setUnsignedInt(unsigned int bit_pattern) throw())
	BitVector bv8(8);
	bv8.setUnsignedInt(1);
	TEST_EQUAL(bv8.getBit(0), true)
	TEST_EQUAL(bv8.getBit(1), false)
RESULT
	
CHECK(unsigned int getUnsignedInt() const throw())
	BitVector bv8(8);
	bv8.setUnsignedInt(1);
	TEST_EQUAL(bv8.getUnsignedInt(), 1)
RESULT

CHECK(void setUnsignedLong(unsigned long bit_pattern) throw())
	BitVector bv8(8);
	bv8.setUnsignedLong(1);
	TEST_EQUAL(bv8.getBit(0), true)
	TEST_EQUAL(bv8.getBit(1), false)
RESULT

CHECK(unsigned long getUnsignedLong() const throw())
	BitVector bv8(8);
	bv8.setUnsignedLong(1);
	TEST_EQUAL(bv8.getUnsignedLong(), 1)
RESULT

BitVector b4(4);	// **_* b4
BitVector b3(3);	// *__  b3
b4.setBit(0);
b4.setBit(1);
b4.setBit(3);
b3.setBit(0);
BitVector erg;

CHECK(void bitwiseOr(const BitVector& bit_vector) throw(Exception::OutOfMemory))
	erg = b4;
	erg.bitwiseOr(b3);
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(void bitwiseXor(const BitVector& bit_vector) throw(Exception::OutOfMemory))
	erg = b4;
	erg.bitwiseXor(b3);
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(void bitwiseAnd(const BitVector& bit_vector) throw(Exception::OutOfMemory))
	erg = b4;
	erg.bitwiseAnd(b3);
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), false)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), false)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector operator | (const BitVector& bit_vector) throw(Exception::OutOfMemory))
	BitVector b11("100");
	BitVector b12("100000");
	STATUS("b11 = " << b11)
	STATUS("b12 = " << b12)
	erg = b11 | b12;
	STATUS("erg = " << erg)
	TEST_EQUAL(erg.getSize(), 6)
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), false)
	TEST_EQUAL(erg.getBit(2), true)
	TEST_EQUAL(erg.getBit(3), false)
	TEST_EQUAL(erg.getBit(4), false)
	TEST_EQUAL(erg.getBit(5), true)
	BitVector b13("100000");
	BitVector b14("100");
	STATUS("b13 = " << b13)
	STATUS("b14 = " << b14)
	erg = b13 | b14;
	STATUS("erg = " << erg)
	TEST_EQUAL(erg.getSize(), 6)
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), false)
	TEST_EQUAL(erg.getBit(2), true)
	TEST_EQUAL(erg.getBit(3), false)
	TEST_EQUAL(erg.getBit(4), false)
	TEST_EQUAL(erg.getBit(5), true)
RESULT

CHECK(BitVector& operator |= (const BitVector& bit_vector) throw(Exception::OutOfMemory))
	BitVector b11("1010101010");
	BitVector b12("10000");
	b11 |= b12;
	TEST_EQUAL(b11.getSize(), 10)
	TEST_EQUAL(b11.getBit(0), false)
	TEST_EQUAL(b11.getBit(1), true)
	TEST_EQUAL(b11.getBit(2), false)
	TEST_EQUAL(b11.getBit(3), true)
	TEST_EQUAL(b11.getBit(4), true)
	TEST_EQUAL(b11.getBit(5), true)
	TEST_EQUAL(b11.getBit(6), false)
	TEST_EQUAL(b11.getBit(7), true)
	TEST_EQUAL(b11.getBit(8), false)
	TEST_EQUAL(b11.getBit(9), true)
	STATUS("b12 = " << b12)
	b12.setSize(10);
	STATUS("b12 = " << b12)

	BitVector b13("10000");
	BitVector b14("1010101010");
	STATUS("b13 = " << b13)
	STATUS("b14 = " << b14)
	b13 |= b14;
	STATUS("b13 |= b14 = " << b13)
	TEST_EQUAL(b13.getSize(), 10)
	TEST_EQUAL(b13.getBit(0), false)
	TEST_EQUAL(b13.getBit(1), true)
	TEST_EQUAL(b13.getBit(2), false)
	TEST_EQUAL(b13.getBit(3), true)
	TEST_EQUAL(b13.getBit(4), true)
	TEST_EQUAL(b13.getBit(5), true)
	TEST_EQUAL(b13.getBit(6), false)
	TEST_EQUAL(b13.getBit(7), true)
	TEST_EQUAL(b13.getBit(8), false)
	TEST_EQUAL(b13.getBit(9), true)
RESULT

CHECK(BitVector operator & (const BitVector& bit_vector) throw(Exception::OutOfMemory))
	erg = b4 & b3;
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), false)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), false)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector& operator &= (const BitVector& bit_vector) throw(Exception::OutOfMemory))
	BitVector b11("001101010001110101");
	BitVector b12(         "111100111");
	BitVector b13("000000000001100101");

	b11 &= b12;
	
	TEST_EQUAL(b11.getSize(),  b13.getSize())
	TEST_EQUAL(b11, b13)


	BitVector b14(         "111100111");
	BitVector b15("001101010001110101");
	BitVector b16(         "001100101");

	b14 &= b15;
	TEST_EQUAL(b14.getSize(), b16.getSize())
	TEST_EQUAL(b14, b16)
RESULT

CHECK(BitVector operator ^ (const BitVector& bit_vector) throw(Exception::OutOfMemory))
	erg = b4 ^ b3;
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector& operator ^= (const BitVector& bit_vector) throw(Exception::OutOfMemory))
	erg = b4;
	erg ^= b3;
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector operator ~ () throw(Exception::OutOfMemory))
	BitVector bv3(3);
	bv3.setBit(1, true);
	BitVector bv3_2 = ~ bv3;
	TEST_EQUAL(bv3_2.getBit(0), true)
	TEST_EQUAL(bv3_2.getBit(1), false)
	TEST_EQUAL(bv3_2.getBit(2), true)
	TEST_EQUAL(bv3_2.getSize(), 3)
RESULT

CHECK(bool operator == (const BitVector& bit_vector) const throw())
	BitVector bv3(3);
	BitVector bv3_2(3);
	bv3.setBit(0, true);
	TEST_EQUAL(bv3 == bv3_2, false)
	bv3_2.setBit(0, true);
	TEST_EQUAL(bv3 == bv3_2, true)
	bv3.setBit(2, true);
	TEST_EQUAL(bv3 == bv3_2, false)
	bv3_2.setBit(2, true);
	TEST_EQUAL(bv3 == bv3_2, true)
RESULT

CHECK(bool operator != (const BitVector& bit_vector) const throw())
	BitVector bv3(3);
	BitVector bv3_2(3);
	bv3.setBit(0, true);
	TEST_EQUAL(bv3 != bv3_2, true)
	bv3_2.setBit(0, true);
	TEST_EQUAL(bv3 != bv3_2, false)
	bv3.setBit(2, true);
	TEST_EQUAL(bv3 != bv3_2, true)
	bv3_2.setBit(2, true);
	TEST_EQUAL(bv3 != bv3_2, false)
RESULT

CHECK(bool isAnyBit(bool value, Index first = 0, Index last = -1) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	BitVector bv3(3);
	TEST_EQUAL(bv3.isAnyBit(false), true)
	TEST_EQUAL(bv3.isAnyBit(true), false)
	bv3.setBit(0, true);
	TEST_EQUAL(bv3.isAnyBit(true, 1, -1), false)
	TEST_EQUAL(bv3.isAnyBit(true), true)
	TEST_EXCEPTION(Exception::IndexOverflow, bv3.isAnyBit(1, 11111111))
	TEST_EXCEPTION(Exception::IndexUnderflow, bv3.isAnyBit(1, -111111111))
RESULT

CHECK(bool isEveryBit(bool value, Index first = 0, Index last = -1) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	BitVector bv3(3);
	TEST_EQUAL(bv3.isEveryBit(false), true)
	bv3.setBit(0, true);
	TEST_EQUAL(bv3.isEveryBit(false), false)
	TEST_EQUAL(bv3.isEveryBit(false, 1, 2), true)
	TEST_EQUAL(bv3.isEveryBit(false, 0, 1), false)
	bv3.setBit(1, true);
	bv3.setBit(2, true);
	TEST_EQUAL(bv3.isEveryBit(true), true)
	TEST_EXCEPTION(Exception::IndexOverflow, bv3.isEveryBit(1, 111111111))
	TEST_EXCEPTION(Exception::IndexUnderflow, bv3.isEveryBit(1, -111111111))
RESULT
 
CHECK(bool isValid() const throw())
	TEST_EQUAL(bv9.isValid(), true)
RESULT

using std::ofstream;
using std::ios;
String filename;
TextPersistenceManager	pm;
using namespace RTTI;
BitVector bv4(4);
bv4.setBit(2, true);

CHECK(friend std::istream& operator >> (std::istream& s, BitVector& bit_vector) throw(Exception::OutOfMemory))
	std::ifstream instr("data/BitVector_test.txt");
	BitVector bv4_2;
	instr >> bv4_2;
	instr.close();
	TEST_EQUAL(bv4_2.getSize(), 4)
	TEST_EQUAL(bv4_2.getBit(0), false)
	TEST_EQUAL(bv4_2.getBit(1), false)
	TEST_EQUAL(bv4_2.getBit(2), true)
	TEST_EQUAL(bv4_2.getBit(3), false)
RESULT


CHECK(void read(std::istream& s) throw(Exception::OutOfMemory))
	std::ifstream instr("data/BitVector_test.txt");
	BitVector bv4_2;
	bv4_2.read(instr);
	instr.close();
	TEST_EQUAL(bv4_2.getSize(), 4)
	TEST_EQUAL(bv4_2.getBit(0), false)
	TEST_EQUAL(bv4_2.getBit(1), false)
	TEST_EQUAL(bv4_2.getBit(2), true)
	TEST_EQUAL(bv4_2.getBit(3), false)
RESULT
 
CHECK(friend std::ostream& operator << (std::ostream& s, const BitVector& bit_vector) throw())
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << bv4;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/BitVector_test.txt")
RESULT

CHECK(bool isValid() const throw())
	std::ifstream instr("data/BitVector_test2.txt");
	BitVector bv4_2;
	bv4_2.read(instr);
	instr.close();
	TEST_EQUAL(bv4_2.getSize(), 4)
	TEST_EQUAL(bv4_2.getBit(0), false)
	TEST_EQUAL(bv4_2.getBit(1), false)
	TEST_EQUAL(bv4_2.getBit(2), true)
	TEST_EQUAL(bv4_2.getBit(3), false)
RESULT

CHECK(void write(std::ostream& s) const throw())
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	bv4.write(outstr);
	outstr.close();
	TEST_FILE(filename.c_str(), "data/BitVector_test2.txt")
RESULT

CHECK(void write(std::ostream& s) const throw())
	NEW_TMP_FILE(filename)
	ofstream	ofile(filename.c_str(), ios::out);
	pm.setOstream(ofile);
	using namespace RTTI;
	pm.registerClass(getStreamName<BitVector>(), BitVector::createDefault);
	bv4.write(pm);
	ofile.close();
	TEST_FILE(filename.c_str(), "data/BitVector_test3.txt")
RESULT

CHECK(bool read(PersistenceManager& pm) throw(Exception::OutOfMemory))
	ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	BitVector bv;
	TEST_NOT_EQUAL(bv.read(pm), false);
	ifile.close();
	TEST_EQUAL(bv.getSize(), 4)
	TEST_EQUAL(bv.getBit(0), false)
	TEST_EQUAL(bv.getBit(1), false)
	TEST_EQUAL(bv.getBit(2), true)
	TEST_EQUAL(bv.getBit(3), false)
RESULT

CHECK(void write(PersistenceManager& pm) const throw())
	String newfile;
	NEW_TMP_FILE(newfile)
	std::ofstream outstr(newfile.c_str(), std::ios::out);
	pm.setOstream(outstr);
	bv4.write(pm);
	TEST_FILE(newfile.c_str(), filename.c_str())
RESULT

CHECK(Size getSize() const throw())
	BitVector b(8);
	b.setBit(3);
	TEST_EQUAL(b.getSize(), 8)
	TEST_EQUAL(b.getBit(0), false)
	TEST_EQUAL(b.getBit(1), false)
	TEST_EQUAL(b.getBit(2), false)
	TEST_EQUAL(b.getBit(3), true)
	TEST_EQUAL(b.getBit(4), false)
	TEST_EQUAL(b.getBit(5), false)
	TEST_EQUAL(b.getBit(6), false)
	TEST_EQUAL(b.getBit(7), false)
	b.setSize(24, true);
	TEST_EQUAL(b.getSize(), 24)
	TEST_EQUAL(b.getBit(0), false)
	TEST_EQUAL(b.getBit(1), false)
	TEST_EQUAL(b.getBit(2), false)
	TEST_EQUAL(b.getBit(3), true)
	TEST_EQUAL(b.getBit(4), false)
	TEST_EQUAL(b.getBit(5), false)
	TEST_EQUAL(b.getBit(6), false)
	TEST_EQUAL(b.getBit(7), false)
	TEST_EQUAL(b.getBit(8), false)
	TEST_EQUAL(b.getBit(9), false)
	TEST_EQUAL(b.getBit(10), false)
	TEST_EQUAL(b.getBit(11), false)
	TEST_EQUAL(b.getBit(12), false)
	TEST_EQUAL(b.getBit(13), false)
	TEST_EQUAL(b.getBit(14), false)
	TEST_EQUAL(b.getBit(15), false)
	TEST_EQUAL(b.getBit(16), false)
	TEST_EQUAL(b.getBit(17), false)
	TEST_EQUAL(b.getBit(18), false)
	TEST_EQUAL(b.getBit(19), false)
	TEST_EQUAL(b.getBit(20), false)
	TEST_EQUAL(b.getBit(21), false)
	TEST_EQUAL(b.getBit(22), false)
	TEST_EQUAL(b.getBit(23), false)
	b.setSize(24, false);
	TEST_EQUAL(b.getSize(), 24)
	b.setSize(0, false);
	TEST_EQUAL(b.getBit(0), false)
	TEST_EQUAL(b.getBit(1), false)
	TEST_EQUAL(b.getBit(2), false)
	TEST_EQUAL(b.getBit(3), false)
	TEST_EQUAL(b.getBit(4), false)
	TEST_EQUAL(b.getBit(5), false)
	TEST_EQUAL(b.getBit(6), false)
	TEST_EQUAL(b.getBit(7), false)
	TEST_EQUAL(b.getBit(8), false)
	TEST_EQUAL(b.getBit(9), false)
	TEST_EQUAL(b.getBit(10), false)
	TEST_EQUAL(b.getBit(11), false)
	TEST_EQUAL(b.getBit(12), false)
	TEST_EQUAL(b.getBit(13), false)
	TEST_EQUAL(b.getBit(14), false)
	TEST_EQUAL(b.getBit(15), false)
	TEST_EQUAL(b.getBit(16), false)
	TEST_EQUAL(b.getBit(17), false)
	TEST_EQUAL(b.getBit(18), false)
	TEST_EQUAL(b.getBit(19), false)
	TEST_EQUAL(b.getBit(20), false)
	TEST_EQUAL(b.getBit(21), false)
	TEST_EQUAL(b.getBit(22), false)
	TEST_EQUAL(b.getBit(23), false)
	TEST_EQUAL(b.getSize(), 24)
RESULT


////////////////////////BIT//////////////////////////////////

BitVector bv(8);
bv.setBit(1, true);
bv.setBit(3, true);
bv.setBit(5, true);
bv.setBit(7, true);
bv[1] = true;
const BitVector cbv(bv);

Bit* b_ptr;

CHECK(Bit())
	b_ptr = new Bit();
	TEST_NOT_EQUAL(b_ptr, 0)
RESULT

CHECK(~Bit())
	delete b_ptr;
RESULT

CHECK(Bit(BitVector* bitvector, Index index = 0))
	Bit b(&bv, 0);
	TEST_EQUAL(b, false)
	Bit b1(&bv, 1);
	TEST_EQUAL(b1, true)
	Bit b7(&bv, 1);
	TEST_EQUAL(b7, true)
	Bit b8(&bv, 1);
	TEST_EQUAL(b8, true)

	BitVector* bvp = 0;
	TEST_EXCEPTION(Exception::NullPointer, Bit cb(bvp, 0))
RESULT

CHECK(BALL_CREATE(Bit))
	Bit b7(&bv, 1);
	Bit* v_ptr = (Bit*)b7.create(false, true);
	TEST_EQUAL(*v_ptr, false)
	delete v_ptr;
	v_ptr = (Bit*)b7.create();
	TEST_EQUAL(*v_ptr, true)
	delete v_ptr;
RESULT

CHECK(Bit(const BitVector* const bitvector, Index index = 0))
	Bit cb(&cbv, 0);
	TEST_EQUAL(cb, false)
	Bit cb1(&cbv, 1);
	TEST_EQUAL(cb1, true)

	Bit cbm8(&cbv, -8);
	TEST_EQUAL(cbm8, false)

	const BitVector* cbvp = 0;
	TEST_EXCEPTION(Exception::NullPointer, Bit cb0(cbvp, 0))
	TEST_EXCEPTION(Exception::IndexUnderflow, Bit cbx(&cbv, -9))
	TEST_EXCEPTION(Exception::IndexOverflow,  Bit cbx(&cbv,  9))

	TEST_EQUAL(cbv.getSize(), 8)
RESULT

CHECK(Bit(const Bit& bit))
	Bit cb(&cbv, 0);
	Bit* b2_ptr = new Bit(cb);
	TEST_NOT_EQUAL(b2_ptr, 0)
	TEST_EQUAL(*b2_ptr == cb, true)
	delete b2_ptr;
RESULT

CHECK(operator bool())
	Bit b(&bv, 0);
	TEST_EQUAL((bool) b, false)
	Bit b1(&bv, 1);
	TEST_EQUAL((bool) b1, true)
RESULT

Bit b0;

CHECK(operator = (const Bit& bit))
	Bit b1(&bv, 1);
	Bit b6(&bv, 6);
	TEST_EQUAL(b6, false)
	TEST_EQUAL(b1, true)
	b6 = b1;
	TEST_EQUAL(b1 == b6, true)
	TEST_EQUAL(b6, true)

	TEST_EQUAL(bv[1], true)
	TEST_EQUAL(bv[6], false)
RESULT

CHECK(operator = (bool bit))
	bv[6] = true;
	Bit b6(&bv, 6);
	TEST_EQUAL(bv[6], true)
	b6 = false;
	TEST_EQUAL(bv[6], false)

	b6 = true;
	TEST_EQUAL(b6, true)
	TEST_EQUAL(bv[6], true)

	Bit cb(&cbv, 6);
	TEST_EQUAL(cb, false)
	TEST_EXCEPTION(Bit::IllegalOperation, cb = true)
	TEST_EQUAL(cbv[6], false)

	TEST_EXCEPTION(Exception::NullPointer, b0 = true)
RESULT

CHECK(clear())
	Bit b(&bv, 1);
	TEST_EQUAL(b, true)
	b.clear();

	TEST_EXCEPTION(Exception::NullPointer, b == false)
RESULT

CHECK(operator == (const Bit& bit))
	Bit b(&bv, 0);
	Bit b1(&bv, 2);
	TEST_EQUAL(b == b1, false)

	b = Bit(&bv, 1);
	b1 = Bit(&cbv, 1);
	TEST_EQUAL(b == b1, false)

	b = Bit(&bv, 1);
	b1 = Bit(&bv, 1);
	TEST_EQUAL(b == b1, true)
RESULT

CHECK(operator == (const bool bit))
	Bit b(&bv, 0);
	TEST_EQUAL(b == false, true)
	b = Bit(&bv, 1);
	TEST_EQUAL(b == true, true)
	b = false;
	TEST_EQUAL(b == true, false)
	b = true;
	TEST_EQUAL(b == false, false)

	TEST_EXCEPTION(Exception::NullPointer, b0 == true)
RESULT

CHECK(operator != (const Bit& bit))
	Bit b(&bv, 0);
	Bit b1(&bv, 2);
	TEST_EQUAL(b != b1, true)

	b = Bit(&bv, 1);
	b1 = Bit(&cbv, 1);
	TEST_EQUAL(b != b1, true)

	b = Bit(&bv, 1);
	b1 = Bit(&bv, 1);
	TEST_EQUAL(b != b1, false)
RESULT

CHECK(operator != (const bool bit))
	Bit b(&bv, 0);
	TEST_EQUAL(b != false, false)
	b = Bit(&bv, 1);
	TEST_EQUAL(b != true, false)
	b = false;
	TEST_EQUAL(b != true, true)
	b = true;
	TEST_EQUAL(b != false, true)

	TEST_EXCEPTION(Exception::NullPointer, b0 != true)
RESULT

CHECK(IllegalOperation(const char* file, int line))
	Bit::IllegalOperation ip(__FILE__, __LINE__);
RESULT
///////////////////////////////////////////////////
END_TEST
