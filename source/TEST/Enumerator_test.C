// $Id: Enumerator_test.C,v 1.4 2001/07/13 20:47:31 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/enumerator.h>
#include <BALL/DATATYPE/string.h>

///////////////////////////

void char_assign(char& c1, char& c2)
{
	c1 = c2;
}

START_TEST(Enumerator, "$Id: Enumerator_test.C,v 1.4 2001/07/13 20:47:31 oliver Exp $")

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
  //BAUSTELLE
RESULT


CHECK(EnumeratorIndex& operator -- ())
  //BAUSTELLE
RESULT


CHECK(set(Position index))
  //BAUSTELLE
RESULT


CHECK(EnumeratorIndex& operator = (Position index))
  //BAUSTELLE
RESULT


CHECK(bool operator == (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //BAUSTELLE
RESULT


CHECK(bool operator != (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //BAUSTELLE
RESULT


CHECK(bool operator < (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //BAUSTELLE
RESULT


CHECK(bool operator > (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //BAUSTELLE
RESULT


CHECK(bool operator <= (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //BAUSTELLE
RESULT


CHECK(bool operator >= (const EnumeratorIndex& x, const EnumeratorIndex& y))
  //BAUSTELLE
RESULT


// tests for class Enumerator::

CHECK(Enumerator::(*MutatorFunction)(Variant&, Variant&))
  //BAUSTELLE
RESULT

Enumerator<string, string::iterator, char>* enumerator_ptr;

CHECK(Enumerator())
  enumerator_ptr = new Enumerator<string, string::iterator, char>;
	TEST_NOT_EQUAL(enumerator_ptr, 0);
RESULT
	

CHECK(~Enumerator())
  delete enumerator_ptr;
RESULT

String s;
Enumerator<String, String::iterator, char> enumerator(s, char_assign);
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
enumerator.addVariants(it, variants);
it++;
enumerator.addVariants(it, variants);
it++;
enumerator.addVariants(it, variants);
it++;
enumerator.addVariants(it, variants);
it++;
enumerator.addVariants(it, variants);

CHECK(countVariants())
  TEST_EQUAL(enumerator.countVariants(), 1e5);
RESULT

String S;
CHECK(createPermutation())
	for (Position i = 0; i < enumerator.countVariants(); i++) 
	{
		enumerator.createPermutation(i); 
		S = String(i);
		S.reverse();
		TEST_EQUAL(enumerator.getCurrent(), s)
	}
RESULT


// tests for class IteratorTraits_::

CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::IteratorTraits_())
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::IteratorTraits_(const ContainerType& enumerator))
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::IteratorTraits_(const IteratorTraits_& traits))
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::IteratorTraits_& operator = (const IteratorTraits_& traits))
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::getContainer())
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::getPosition())
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::bool operator == (const IteratorTraits_& traits) const )
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::isValid() const )
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::toBegin())
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::toEnd())
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::getData())
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::Container::VariantIterator::Variant::forward())
  //BAUSTELLE
RESULT


CHECK(IteratorTraits_::Enumerator::mutate_(VariantIterator& it, Variant& v))
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
