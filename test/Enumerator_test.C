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

Enumerator<string, string::iterator, char>* enumerator_ptr;
CHECK(Enumerator())
  enumerator_ptr = new Enumerator<string, string::iterator, char>;
	TEST_NOT_EQUAL(enumerator_ptr, 0);
RESULT

CHECK(~Enumerator())
  delete enumerator_ptr;
RESULT

String s;
CHECK(Size countVariants())
	Enumerator<String, String::iterator, char> enumerator(s, char_assign);	
  TEST_EQUAL(enumerator.countVariants(), 1);
RESULT

s = "12345";
Enumerator<String, String::iterator, char> enumerator(s, char_assign);
CHECK(void addVariants(const SiteIterator& it, const VariantVector& variants))
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

CHECK(void createCombination(const Position index) throw(Exception::IndexOverflow))
	for (Position i = 0; i < enumerator.countVariants(); i++) 
	{
		enumerator.createCombination(i); 
		String p = s;
		p.reverse();
		TEST_EQUAL(p.toUnsignedInt(), i)
	}
RESULT

CHECK(void createCombination(const EnumeratorIndex& index) throw(EnumeratorIndex::IncompatibleIndex))
	for (Position i = 0; i < enumerator.countVariants(); i++) 
	{
		enumerator.createCombination(i); 
		String p = s;
		p.reverse();
		TEST_EQUAL(p.toUnsignedInt(), i)
	}
RESULT

CHECK(void deleteVariants(const SiteIterator& it, const VariantVector& variants))
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
		enumerator.createCombination(i); 
		TEST_EQUAL(s[0], 'X')
		String p = s(1);
		p.reverse();
		TEST_EQUAL(p.toUnsignedInt(), i)
	}
RESULT

CHECK(Iterator begin())
	Enumerator<String, String::iterator, char>::Iterator it = enumerator.begin();
	TEST_EQUAL(it.isValid(), true)
RESULT

CHECK(Iterator end())
	Size counter = 0;
	Enumerator<String, String::iterator, char>::Iterator it = enumerator.begin();
	for ( ; it != enumerator.end(); ++it, counter++)  ; // just count
	TEST_EQUAL(enumerator.countVariants(), 10000);
	
	TEST_EQUAL(counter, 10000)
RESULT

CHECK(ConstIterator begin() const )
	Enumerator<String, String::iterator, char>& c_enumerator(enumerator);
	Enumerator<String, String::iterator, char>::ConstIterator it = c_enumerator.begin();
	TEST_EQUAL(it.isValid(), true)
RESULT

CHECK(ConstIterator end() const )
	Size counter = 0;
	Enumerator<String, String::iterator, char>& c_enumerator(enumerator);
	Enumerator<String, String::iterator, char>::ConstIterator it = c_enumerator.begin();
	for ( ; it != c_enumerator.end(); ++it, counter++)  ; // just count
	TEST_EQUAL(counter, 10000)
RESULT

CHECK([EXTRA] Enumerator iteration)
	Size counter = 0;
	Enumerator<String, String::iterator, char>::Iterator it = enumerator.begin();
	for ( ; it != enumerator.end(); ++it, counter++)
	{
		String p = (*it)(1);
		p.reverse();
		TEST_EQUAL(p.toUnsignedInt(), counter)
	}
RESULT

CHECK(Container& getCurrent())
  // ???
RESULT

CHECK(Enumerator(Container& container))
  // ???
RESULT

CHECK(Enumerator(Container& container, MutatorFunction mutator))
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
