// $Id: BitVector_test.C,v 1.7 2000/07/20 21:40:27 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

///////////////////////////

START_TEST(BitVector, "$Id: BitVector_test.C,v 1.7 2000/07/20 21:40:27 amoll Exp $")

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

BitVector bv10(10);
bv10.setBit(1, true);
bv10.setBit(2, true);
bv10.setBit(4, true);
bv10.setBit(9, true);

CHECK(BitVector::operator () (Index,Index) const)
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

CHECK(BitVector::getMaxIndex() const)
	TEST_EQUAL(bv10.getMaxIndex(), 9)
	BitVector b;
	TEST_EQUAL(b.getMaxIndex(), 0)
RESULT

CHECK(BitVector::getBitSet())
	unsigned char* bt;
	bt = bv10.getBitSet();
	TEST_EQUAL(*bt, 0)
	TEST_EQUAL(*(bt + 1), 1)
RESULT

CHECK(BitVector::getBitSet() const)
	unsigned char* c = 0;
	TEST_EQUAL(bv10.getBitSet(), c)
RESULT

CHECK(BitVector::operator [] (Index))
	//****************
RESULT

CHECK(BitVector::operator [] (Index) const)
	TEST_EQUAL(bv10[0] == false, true)
	TEST_EQUAL(bv10[1] == true, true)
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

CHECK(BitVector::setUnsignedChar(unsigned char))
	// BAUSTELLE
RESULT

CHECK(BitVector::getUnsignedChar() const)
	//TEST_EQUAL(bv4.getUnsignedChar(), 0)
RESULT

CHECK(BitVector::setUnsignedShort(unsigned short))
	// BAUSTELLE
RESULT
 
CHECK(BitVector::setUnsignedInt(unsigned int bit_pattern))
//
RESULT

CHECK(BitVector::getUnsignedInt() const)
	//TEST_EQUAL(bv4.getUnsignedInt(), 0)
RESULT

CHECK(BitVector::setUnsignedLong(unsigned long))
	// BAUSTELLE
RESULT

CHECK(BitVector::getUnsignedLong() const)
	//TEST_EQUAL(bv4.getUnsignedLong(), 0)
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
	PersistentObject*	ptr;
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<BitVector>(*ptr), true)
		if (isKindOf<BitVector>(*ptr))
		{
			BitVector* bv4_2 = castTo<BitVector>(*ptr);
			TEST_EQUAL(bv4_2->getSize(), 4)
			TEST_EQUAL(bv4_2->getBit(0), false)
			TEST_EQUAL(bv4_2->getBit(1), false)
			TEST_EQUAL(bv4_2->getBit(2), true)
			TEST_EQUAL(bv4_2->getBit(3), false)
		}
	}
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
