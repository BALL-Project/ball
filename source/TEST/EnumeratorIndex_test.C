// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/enumerator.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

void char_assign(char& c1, const char& c2)
{
	c1 = c2;
}

START_TEST(Enumerator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class EnumeratorIndex::IncompatibleIndex::

EnumeratorIndex::IncompatibleIndex* ex_ptr;

CHECK([EXTRA] EnumeratorIndex::IncompatibleIndex(const char* file, int line))
	ex_ptr = new EnumeratorIndex::IncompatibleIndex(__FILE__,__LINE__);
	TEST_NOT_EQUAL(ex_ptr, 0)
RESULT

delete ex_ptr;

// tests for class EnumeratorIndex::

EnumeratorIndex* enumerator_index_ptr;

CHECK(EnumeratorIndex() throw())
  enumerator_index_ptr = new EnumeratorIndex;
	TEST_NOT_EQUAL(enumerator_index_ptr, 0)
RESULT

CHECK(~EnumeratorIndex() throw())
  delete enumerator_index_ptr;
RESULT

Enumerator<String, String::iterator, char>::SiteList variant_list;
CHECK("template <typename Variant, typename VariantIterator> EnumeratorIndex(const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list) throw()")
	enumerator_index_ptr = new EnumeratorIndex(variant_list);
	TEST_NOT_EQUAL(enumerator_index_ptr, 0)
RESULT

CHECK(Size getSize() const throw())
	EnumeratorIndex ei;
	TEST_EQUAL(ei.getSize(), 0)
RESULT

CHECK(const std::vector<Size>& getModulus() const throw())
  vector<Size> modulus = enumerator_index_ptr->getModulus();
  TEST_EQUAL(modulus.size(), 0)
RESULT
delete enumerator_index_ptr;

CHECK(EnumeratorIndex& operator << (Size modulus) throw(Exception::OutOfRange))
	EnumeratorIndex ei;
	TEST_EQUAL(ei.getSize(), 0)
	
	ei << 2;
	TEST_EQUAL(ei.getSize(), 1)
	TEST_EQUAL(ei.getModulus(0), 2)
	TEST_EQUAL(ei[0], 0)

	ei << 4;
	TEST_EQUAL(ei.getSize(), 2)
	TEST_EQUAL(ei.getModulus(0), 2)
	TEST_EQUAL(ei.getModulus(1), 4)
	TEST_EQUAL(ei[0], 0)
	TEST_EQUAL(ei[1], 0)

	ei << 5 << 6;
	TEST_EQUAL(ei.getSize(), 4)
	TEST_EQUAL(ei.getModulus(0), 2)
	TEST_EQUAL(ei.getModulus(1), 4)
	TEST_EQUAL(ei.getModulus(2), 5)
	TEST_EQUAL(ei.getModulus(3), 6)
	TEST_EQUAL(ei[0], 0)
	TEST_EQUAL(ei[1], 0)
	TEST_EQUAL(ei[2], 0)
	TEST_EQUAL(ei[3], 0)

	TEST_EXCEPTION(Exception::OutOfRange, ei << 1)
	TEST_EXCEPTION(Exception::OutOfRange, ei << 0)
RESULT

CHECK(Position operator [] (Position pos) const throw())
	EnumeratorIndex ei;
	ei << 5 << 6 << 7;

	const EnumeratorIndex& c_ei(ei);
	TEST_EQUAL(c_ei[0], 0)
	TEST_EQUAL(c_ei[1], 0)
	TEST_EQUAL(c_ei[2], 0)
RESULT

CHECK(Position& operator [] (Position pos) throw())
	EnumeratorIndex ei;	
	ei << 5 << 6 << 7;

	const EnumeratorIndex& c_ei(ei);
	TEST_EQUAL(c_ei[0], 0)
	TEST_EQUAL(c_ei[1], 0)
	TEST_EQUAL(c_ei[2], 0)
	
	ei[0] = 2;
	TEST_EQUAL(c_ei[0], 2)
	TEST_EQUAL(c_ei[1], 0)
	TEST_EQUAL(c_ei[2], 0)
	
	ei[1] = 3;
	TEST_EQUAL(c_ei[0], 2)
	TEST_EQUAL(c_ei[1], 3)
	TEST_EQUAL(c_ei[2], 0)
	
	ei[2] = 4;
	TEST_EQUAL(c_ei[0], 2)
	TEST_EQUAL(c_ei[1], 3)
	TEST_EQUAL(c_ei[2], 4)
RESULT

CHECK(Size getModulus(Position pos) const throw())
	EnumeratorIndex ei;
	ei << 5 << 6 << 7;
RESULT

CHECK(const EnumeratorIndex& operator = (Position index) throw(Exception::IndexOverflow))
	EnumeratorIndex ei;
	ei << 4 << 5 << 6;
	TEST_EQUAL(ei[0], 0)
	TEST_EQUAL(ei[1], 0)
	TEST_EQUAL(ei[2], 0)
	ei = 1;
	TEST_EQUAL(ei[2], 1)
	TEST_EQUAL(ei[1], 0)
	TEST_EQUAL(ei[0], 0)
	ei = (4 * 5 * 6 - 1);
	TEST_EQUAL(ei[2], 5)
	TEST_EQUAL(ei[1], 4)
	TEST_EQUAL(ei[0], 3)	
	TEST_EXCEPTION(Exception::IndexOverflow, ei = 4 * 5 *6)
RESULT

CHECK(EnumeratorIndex& operator ++ () throw(Exception::IndexOverflow))
	EnumeratorIndex ei;
	ei << 2 << 3 << 4;
	ABORT_IF(ei.getSize() < 3)
	TEST_EQUAL(ei[0], 0) 
	TEST_EQUAL(ei[1], 0) 
	TEST_EQUAL(ei[2], 0) 
	for (Position i = 0; i < (2 * 3 * 4 - 1); ++i, ++ei)
	{
		Size current = ei[0] * 3 * 4 + ei[1] * 4 + ei[2];
		TEST_EQUAL(current, i)
	}
	TEST_EQUAL(ei[0], 1) 
	TEST_EQUAL(ei[1], 2) 
	TEST_EQUAL(ei[2], 3) 
	TEST_EXCEPTION(Exception::IndexOverflow, ++ei)
RESULT


CHECK(EnumeratorIndex& operator -- () throw(Exception::IndexUnderflow))
	EnumeratorIndex ei;
	ei << 2 << 3 << 4;
	ABORT_IF(ei.getSize() < 3)
	ei[0] = 1;
	ei[1] = 2;
	ei[2] = 3;
	for (Position i = (2 * 3 * 4 - 1); i > 0; --i, --ei)
	{
		Size current = ei[0] * 3 * 4 + ei[1] * 4 + ei[2];
		TEST_EQUAL(current, i)
	}
	TEST_EQUAL(ei[0], 0) 
	TEST_EQUAL(ei[1], 0) 
	TEST_EQUAL(ei[2], 0) 
	TEST_EXCEPTION(Exception::IndexUnderflow, --ei)
RESULT


CHECK(const EnumeratorIndex& operator = (const EnumeratorIndex& rhs) throw())
	EnumeratorIndex ei;
	ei << 2 << 3 << 4;
	ei = 17;	

	EnumeratorIndex ei2;
	ei2 = ei;

	TEST_EQUAL(ei2.getSize(), ei.getSize())
	ABORT_IF(ei2.getSize() != 3)
	TEST_EQUAL(ei2.getModulus(0), ei.getModulus(0))
	TEST_EQUAL(ei2.getModulus(1), ei.getModulus(1))
	TEST_EQUAL(ei2.getModulus(2), ei.getModulus(2))
	TEST_EQUAL(ei2[0], ei[0])
	TEST_EQUAL(ei2[1], ei[1])
	TEST_EQUAL(ei2[2], ei[2])
RESULT

CHECK("template <typename Variant, typename VariantIterator> const EnumeratorIndex& operator = (const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list) throw()")
  // ???
RESULT

CHECK(bool operator == (const EnumeratorIndex& rhs) const throw())
	EnumeratorIndex ei;
	ei << 2 << 3 << 4;
	ei[0] = 1;
	ei[1] = 2;
	ei[2] = 3;
	
	EnumeratorIndex ei2(ei);
	TEST_EQUAL(ei2 == ei, true)
	TEST_EQUAL(ei == ei2, true)
	
	ei2[0] = 0;
	TEST_EQUAL(ei2 == ei, false)
	TEST_EQUAL(ei == ei2, false)

	ei2[0] = 1;
	TEST_EQUAL(ei2 == ei, true)
	TEST_EQUAL(ei == ei2, true)

	ei2[1] = 0;
	TEST_EQUAL(ei2 == ei, false)
	TEST_EQUAL(ei == ei2, false)

	ei2[1] = 2;
	TEST_EQUAL(ei2 == ei, true)
	TEST_EQUAL(ei == ei2, true)

	ei2[2] = 0;
	TEST_EQUAL(ei2 == ei, false)
	TEST_EQUAL(ei == ei2, false)

	ei2[2] = 3;
	TEST_EQUAL(ei2 == ei, true)
	TEST_EQUAL(ei == ei2, true)

	EnumeratorIndex ei3;
	TEST_EQUAL(ei3 == ei, false)
	TEST_EQUAL(ei == ei3, false)

	ei3 << 2 << 3;
	TEST_EQUAL(ei3 == ei, false)
	TEST_EQUAL(ei == ei3, false)

	ei3 << 4;
	TEST_EQUAL(ei3 == ei, false)
	TEST_EQUAL(ei == ei3, false)
	
	ei3 << 5;
	TEST_EQUAL(ei3 == ei, false)
	TEST_EQUAL(ei == ei3, false)
	
	EnumeratorIndex ei4;
	ei4 << 2 << 3 << 5;
	TEST_EQUAL(ei4 == ei, false)
	TEST_EQUAL(ei == ei4, false)
RESULT


CHECK(bool operator != (const EnumeratorIndex& rhs) const throw())
	EnumeratorIndex ei;
	ei << 2 << 3 << 4;
	ei[0] = 1;
	ei[1] = 2;
	ei[2] = 3;
	
	EnumeratorIndex ei2(ei);
	TEST_EQUAL(ei2 != ei, false)
	TEST_EQUAL(ei != ei2, false)
	
	ei2[0] = 0;
	TEST_EQUAL(ei2 != ei, true)
	TEST_EQUAL(ei != ei2, true)

	ei2[0] = 1;
	TEST_EQUAL(ei2 != ei, false)
	TEST_EQUAL(ei != ei2, false)

	ei2[1] = 0;
	TEST_EQUAL(ei2 != ei, true)
	TEST_EQUAL(ei != ei2, true)

	ei2[1] = 2;
	TEST_EQUAL(ei2 != ei, false)
	TEST_EQUAL(ei != ei2, false)

	ei2[2] = 0;
	TEST_EQUAL(ei2 != ei, true)
	TEST_EQUAL(ei != ei2, true)

	ei2[2] = 3;
	TEST_EQUAL(ei2 != ei, false)
	TEST_EQUAL(ei != ei2, false)

	EnumeratorIndex ei3;
	TEST_EQUAL(ei3 == ei, false)
	TEST_EQUAL(ei == ei3, false)
	
	ei3 << 2 << 3;
	TEST_EQUAL(ei3 == ei, false)
	TEST_EQUAL(ei == ei3, false)

	ei3 << 4;
	TEST_EQUAL(ei3 != ei, true)
	TEST_EQUAL(ei != ei3, true)
	
	ei3 << 5;
	TEST_EQUAL(ei3 != ei, true)
	TEST_EQUAL(ei != ei3, true)
	
	EnumeratorIndex ei4;
	ei4 << 2 << 3 << 5;
	TEST_EQUAL(ei3 != ei, true)
	TEST_EQUAL(ei != ei3, true)
RESULT

EnumeratorIndex ei;
ei << 3 << 4 << 5;
ei[0] = 1;
ei[1] = 2;
ei[2] = 3;

EnumeratorIndex ei_equal(ei);

// lesser in the first digit
EnumeratorIndex ei_lesser_1;
ei_lesser_1 << 3 << 4 << 5;
ei_lesser_1[0] = 0;
ei_lesser_1[1] = 2;
ei_lesser_1[2] = 3;

// lesser in the 2nd digit
EnumeratorIndex ei_lesser_2;
ei_lesser_2 << 3 << 4 << 5;
ei_lesser_2[0] = 1;
ei_lesser_2[1] = 1;
ei_lesser_2[2] = 3;

// lesser in the 3rd digit
EnumeratorIndex ei_lesser_3;
ei_lesser_3 << 3 << 4 << 5;
ei_lesser_3[0] = 1;
ei_lesser_3[1] = 2;
ei_lesser_3[2] = 2;

// greater in the first digit
EnumeratorIndex ei_greater_1;
ei_greater_1 << 3 << 4 << 5;
ei_greater_1[0] = 2;
ei_greater_1[1] = 2;
ei_greater_1[2] = 3;

// greater in the 2nd digit
EnumeratorIndex ei_greater_2;
ei_greater_2 << 3 << 4 << 5;
ei_greater_2[0] = 1;
ei_greater_2[1] = 3;
ei_greater_2[2] = 3;

// greater in the 3rd digit
EnumeratorIndex ei_greater_3;
ei_greater_3 << 3 << 4 << 5;
ei_greater_3[0] = 1;
ei_greater_3[1] = 2;
ei_greater_3[2] = 4;

// different modulus size
EnumeratorIndex ei_lesser_size;
ei_lesser_size << 3 << 4;
EnumeratorIndex ei_greater_size;
ei_greater_size << 3 << 4 << 5 << 6;

// different modulus values
EnumeratorIndex ei_mod_diff_1;
ei_mod_diff_1 << 2 << 4 << 5;
EnumeratorIndex ei_mod_diff_2;
ei_mod_diff_1 << 3 << 3 << 5;
EnumeratorIndex ei_mod_diff_3;
ei_mod_diff_1 << 3 << 4 << 6;

CHECK(bool operator < (const EnumeratorIndex& rhs) const throw(EnumeratorIndex::IncompatibleIndex))
	TEST_EQUAL(ei < ei_equal, false)
	TEST_EQUAL(ei_equal < ei, false)
	
	TEST_EQUAL(ei_lesser_1 < ei, true)
	TEST_EQUAL(ei_lesser_2 < ei, true)
	TEST_EQUAL(ei_lesser_3 < ei, true)
	TEST_EQUAL(ei < ei_lesser_1, false)
	TEST_EQUAL(ei < ei_lesser_2, false)
	TEST_EQUAL(ei < ei_lesser_2, false)

	TEST_EQUAL(ei_greater_1 < ei, false)
	TEST_EQUAL(ei_greater_2 < ei, false)
	TEST_EQUAL(ei_greater_3 < ei, false)
	TEST_EQUAL(ei < ei_greater_1, true)
	TEST_EQUAL(ei < ei_greater_2, true)
	TEST_EQUAL(ei < ei_greater_2, true)

	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei < ei_lesser_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_lesser_size < ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei < ei_greater_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_greater_size < ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei < ei_mod_diff_1)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_1 < ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei < ei_mod_diff_2)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_2 < ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei < ei_mod_diff_3)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_3 < ei)
RESULT


CHECK(bool operator > (const EnumeratorIndex& rhs) const throw(EnumeratorIndex::IncompatibleIndex))
	TEST_EQUAL(ei > ei_equal, false)
	TEST_EQUAL(ei_equal > ei, false)
	
	TEST_EQUAL(ei_lesser_1 > ei, false)
	TEST_EQUAL(ei_lesser_2 > ei, false)
	TEST_EQUAL(ei_lesser_3 > ei, false)
	TEST_EQUAL(ei > ei_lesser_1, true)
	TEST_EQUAL(ei > ei_lesser_2, true)
	TEST_EQUAL(ei > ei_lesser_2, true)

	TEST_EQUAL(ei_greater_1 > ei, true)
	TEST_EQUAL(ei_greater_2 > ei, true)
	TEST_EQUAL(ei_greater_3 > ei, true)
	TEST_EQUAL(ei > ei_greater_1, false)
	TEST_EQUAL(ei > ei_greater_2, false)
	TEST_EQUAL(ei > ei_greater_2, false)

	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei > ei_lesser_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_lesser_size > ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei > ei_greater_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_greater_size > ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei > ei_mod_diff_1)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_1 > ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei > ei_mod_diff_2)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_2 > ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei > ei_mod_diff_3)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_3 > ei)
RESULT

CHECK(bool operator <= (const EnumeratorIndex& rhs) const throw(EnumeratorIndex::IncompatibleIndex))
	TEST_EQUAL(ei <= ei_equal, true)
	TEST_EQUAL(ei_equal <= ei, true)
	
	TEST_EQUAL(ei_lesser_1 <= ei, true)
	TEST_EQUAL(ei_lesser_2 <= ei, true)
	TEST_EQUAL(ei_lesser_3 <= ei, true)
	TEST_EQUAL(ei <= ei_lesser_1, false)
	TEST_EQUAL(ei <= ei_lesser_2, false)
	TEST_EQUAL(ei <= ei_lesser_2, false)

	TEST_EQUAL(ei_greater_1 <= ei, false)
	TEST_EQUAL(ei_greater_2 <= ei, false)
	TEST_EQUAL(ei_greater_3 <= ei, false)
	TEST_EQUAL(ei <= ei_greater_1, true)
	TEST_EQUAL(ei <= ei_greater_2, true)
	TEST_EQUAL(ei <= ei_greater_2, true)

	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei <= ei_lesser_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_lesser_size <= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei <= ei_greater_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_greater_size <= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei <= ei_mod_diff_1)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_1 <= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei <= ei_mod_diff_2)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_2 <= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei <= ei_mod_diff_3)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_3 <= ei)
RESULT


CHECK(bool operator >= (const EnumeratorIndex& rhs) const throw(EnumeratorIndex::IncompatibleIndex))
	TEST_EQUAL(ei >= ei_equal, true)
	TEST_EQUAL(ei_equal >= ei, true)
	
	TEST_EQUAL(ei_lesser_1 >= ei, false)
	TEST_EQUAL(ei_lesser_2 >= ei, false)
	TEST_EQUAL(ei_lesser_3 >= ei, false)
	TEST_EQUAL(ei >= ei_lesser_1, true)
	TEST_EQUAL(ei >= ei_lesser_2, true)
	TEST_EQUAL(ei >= ei_lesser_2, true)

	TEST_EQUAL(ei_greater_1 >= ei, true)
	TEST_EQUAL(ei_greater_2 >= ei, true)
	TEST_EQUAL(ei_greater_3 >= ei, true)
	TEST_EQUAL(ei >= ei_greater_1, false)
	TEST_EQUAL(ei >= ei_greater_2, false)
	TEST_EQUAL(ei >= ei_greater_2, false)

	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei >= ei_lesser_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_lesser_size >= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei >= ei_greater_size)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_greater_size >= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei >= ei_mod_diff_1)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_1 >= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei >= ei_mod_diff_2)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_2 >= ei)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei >= ei_mod_diff_3)
	TEST_EXCEPTION(EnumeratorIndex::IncompatibleIndex, ei_mod_diff_3 >= ei)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
