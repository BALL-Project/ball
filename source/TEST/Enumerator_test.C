// $Id: Enumerator_test.C,v 1.11.4.1 2002/06/05 00:29:02 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/enumerator.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

void char_assign(char& c1, char& c2)
{
	c1 = c2;
}

START_TEST(Enumerator, "$Id: Enumerator_test.C,v 1.11.4.1 2002/06/05 00:29:02 oliver Exp $")

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


CHECK(EnumeratorIndex::getModulus() const)
  vector<Size> modulus = enumerator_index_ptr->getModulus();
  TEST_EQUAL(modulus.size(), 0)
RESULT

CHECK(EnumeratorIndex::set(Position index))
	EnumeratorIndex ei;
RESULT


CHECK(EnumeratorIndex::EnumeratorIndex& operator ++ ())
	//????
RESULT


CHECK(EnumeratorIndex::EnumeratorIndex& operator -- ())
  //?????
RESULT


CHECK(const EnumeratorIndex& EnumeratorIndex::operator = (Position rhs))
  //?????
RESULT


CHECK(bool EnumeratorIndex::operator != (const EnumeratorIndex& rhs))
  //?????
RESULT


CHECK(bool EnumeratorIndex::operator == (const EnumeratorIndex& rhs))
  //?????
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
