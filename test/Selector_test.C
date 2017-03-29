// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(Selector)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Selector* ptr;

CHECK(Selector() throw())
	ptr = new Selector;
	TEST_NOT_EQUAL(ptr, 0)
	TEST_EQUAL(ptr->getNumberOfSelectedAtoms(), 0)
RESULT


CHECK(~Selector() throw())
	delete ptr;
RESULT


CHECK(Selector(const Selector& selector) throw())
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


CHECK(Selector(const String& expression_string) throw())
	Selector s("true()");
	Expression e("true()");
	bool test = (s.getExpression() == e);
	TEST_EQUAL(test, true)
RESULT


CHECK(Processor::Result operator () (Composite& composite) throw())
	HINFile file(BALL_TEST_DATA_PATH(Selector_test.hin));
	System S;
	file >> S;
	file.close();

	HashMap<String, Size> test_expressions;
	test_expressions.insert(std::pair<String, Size>("true()", 6));
	test_expressions.insert(std::pair<String, Size>("connectedTo((-H))", 2));
	test_expressions.insert(std::pair<String, Size>("element(H)", 4));
	test_expressions.insert(std::pair<String, Size>("(element(H))", 4));
	test_expressions.insert(std::pair<String, Size>("!element(H)", 2));
	test_expressions.insert(std::pair<String, Size>("!(element(H))", 2));
	test_expressions.insert(std::pair<String, Size>("element(O)", 1));
	test_expressions.insert(std::pair<String, Size>("element(C)", 1));
	test_expressions.insert(std::pair<String, Size>("element(H) OR (name(OXT) AND chain(A))", 4));

	Selector s;
	TEST_EQUAL(s.getNumberOfSelectedAtoms(), 0)
	HashMap<String, Size>::ConstIterator exp_iterator = test_expressions.begin();
	for (; exp_iterator != test_expressions.end(); ++exp_iterator)
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
		STATUS("testing expression " << exp_iterator->first)
		TEST_EQUAL(s.getNumberOfSelectedAtoms(), counter)
	}
RESULT


CHECK(bool start() throw())
	Selector s;
	s.start();
	TEST_EQUAL(s.getNumberOfSelectedAtoms(), 0)
RESULT


CHECK(Size getNumberOfSelectedAtoms() const throw())
	HINFile file(BALL_TEST_DATA_PATH(Selector_test.hin));
	System S;
	file >> S;
	file.close();

	HashMap<String, Size> test_expressions;
	test_expressions.insert(std::pair<String, Size>("true()", 6));
	test_expressions.insert(std::pair<String, Size>("connectedTo((-H))", 2));
	test_expressions.insert(std::pair<String, Size>("element(H)", 4));
	test_expressions.insert(std::pair<String, Size>("!element(H)", 2));
	test_expressions.insert(std::pair<String, Size>("(element(H))", 4));
	test_expressions.insert(std::pair<String, Size>("!(element(H))", 2));
	test_expressions.insert(std::pair<String, Size>("(!(element(H)))", 2));
	test_expressions.insert(std::pair<String, Size>("!(!(element(H)))", 4));
	test_expressions.insert(std::pair<String, Size>("element(O)", 1));
	test_expressions.insert(std::pair<String, Size>("element(C)", 1));
	test_expressions.insert(std::pair<String, Size>("element(H) OR (name(OXT) AND chain(A))", 4));

	Selector s;
	TEST_EQUAL(s.getNumberOfSelectedAtoms(), 0)
	HashMap<String, Size>::ConstIterator exp_iterator = test_expressions.begin();
	for (; exp_iterator != test_expressions.end(); ++exp_iterator)
	{
		S.deselect();
		s.setExpression(exp_iterator->first);
		S.apply(s);
		STATUS("testing expression " << exp_iterator->first)
		TEST_EQUAL(s.getNumberOfSelectedAtoms(), exp_iterator->second);
	}
RESULT

CHECK(List& getSelectedAtoms() throw())
	HINFile file(BALL_TEST_DATA_PATH(Selector_test.hin));
	System system;
	file >> system;
	file.close();

	system.deselect();
	Selector selector("true()");
	system.apply(selector);
	list<Atom*> atom_list = selector.getSelectedAtoms();
	TEST_EQUAL(atom_list.size(), 6)
RESULT 

CHECK(Selector& operator = (const Selector& selector) throw())
	Selector s("true()");
	Selector s2;
	s2 = s;
	TEST_EQUAL(s == s2, true)
RESULT

CHECK(bool operator == (const Selector& selector) const throw())
	Selector s("true()");
	Selector s2("false()");
	TEST_EQUAL(s == s2, false)
	s2 = s;
	TEST_EQUAL(s == s2, true)
RESULT

CHECK(const Expression& getExpression() const throw())
	Selector s("true()");
	TEST_EQUAL(s.getExpression()== Expression("true()"), true)
	Selector empty;
	TEST_EQUAL(empty.getExpression() == Expression(), true);
RESULT

CHECK(void clear() throw())
	Selector s("true()");
	s.clear();
	TEST_EQUAL(s.getExpression() == Expression(), true)
RESULT

CHECK(void setExpression(const Expression& expression) throw())
	Selector s("true()");
	s.setExpression(Expression("false()"));
	TEST_EQUAL(s.getExpression() == Expression("false()"), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
