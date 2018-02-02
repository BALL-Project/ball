// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

///////////////////////////

START_TEST(BitVector)

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


CHECK(void setSize(Size size, bool keep = true))
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

CHECK(BitVector(Size size))
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

CHECK(BitVector(const BitVector& bit_vector))
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

CHECK(BitVector(const char* bit_string))
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


CHECK(void set(const BitVector& bit_vector))
	BitVector bv9_1("100000001");
	BitVector bv9_2;
	bv9_2.set(bv9_1);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(void set(const char* bit_string))
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

CHECK(BitVector& operator = (const BitVector& bit_vector))
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

CHECK(BitVector& operator = (const char *bit_string))
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

CHECK(void get(BitVector& bitvector) const)
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

CHECK(BitVector operator () (Index first, Index last) const)
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

CHECK(Bit operator []	(Index index))
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

CHECK(bool operator [] (Index index) const)
	const BitVector bv10_2(bv10);
	TEST_EQUAL(bv10_2[0], false)
	TEST_EQUAL(bv10_2[1], true)
	TEST_EXCEPTION(Exception::IndexOverflow, bv10_2[1111])
	TEST_EXCEPTION(Exception::IndexUnderflow, bv10_2[-1111])
	TEST_EQUAL(bv10_2.getSize(), 10)
RESULT

CHECK(void setBit(Index index, bool value = true))
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

CHECK(bool getBit(Index index) const)
		BitVector bv(2);
		bv.setBit(1);
		const BitVector& b(bv);
		TEST_EQUAL(b.getBit(0), false)
		TEST_EQUAL(b.getBit(1), true)
		TEST_EXCEPTION(Exception::IndexOverflow, b.getBit(11111111))
		TEST_EXCEPTION(Exception::IndexUnderflow, b.getBit(-111111111))
RESULT

CHECK(bool getBit(Index index))
RESULT

CHECK(void toggleBit(Index index))
	TEST_EQUAL(bv9.getBit(0), false)
	bv9.toggleBit(0);
	TEST_EQUAL(bv9.getBit(0), true)
RESULT

CHECK(void fill(bool value = true, Index first = 0, Index last = -1))
	bv9.fill(true, 2, 3);
	TEST_EQUAL(bv9.getBit(1), false)
	TEST_EQUAL(bv9.getBit(2), true)
	TEST_EQUAL(bv9.getBit(3), true)
	TEST_EQUAL(bv9.getBit(4), false)
RESULT

CHECK(void toggle(Index first = 0, Index last = -1))
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

END_TEST
