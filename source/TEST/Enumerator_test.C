// $Id: Enumerator_test.C,v 1.8 2002/01/04 03:09:14 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/enumerator.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

void char_assign(char& c1, char& c2)
{
	c1 = c2;
}

START_TEST(Enumerator, "$Id: Enumerator_test.C,v 1.8 2002/01/04 03:09:14 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class EnumeratorIndex::IncompatibleIndex::

EnumeratorIndex::IncompatibleIndex* ex_ptr;

CHECK(EnumeratorIndex::IncompatibleIndex(const char* file, int line))
	ex_ptr = new EnumeratorIndex::IncompatibleIndex(__FILE__,__LINE__);
	TEST_NOT_EQUAL(ex_ptr, 0)
RESULT

delete ex_ptr;

// tests for class EnumeratorIndex::

EnumeratorIndex* enumerator_index_ptr;

CHECK(EnumeratorIndex())
  enumerator_index_ptr = new EnumeratorIndex;
	TEST_NOT_EQUAL(enumerator_index_ptr, 0)
RESULT

CHECK(~EnumeratorIndex())
  delete enumerator_index_ptr;
RESULT

Enumerator<String, String::iterator, char>::SiteList variant_list;
CHECK(EnumeratorIndex(const list<pair<VariantIterator, vector<Variant> > >& variant_list))
  enumerator_index_ptr = new EnumeratorIndex(variant_list);
  TEST_NOT_EQUAL(enumerator_index_ptr, 0)
RESULT


CHECK(getModulus() const )
  vector<Size> modulus = enumerator_index_ptr->getModulus();
  TEST_EQUAL(modulus.size(), 0)
RESULT

CHECK(EnumeratorIndex& operator ++ ())
	//????
RESULT


CHECK(EnumeratorIndex& operator -- ())
  //?????
RESULT


CHECK(set(Position index))
  //?????
RESULT


CHECK(EnumeratorIndex& operator = (Position index))
  //?????
RESULT


CHECK(bool operator == (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //?????
RESULT


CHECK(bool operator != (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //?????
RESULT


CHECK(bool operator < (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //?????
RESULT


CHECK(bool operator > (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //?????
RESULT


CHECK(bool operator <= (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //?????
RESULT


CHECK(bool operator >= (const EnumeratorIndex& x, const EnumeratorIndex& y))
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

Enumerator<String, String::iterator, char> enumerator(s, char_assign);
CHECK(addVariant())
	String s;
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
	string::iterator it = s.begin();
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

String S;
CHECK(createPermutation(Position i))
	for (Position i = 0; i < enumerator.countVariants(); i++) 
	{
		enumerator.createPermutation(i); 
		S = String(i);
		S.reverse();
		TEST_EQUAL(enumerator.getCurrent(), s)
	}
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
