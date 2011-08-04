// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

///////////////////////////

START_TEST(Bit)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

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
	Bit* v_ptr = (Bit*)b7.create();
	TEST_NOT_EQUAL(v_ptr, 0)
	ABORT_IF(v_ptr == 0)
	TEST_EQUAL(*v_ptr, true)
	delete v_ptr;
	v_ptr = 0;
	v_ptr = (Bit*)b7.create(false, true);
	TEST_NOT_EQUAL(v_ptr, 0)
	ABORT_IF(v_ptr == 0)
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
