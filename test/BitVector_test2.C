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
 
BitVector bv9(9);

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
	std::ifstream instr(BALL_TEST_DATA_PATH(BitVector_test.txt));
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
	std::ifstream instr(BALL_TEST_DATA_PATH(BitVector_test.txt));
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
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(BitVector_test.txt))
RESULT

CHECK(void write(std::ostream& s) const throw())
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	bv4.write(outstr);
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(BitVector_test2.txt))
RESULT

CHECK(bool read(PersistenceManager& pm) throw(Exception::OutOfMemory))
	ifstream	ifile(BALL_TEST_DATA_PATH(BitVector_test3.txt));
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
	NEW_TMP_FILE(filename)
	ofstream	ofile(filename.c_str(), ios::out);
	pm.setOstream(ofile);
	using namespace RTTI;
	pm.registerClass(getStreamName<BitVector>(), BitVector::createDefault);
	bv4.write(pm);
	ofile.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(BitVector_test3.txt))
RESULT

END_TEST
