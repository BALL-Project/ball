// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/expression.h>
/////////////////

using namespace BALL;

///////////////////////////

START_TEST(ExpressionPredicate)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// tests for class ExpressionPredicate::

ExpressionPredicate* ep_ptr;

CHECK(ExpressionPredicate() throw())
	ep_ptr = new ExpressionPredicate;
	TEST_NOT_EQUAL(ep_ptr, 0)
RESULT


CHECK(~ExpressionPredicate() throw())
	delete ep_ptr;
RESULT


CHECK(ExpressionPredicate(const ExpressionPredicate& predicate) throw())
	ExpressionPredicate ep1;
	String test_string("BALL argument test");
	ep1.setArgument(test_string);

	ExpressionPredicate ep2;
	bool test = (ep1 == ep2);
	TEST_NOT_EQUAL(test, true)

	ExpressionPredicate ep3(ep1);
	test = (ep1 == ep3);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionPredicate(const String& argument) throw())
	String argument("argument test");
	ExpressionPredicate ep1;
	TEST_NOT_EQUAL(ep1.getArgument(), argument)
	ExpressionPredicate ep2(argument);
	TEST_EQUAL(ep2.getArgument(), argument)
RESULT


CHECK(ExpressionPredicate& operator = (const ExpressionPredicate& predicate) throw())
	String arg("Yippieh!");
	ExpressionPredicate ep1(arg);
	ExpressionPredicate ep2;
	TEST_NOT_EQUAL(ep2.getArgument(), arg)
	ep2 = ep1;
	TEST_EQUAL(ep2.getArgument(), arg)
RESULT


CHECK(void clear() throw())
	ExpressionPredicate empty;
	ExpressionPredicate not_empty("Nonsense Argument");
	bool test = (empty == not_empty);
	TEST_NOT_EQUAL(test, true)
	not_empty.clear();
	test = (empty == not_empty);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool operator == (const ExpressionPredicate& predicate) const throw())
	String arg("BALL-Test");
	ExpressionPredicate ep1;
	ExpressionPredicate ep2;
	bool test = (ep1 == ep2);
	TEST_EQUAL(test, true)

	ep1.setArgument(arg);
	test = (ep1 == ep2);
	TEST_NOT_EQUAL(test, true)

	ep2.setArgument(arg);
	test = (ep1 == ep2);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool operator () (const Atom& atom) const throw())
	Atom atom;
	ExpressionPredicate ep;
	TEST_EQUAL(ep.operator()(atom), true)
RESULT


CHECK(void setArgument(const String& argument) throw())
	String arg = "BALL-test";
	ExpressionPredicate ep;
	TEST_NOT_EQUAL(ep.getArgument(), arg)
	ep.setArgument(arg);
	TEST_EQUAL(ep.getArgument(), arg)
RESULT


CHECK(const String& getArgument() const throw())
	String arg = "BALL-test";
	ExpressionPredicate ep;
	TEST_NOT_EQUAL(ep.getArgument(), arg)
	ep.setArgument(arg);
	TEST_EQUAL(ep.getArgument(), arg)
RESULT

CHECK(BALL_CREATE(ExpressionPredicate))
	ExpressionPredicate ep1, empty;
	String test_string("BALL argument test");
	ep1.setArgument(test_string);

	ExpressionPredicate* e = (ExpressionPredicate*) ep1.create(false,true);
	TEST_NOT_EQUAL(e, 0)
	TEST_NOT_EQUAL(e, &ep1)
	TEST_EQUAL(*e == empty, true)
	delete e;

	e = (ExpressionPredicate*) ep1.create(true, false);
	TEST_NOT_EQUAL(e, 0)
	TEST_NOT_EQUAL(e, &ep1)
	TEST_EQUAL(*e == ep1, true)
	delete e;
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
