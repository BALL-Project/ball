// $Id: Enumerator_test.C,v 1.11.4.2 2002/06/05 22:52:41 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/enumerator.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

void char_assign(char& c1, char& c2)
{
	c1 = c2;
}

START_TEST(Enumerator, "$Id: Enumerator_test.C,v 1.11.4.2 2002/06/05 22:52:41 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class EnumeratorIndex::IncompatibleIndex::

EnumeratorIndex::IncompatibleIndex* ex_ptr;

CHECK(EnumeratorIndex::IncompatibleIndex::IncompatibleIndex(const char* file, int line))
	ex_ptr = new EnumeratorIndex::IncompatibleIndex(__FILE__,__LINE__);
	TEST_NOT_EQUAL(ex_ptr, 0)
RESULT

delete ex_ptr;

// tests for class EnumeratorIndex::

EnumeratorIndex* enumerator_index_ptr;

CHECK(EnumeratorIndex::EnumeratorIndex())
  enumerator_index_ptr = new EnumeratorIndex;
	TEST_NOT_EQUAL(enumerator_index_ptr, 0)
RESULT

CHECK(EnumeratorIndex::~EnumeratorIndex())
  delete enumerator_index_ptr;
RESULT

Enumerator<String, String::iterator, char>::SiteList variant_list;
CHECK(EnumeratorIndex::EnumeratorIndex(const list<pair<VariantIterator, vector<Variant> > >& variant_list))
  enumerator_index_ptr = new EnumeratorIndex(variant_list);
  TEST_NOT_EQUAL(enumerator_index_ptr, 0)
RESULT

CHECK(Size EnumeratorIndex::getSize() const)
	EnumeratorIndex ei;
	TEST_EQUAL(ei.getSize(), 0)
RESULT

CHECK(EnumeratorIndex::getModulus() const)
  vector<Size> modulus = enumerator_index_ptr->getModulus();
  TEST_EQUAL(modulus.size(), 0)
RESULT

CHECK(EnumeratorIndex& EnumeratorIndex::operator << (Size modulus))
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

CHECK(Position EnumeratorIndex::operator [] (Position pos) const)
	EnumeratorIndex ei;
	ei << 5 << 6 << 7;

	const EnumeratorIndex& c_ei(ei);
	TEST_EQUAL(c_ei[0], 0)
	TEST_EQUAL(c_ei[1], 0)
	TEST_EQUAL(c_ei[2], 0)
RESULT

CHECK(Position& EnumeratorIndex::operator [] (Position pos))
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

CHECK(Size EnumeratorIndex::getModulus(Position pos) const)
	EnumeratorIndex ei;
	ei << 5 << 6 << 7;
RESULT

CHECK(EnumeratorIndex::operator = (Position index))
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

CHECK(EnumeratorIndex::EnumeratorIndex& operator ++ ())
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


CHECK(EnumeratorIndex::EnumeratorIndex& operator -- ())
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


CHECK(const EnumeratorIndex& EnumeratorIndex::operator = (Position rhs))
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

CHECK(bool EnumeratorIndex::operator == (const EnumeratorIndex& rhs))
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


CHECK(bool EnumeratorIndex::operator != (const EnumeratorIndex& rhs))
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

CHECK(bool EnumeratorIndex::operator < (const EnumeratorIndex& rhs))
  //?????
RESULT


CHECK(bool EnumeratorIndex::operator > (const EnumeratorIndex& rhs))
  //?????
RESULT

CHECK(bool EnumeratorIndex::operator <= (const EnumeratorIndex& rhs))
  //?????
RESULT


CHECK(bool EnumeratorIndex::operator >= (const EnumeratorIndex& rhs))
  //?????
RESULT


// tests for class Enumerator::

Enumerator<string, string::iterator, char>* enumerator_ptr;
CHECK(Enumerator())
  enumerator_ptr = new Enumerator<string, string::iterator, char>;
	TEST_NOT_EQUAL(enumerator_ptr, 0);
RESULT
	

CHECK(~Enumerator())
  delete enumerator_ptr;
RESULT

String s;
CHECK(countVariants())
	Enumerator<String, String::iterator, char> enumerator(s, char_assign);	
  TEST_EQUAL(enumerator.countVariants(), 1);
RESULT

s = "12345";
Enumerator<String, String::iterator, char> enumerator(s, char_assign);
CHECK(addVariants(VariantIterator& it, const VariantVector& variants))
	vector<char> variants; 
	variants.push_back('0');
	variants.push_back('1');
	variants.push_back('2');
	variants.push_back('3');
	variants.push_back('4');
	variants.push_back('5');
	variants.push_back('6');
	variants.push_back('7');
	variants.push_back('8');
	variants.push_back('9');
	String::iterator it = s.begin();
	TEST_EQUAL(enumerator.countVariants(), 1)
	enumerator.addVariants(it, variants);
	TEST_EQUAL(enumerator.countVariants(), 10)
	it++;
	enumerator.addVariants(it, variants);
	TEST_EQUAL(enumerator.countVariants(), 100)
	it++;
	enumerator.addVariants(it, variants);
	TEST_EQUAL(enumerator.countVariants(), 1000)
	it++;
	enumerator.addVariants(it, variants);
	TEST_EQUAL(enumerator.countVariants(), 10000)
	it++;
	enumerator.addVariants(it, variants);
	TEST_EQUAL(enumerator.countVariants(), 100000)
RESULT

CHECK(createPermutation(Position i))
	for (Position i = 0; i < enumerator.countVariants(); i++) 
	{
		enumerator.createPermutation(i); 
		String p = s;
		p.reverse();
		TEST_EQUAL(p.toUnsignedInt(), i)
	}
RESULT

CHECK(deleteVariants(VariantIterator& it, const VariantVector& variants))
	vector<char> variants; 
	variants.push_back('0');
	variants.push_back('1');
	variants.push_back('2');
	variants.push_back('3');
	variants.push_back('4');
	variants.push_back('5');
	variants.push_back('6');
	variants.push_back('7');
	variants.push_back('8');
	variants.push_back('9');
	enumerator.deleteVariants(s.begin(), variants);	
	TEST_EQUAL(enumerator.countVariants(), 10000)
	s[0] = 'X';
	for (Position i = 0; i < enumerator.countVariants(); i++) 
	{
		enumerator.createPermutation(i); 
		TEST_EQUAL(s[0], 'X')
		String p = s(1);
		p.reverse();
		TEST_EQUAL(p.toUnsignedInt(), i)
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
