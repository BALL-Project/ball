// $Id: Selector_test.C,v 1.5 2001/07/15 17:32:41 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(Selector, "$Id: Selector_test.C,v 1.5 2001/07/15 17:32:41 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

Selector* ptr;

CHECK(Selector::Selector() throw())
	ptr = new Selector;
	TEST_NOT_EQUAL(ptr, 0)
	TEST_EQUAL(ptr->getNumberOfSelectedAtoms(), 0)
RESULT


CHECK(Selector::~Selector() throw())
	delete ptr;
RESULT


CHECK(Selector::Selector(const Selector& selector) throw())
	Selector s1;
	Expression e("true()");
	s1.setExpression(e);
	Selector s2;
	bool test = (s2.getExpression() == e);
	TEST_NOT_EQUAL(test, true)

	Selector s3(s1);
	test = (s3.getExpression() == e);
	TEST_EQUAL(test, true)
RESULT


CHECK(Selector::Selector(const String& expression_string) throw())
	Selector s("true()");
	Expression e("true()");
	bool test = (s.getExpression() == e);
	TEST_EQUAL(test, true)
RESULT


CHECK(Selector::Processor::Result operator () (Composite& composite) throw())
	HINFile file("data/Expression_test.hin");
	System S;
	file.read(S);
	file.close();

	HashMap<String, Size> test_expressions;
	test_expressions.insert(pair<String, Size>("true()", 6));
	// BAUSTELLE
	// test_expressions.insert(pair<String, Size>("connectedTo(H)", 2));
	test_expressions.insert(pair<String, Size>("element(H)", 4));
	test_expressions.insert(pair<String, Size>("element(O)", 1));
	test_expressions.insert(pair<String, Size>("element(C)", 1));
	test_expressions.insert(pair<String, Size>("element(H) OR (name(OXT) AND chain(A))", 0));

	Selector s;
	TEST_EQUAL(s.getNumberOfSelectedAtoms(), 0)
	HashMap<String, Size>::ConstIterator exp_iterator = test_expressions.begin();
	for (; +exp_iterator; ++exp_iterator)
	{
		S.deselect();
		s.setExpression(exp_iterator->first);
		S.apply(s);
		TEST_EQUAL(s.getNumberOfSelectedAtoms(), exp_iterator->second);
		Size counter = 0;
		for (AtomIterator it = S.beginAtom(); +it; ++it)
		{
			if (it->isSelected())
			{
				counter++;
			}
		}
		TEST_EQUAL(s.getNumberOfSelectedAtoms(), counter)
	}
RESULT


CHECK(Selector::start() throw())
	Selector s;
	s.start();
	TEST_EQUAL(s.getNumberOfSelectedAtoms(), 0)
RESULT


CHECK(Selector::getNumberOfSelectedAtoms() const  throw())
	HINFile file("data/Expression_test.hin");
	System S;
	file.read(S);
	file.close();

	HashMap<String, Size> test_expressions;
	test_expressions.insert(pair<String, Size>("true()", 6));
	// BAUSTELLE
	// test_expressions.insert(pair<String, Size>("connectedTo(H)", 2));
	test_expressions.insert(pair<String, Size>("element(H)", 4));
	test_expressions.insert(pair<String, Size>("element(O)", 1));
	test_expressions.insert(pair<String, Size>("element(C)", 1));
	test_expressions.insert(pair<String, Size>("element(H) OR (name(OXT) AND chain(A))", 0));

	Selector s;
	TEST_EQUAL(s.getNumberOfSelectedAtoms(), 0)
	HashMap<String, Size>::ConstIterator exp_iterator = test_expressions.begin();
	for (; +exp_iterator; ++exp_iterator)
	{
		S.deselect();
		s.setExpression(exp_iterator->first);
		S.apply(s);
		TEST_EQUAL(s.getNumberOfSelectedAtoms(), exp_iterator->second);
	}
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
