// $Id: BitVector_test.C,v 1.23 2001/10/11 00:28:35 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

///////////////////////////

START_TEST(BitVector, "$Id: BitVector_test.C,v 1.23 2001/10/11 00:28:35 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

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
	TEST_EQUAL(bv.getSize(), 0)
	TEST_EQUAL(bv.getBit(0), false)
RESULT

CHECK(BitVector::BitVector(Size))
	BitVector bv0((Size)0);
	TEST_EQUAL(bv0.getSize(), 0)
	TEST_EQUAL(bv0.getBit(0), false)

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

CHECK(BitVector::BitVector(const BitVector&, bool))
	bv9.setBit(0, true);
	bv9.setBit(8, true);
	BitVector bv9_2 = BitVector(bv9);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(BitVector::BitVector(const char*))
	const char* c = "100101011";
	BitVector bv9_2 = BitVector(c);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(2), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(BitVector::clear())
	bv9.setBit(0, true);
	bv9.setBit(8, true);
	TEST_EQUAL(bv9.getSize(), 9)
	bv9.clear();
	TEST_EQUAL(bv9.getBit(0), false)
	TEST_EQUAL(bv9.getBit(8), false)
	bv9.setBit(0, true);
	bv9.setBit(8, true);
	TEST_EQUAL(bv9.getSize(), 9)
RESULT

CHECK(BitVector::set(const BitVector&, bool))
	BitVector bv9_2;
	bv9_2.set(bv9);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(BitVector::set(const char*))
	const char* c = "100101011";
	BitVector bv9_2;
	bv9_2.set(c);
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(2), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(BitVector::BitVector& operator = (const BitVector&))
	BitVector bv9_2;
	bv9_2 = bv9;
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
RESULT

CHECK(BitVector::BitVector& operator = (const char*))
	const char* c = "100101011";
	BitVector bv9_2;
	bv9_2 = c;
	TEST_EQUAL(bv9_2.getSize(), 9)
	TEST_EQUAL(bv9_2.getBit(0), true)
	TEST_EQUAL(bv9_2.getBit(1), false)
	TEST_EQUAL(bv9_2.getBit(8), true)
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

BitVector bv10(10);
bv10.setBit(1, true);
bv10.setBit(2, true);
bv10.setBit(4, true);
bv10.setBit(9, true);

CHECK(BitVector::operator () (Index, Index) const)
	BitVector bv = bv10(1, 4);
	TEST_EQUAL(bv.getBit(0), true)
	TEST_EQUAL(bv.getBit(1), true)
	TEST_EQUAL(bv.getBit(2), false)
	TEST_EQUAL(bv.getBit(3), true)
RESULT

CHECK(BitVector::countValue(bool) const)
	TEST_EQUAL(bv10.countValue(true), 4)
	TEST_EQUAL(bv10.countValue(false), 6)
RESULT

CHECK(BitVector::getBitSet())
	BitVector bv(10);
	BitVector::VectorType& bt = bv.getBitSet();
	bt[0] = 1;
	TEST_EQUAL(bv[0], true)
	TEST_EQUAL(bv[1], false)
	TEST_EQUAL(bv[2], false)
	bv.setBit(1);
	TEST_EQUAL(bt[0], 3)
RESULT

CHECK(BitVector::getBitSet() const)
	const BitVector bv10_2(bv10);
	TEST_EQUAL(bv10_2.getBit(0), false)
	TEST_EQUAL(bv10_2.getBit(1), true)
	const BitVector::VectorType& bt = bv10_2.getBitSet();
	TEST_EQUAL(bt[0], 22)
	TEST_EQUAL(bt[1], 2)
RESULT

CHECK(BitVector::operator [] (Index))
	BitVector bv3(3);
	bv3.setBit(1, true);
	TEST_EQUAL(bv3[0], false)
	TEST_EQUAL(bv3[1], true)
	TEST_EQUAL(bv3[3], false)
	TEST_EQUAL(bv3.getSize(), 4)
RESULT
CHECK(BitVector::operator [] (Index) const)
	const BitVector bv10_2(bv10);
	TEST_EQUAL(bv10_2[0], false)
	TEST_EQUAL(bv10_2[1], true)
	TEST_EXCEPTION(Exception::IndexOverflow, bv10_2[1111])
	TEST_EQUAL(bv10_2.getSize(), 10)
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

	const BitVector const_bv(8);
	TEST_EXCEPTION(Exception::IndexOverflow, const_bv.getBit(11))
	TEST_EQUAL(const_bv.getSize(), 8)

	BitVector bv;
	bv.setBit(0, true);
	TEST_EQUAL(bv.getBit(0), true)
RESULT

CHECK(BitVector::toggleBit(Index))
	TEST_EQUAL(bv9.getBit(0), false)
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
	BitVector bv4(4);
	bv4.setBit(1, true);
	bv4.toggle(1, 2);
	TEST_EQUAL(bv4.getBit(0), false)
	TEST_EQUAL(bv4.getBit(1), false)
	TEST_EQUAL(bv4.getBit(2), true)
	TEST_EQUAL(bv4.getBit(3), false)
RESULT

CHECK(BitVector::setUnsignedChar(unsigned char)/getUnsignedChar)
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
	TEST_EQUAL((int)bv8.getUnsignedChar(), (int)0xaa)
	bv8.setUnsignedChar((char)0);
	TEST_EQUAL(bv8.getBit(0), false)
	TEST_EQUAL(bv8.getBit(1), false)
	TEST_EQUAL(bv8.getBit(2), false)
	TEST_EQUAL(bv8.getBit(3), false)
	TEST_EQUAL(bv8.getBit(4), false)
	TEST_EQUAL(bv8.getBit(5), false)
	TEST_EQUAL(bv8.getBit(6), false)
	TEST_EQUAL(bv8.getBit(7), false)
	TEST_EQUAL((int)bv8.getUnsignedChar(), (int)0x0)
	bv8.setUnsignedChar((char)2);
	TEST_EQUAL(bv8.getBit(0), false)
	TEST_EQUAL(bv8.getBit(1), true)
	TEST_EQUAL(bv8.getBit(2), false)
	TEST_EQUAL(bv8.getBit(3), false)
	TEST_EQUAL(bv8.getBit(4), false)
	TEST_EQUAL(bv8.getBit(5), false)
	TEST_EQUAL(bv8.getBit(6), false)
	TEST_EQUAL(bv8.getBit(7), false)
	TEST_EQUAL((int)bv8.getUnsignedChar(), (int)0x2)
RESULT

CHECK(BitVector::setUnsignedShort(unsigned short)/getUnsignedShort)
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
	TEST_EQUAL(bv8.getUnsignedShort(), 1)
RESULT
 
CHECK(BitVector::setUnsignedInt(unsigned int)/getUnsignedInt)
	BitVector bv8(8);
	bv8.setUnsignedInt(1);
	TEST_EQUAL(bv8.getBit(0), true)
	TEST_EQUAL(bv8.getBit(1), false)
	TEST_EQUAL(bv8.getUnsignedInt(), 1)
RESULT

CHECK(BitVector::setUnsignedLong(unsigned long)/getUnsignedLong)
	BitVector bv8(8);
	bv8.setUnsignedLong(1);
	TEST_EQUAL(bv8.getBit(0), true)
	TEST_EQUAL(bv8.getBit(1), false)
	TEST_EQUAL(bv8.getUnsignedLong(), 1)
RESULT

BitVector b4(4);	// **_* b4
BitVector b3(3);	// *__  b3
b4.setBit(0);
b4.setBit(1);
b4.setBit(3);
b3.setBit(0);
BitVector erg;

CHECK(BitVector::bitwiseOr(const BitVector&))
	erg = b4;
	erg.bitwiseOr(b3);
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::bitwiseXor(const BitVector&))
	erg = b4;
	erg.bitwiseXor(b3);
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::bitwiseAnd(const BitVector&))
	erg = b4;
	erg.bitwiseAnd(b3);
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), false)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), false)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::operator | (const BitVector&))
	erg = b4 | b3;
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::operator |= (const BitVector&))
	erg = b4;
	erg |= b3;
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::operator & (const BitVector&))
	erg = b4 & b3;
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), false)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), false)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::operator &= (const BitVector&))
	erg = b4;
	erg &= b3;
	TEST_EQUAL(erg.getBit(0), true)
	TEST_EQUAL(erg.getBit(1), false)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), false)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::operator ^ (const BitVector&))
	erg = b4 ^ b3;
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::operator ^= (const BitVector&))
	erg = b4;
	erg ^= b3;
	TEST_EQUAL(erg.getBit(0), false)
	TEST_EQUAL(erg.getBit(1), true)
	TEST_EQUAL(erg.getBit(2), false)
	TEST_EQUAL(erg.getBit(3), true)
	TEST_EQUAL(erg.getSize(), 4)
RESULT

CHECK(BitVector::operator ~ ())
	BitVector bv3(3);
	bv3.setBit(1, true);
	BitVector bv3_2 = ~ bv3;
	TEST_EQUAL(bv3_2.getBit(0), true)
	TEST_EQUAL(bv3_2.getBit(1), false)
	TEST_EQUAL(bv3_2.getBit(2), true)
	TEST_EQUAL(bv3_2.getSize(), 3)
RESULT

CHECK(BitVector::operator == (const BitVector&) const)
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

CHECK(BitVector::operator != (const BitVector&) const)
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

CHECK(BitVector::isAnyBit(bool value, Index first = 0, Index last = -1) const)
	BitVector bv3(3);
	TEST_EQUAL(bv3.isAnyBit(false), true)
	TEST_EQUAL(bv3.isAnyBit(true), false)
	bv3.setBit(0, true);
	TEST_EQUAL(bv3.isAnyBit(true, 1, -1), false)
	TEST_EQUAL(bv3.isAnyBit(true), true)
RESULT

CHECK(BitVector::isEveryBit(bool, Index, Index) const)
	BitVector bv3(3);
	TEST_EQUAL(bv3.isEveryBit(false), true)
	bv3.setBit(0, true);
	TEST_EQUAL(bv3.isEveryBit(false), false)
	TEST_EQUAL(bv3.isEveryBit(false, 1, 2), true)
	TEST_EQUAL(bv3.isEveryBit(false, 0, 1), false)
	bv3.setBit(1, true);
	bv3.setBit(2, true);
	TEST_EQUAL(bv3.isEveryBit(true), true)
RESULT
 
CHECK(BitVector::isValid() const)
	TEST_EQUAL(bv9.isValid(), true)
RESULT

using std::ofstream;
using std::ios;
String filename;
TextPersistenceManager	pm;
using namespace RTTI;
BitVector bv4(4);
bv4.setBit(2, true);

CHECK(BitVector::operator >> (istream&, BitVector&))
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
 
CHECK(BitVector::operator << (ostream&, const BitVector&))
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << bv4;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/BitVector_test.txt", false)
RESULT

CHECK(BitVector::read(istream&))
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

CHECK(BitVector::write(ostream&) const)
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	bv4.write(outstr);
	outstr.close();
	TEST_FILE(filename.c_str(), "data/BitVector_test2.txt", false)
RESULT

CHECK(BitVector::write(PersistenceManager&) const)
	NEW_TMP_FILE(filename)
	ofstream	ofile(filename.c_str(), ios::out);
	pm.setOstream(ofile);
	using namespace RTTI;
	pm.registerClass(getStreamName<BitVector>(), BitVector::createDefault);
	bv4.write(pm);
	ofile.close();
	TEST_FILE(filename.c_str(), "data/BitVector_test3.txt", false)
RESULT

CHECK(BitVector::read(PersistenceManager&))
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

CHECK(BitVector::setSize())
	BitVector b(8);
	b.setBit(3);
	TEST_EQUAL(b.getSize(), 8)
	b.setSize(24, true);
	TEST_EQUAL(b.getSize(), 24)
	b.setSize(24, false);
	TEST_EQUAL(b.getSize(), 24)
	b.setSize(0, false);
	TEST_EQUAL(b.getSize(), 0)
RESULT


////////////////////////BIT//////////////////////////////////

BitVector* bv = new BitVector(8);
bv->setBit(1, true);
bv->setBit(3, true);
bv->setBit(5, true);
bv->setBit(7, true);
(*bv)[1] = true;
const BitVector* cbv = new BitVector(*bv);

Bit* b_ptr;

CHECK(Bit())
	b_ptr = new Bit();
	TEST_NOT_EQUAL(b_ptr, 0)
RESULT

CHECK(~Bit())
	delete b_ptr;
RESULT

CHECK(Bit(BitVector* bitvector, Index index = 0))
	Bit b(bv, 0);
	TEST_EQUAL(b, false)
	Bit b1(bv, 1);
	TEST_EQUAL(b1, true)
	Bit b7(bv, 1);
	TEST_EQUAL(b7, true)
	Bit b8(bv, 1);
	TEST_EQUAL(b8, true)

	Bit b99(bv, 99);
	b99 = true;
	TEST_EQUAL(bv->getSize(), 100)
	TEST_EQUAL(b99, true)

	BitVector* bvp = 0;
	TEST_EXCEPTION(Exception::NullPointer, Bit cb(bvp, 0))
RESULT

CHECK(Bit(const BitVector* const bitvector, Index index = 0))
	Bit cb(cbv, 0);
	TEST_EQUAL(cb, false)
	Bit cb1(cbv, 1);
	TEST_EQUAL(cb1, true)

	Bit cbm8(cbv, -8);
	TEST_EQUAL(cbm8, false)

	const BitVector* cbvp = 0;
	TEST_EXCEPTION(Exception::NullPointer, Bit cb0(cbvp, 0))
	TEST_EXCEPTION(Exception::IndexUnderflow, Bit cbx(cbv, -9))
	TEST_EXCEPTION(Exception::IndexOverflow,  Bit cbx(cbv,  9))

	TEST_EQUAL(cbv->getSize(), 8)
RESULT

CHECK(Bit(const Bit& bit))
	Bit cb(cbv, 0);
	Bit* b2_ptr = new Bit(cb);
	TEST_NOT_EQUAL(b2_ptr, 0)
	TEST_EQUAL(*b2_ptr == cb, true)
RESULT

CHECK(operator bool())
	Bit b(bv, 0);
	TEST_EQUAL((bool) b, false)
	Bit b1(bv, 1);
	TEST_EQUAL((bool) b1, true)
RESULT

Bit b0;

CHECK(operator = (const Bit& bit))
	Bit b1(bv, 1);
	Bit b6(bv, 6);
	TEST_EQUAL(b6, false)
	TEST_EQUAL(b1, true)
	b6 = b1;
	TEST_EQUAL(b1 == b6, true)
	TEST_EQUAL(b6, true)

	TEST_EQUAL((*bv)[1], true)
	TEST_EQUAL((*bv)[6], false)
RESULT

CHECK(operator = (bool bit))
	(*bv)[6] = true;
	Bit b6(bv, 6);
	TEST_EQUAL((*bv)[6], true)
	b6 = false;
	TEST_EQUAL((*bv)[6], false)

	b6 = true;
	TEST_EQUAL(b6, true)
	TEST_EQUAL((*bv)[6], true)

	Bit cb(cbv, 6);
	TEST_EQUAL(cb, false)
	TEST_EXCEPTION(Bit::IllegalOperation, cb = true)
	TEST_EQUAL((*cbv)[6], false)

	TEST_EXCEPTION(Exception::NullPointer, b0 = true)
RESULT

CHECK(clear())
	Bit b(bv, 1);
	TEST_EQUAL(b, true)
	b.clear();

	TEST_EXCEPTION(Exception::NullPointer, b == false)
RESULT

CHECK(operator == (const Bit& bit))
	Bit b(bv, 0);
	Bit b1(bv, 2);
	TEST_EQUAL(b == b1, false)

	b = Bit(bv, 1);
	b1 = Bit(cbv, 1);
	TEST_EQUAL(b == b1, false)

	b = Bit(bv, 1);
	b1 = Bit(bv, 1);
	TEST_EQUAL(b == b1, true)
RESULT

CHECK(operator == (const bool bit))
	Bit b(bv, 0);
	TEST_EQUAL(b == false, true)
	b = Bit(bv, 1);
	TEST_EQUAL(b == true, true)
	b = false;
	TEST_EQUAL(b == true, false)
	b = true;
	TEST_EQUAL(b == false, false)

	TEST_EXCEPTION(Exception::NullPointer, b0 == true)
RESULT

CHECK(operator != (const Bit& bit))
	Bit b(bv, 0);
	Bit b1(bv, 2);
	TEST_EQUAL(b != b1, true)

	b = Bit(bv, 1);
	b1 = Bit(cbv, 1);
	TEST_EQUAL(b != b1, true)

	b = Bit(bv, 1);
	b1 = Bit(bv, 1);
	TEST_EQUAL(b != b1, false)
RESULT

CHECK(operator != (const bool bit))
	Bit b(bv, 0);
	TEST_EQUAL(b != false, false)
	b = Bit(bv, 1);
	TEST_EQUAL(b != true, false)
	b = false;
	TEST_EQUAL(b != true, true)
	b = true;
	TEST_EQUAL(b != false, true)

	TEST_EXCEPTION(Exception::NullPointer, b0 != true)
RESULT

///////////////////////////////////////////////////
END_TEST
