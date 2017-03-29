// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <list>

/////////////////

using namespace BALL;

///////////////////////////

START_TEST(Expression)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// Predicate for testing

class MickeyPredicate
	: public  ExpressionPredicate
{
	public:

	BALL_CREATE(MickeyPredicate)
	virtual bool operator () (const Atom& atom) const
		throw()
	{
		return (atom.getElement().getSymbol() == "H");
	}
};

String mickey_predicate_string("isMickeyMouse");

// tests for class Expression::

Expression* e_ptr = 0;

CHECK(Expression() throw())
	e_ptr = new Expression;
	TEST_NOT_EQUAL(e_ptr, 0)
	TEST_EQUAL(e_ptr->getCreationMethods().size(), 27)
RESULT


CHECK(~Expression() throw())
	delete e_ptr;
RESULT


CHECK(Expression(const Expression& expression) throw())
	String expression("connectedTo((-H))");
	Expression e1(expression);
	Expression e2;

	bool test = (e1 == e2);
	TEST_NOT_EQUAL(test, true)

	Expression e3(e1);
	test = (e1 == e3);
	TEST_EQUAL(test, true)
RESULT


CHECK(Expression(const String& expression_string) throw(Exception::ParseError))
	Expression e("true()");
	TEST_EQUAL(e.getCreationMethods().size(), 27)
	TEST_EQUAL(e(Atom()), true)
RESULT


CHECK(bool hasPredicate(const String& name) const throw())
	Expression e;
	e.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	TEST_EQUAL(e.hasPredicate("You don't have this predicate"), false)
	TEST_EQUAL(e.hasPredicate(mickey_predicate_string), true)
RESULT


CHECK(bool operator == (const Expression& expression) const throw())
	Expression e1;
	Expression e2;
	bool test = (e1 == e2);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool operator () (const Atom& atom) const throw())
	PDBFile file(BALL_TEST_DATA_PATH(Expression_test.pdb));
	System S;
	file >> S;
	HashMap<String, Size> test_expressions;
	
	test_expressions.insert(std::pair<String, Size>("true()", 76));
	test_expressions.insert(std::pair<String, Size>("false()", 0));
	test_expressions.insert(std::pair<String, Size>("element(H) AND connectedTo((C))", 24));
	test_expressions.insert(std::pair<String, Size>("element(N) AND connectedTo((C)) AND connectedTo((H))", 10));
	test_expressions.insert(std::pair<String, Size>("connectedTo((H))", 24));
	test_expressions.insert(std::pair<String, Size>("connectedTo((O))", 6));
	test_expressions.insert(std::pair<String, Size>("connectedTo((H)(H))", 14));
	test_expressions.insert(std::pair<String, Size>("connectedTo(C(H)(H)(H))", 0));
	test_expressions.insert(std::pair<String, Size>("element(H)", 38));
	test_expressions.insert(std::pair<String, Size>("!element(H)", 38));
	test_expressions.insert(std::pair<String, Size>("element(O)", 6));
	test_expressions.insert(std::pair<String, Size>("element(C)", 22));
	test_expressions.insert(std::pair<String, Size>("!element(C)", 54));
	test_expressions.insert(std::pair<String, Size>("((element(C)))", 22));
	test_expressions.insert(std::pair<String, Size>("!(element(C))", 54));
	test_expressions.insert(std::pair<String, Size>("(!element(C))", 54));
	test_expressions.insert(std::pair<String, Size>("!(!element(C))", 22));
	test_expressions.insert(std::pair<String, Size>("element(H) OR (name(CA) AND chain(A))", 40));
	test_expressions.insert(std::pair<String, Size>("inRing()", 10));

	Expression e;
	Size counter;
	HashMap<String, Size>::ConstIterator exp_iterator = test_expressions.begin();
	for (; exp_iterator !=test_expressions.end(); ++exp_iterator)
	{
		counter = 0;
		e.setExpression(exp_iterator->first);
		for (AtomIterator it = S.beginAtom(); +it; ++it)
		{
			if (e.operator () (*it)) counter++;
		}
		STATUS("testing expression " << exp_iterator->first)
		TEST_EQUAL(counter, exp_iterator->second);
	}

	Atom dummy;
	TEST_EQUAL(Expression("true() AND true()")(dummy), true)
	TEST_EQUAL(Expression("true() AND false()")(dummy), false)
	TEST_EQUAL(Expression("false() AND true()")(dummy), false)
	TEST_EQUAL(Expression("false() AND false()")(dummy), false)
	TEST_EQUAL(Expression("true() OR false()")(dummy), true)
	TEST_EQUAL(Expression("true() OR true()")(dummy), true)
	TEST_EQUAL(Expression("false() OR false()")(dummy), false)
	TEST_EQUAL(Expression("false() OR true()")(dummy), true)
	// triple OR
	TEST_EQUAL(Expression("false() OR false() OR false()")(dummy), false)
	TEST_EQUAL(Expression("false() OR false() OR true()")(dummy), true)
	TEST_EQUAL(Expression("false() OR true() OR false()")(dummy), true)
	TEST_EQUAL(Expression("true() OR false() OR false()")(dummy), true)
	TEST_EQUAL(Expression("true() OR true() OR true()")(dummy), true)
	// triple AND
	TEST_EQUAL(Expression("false() AND false() AND false()")(dummy), false)
	TEST_EQUAL(Expression("false() AND false() AND true()")(dummy), false)
	TEST_EQUAL(Expression("false() AND true() AND false()")(dummy), false)
	TEST_EQUAL(Expression("true() AND false() AND false()")(dummy), false)
	TEST_EQUAL(Expression("true() AND true() AND true()")(dummy), true)
	// AND/OR
	TEST_EQUAL(Expression("false() OR (false() AND false())")(dummy), false)
	TEST_EQUAL(Expression("false() OR (false() AND true())")(dummy), false)
	TEST_EQUAL(Expression("false() OR (true() AND false())")(dummy), false)
	TEST_EQUAL(Expression("false() OR (true() AND true())")(dummy), true)
	TEST_EQUAL(Expression("true() OR (false() AND false())")(dummy), true)
	TEST_EQUAL(Expression("true() OR (false() AND true())")(dummy), true)
	TEST_EQUAL(Expression("true() OR (true() AND false())")(dummy), true)
	TEST_EQUAL(Expression("true() OR (true() AND true())")(dummy), true)
	// OR/AND
	TEST_EQUAL(Expression("false() AND (false() OR false())")(dummy), false)
	TEST_EQUAL(Expression("false() AND (false() OR true())")(dummy), false)
	TEST_EQUAL(Expression("false() AND (true() OR false())")(dummy), false)
	TEST_EQUAL(Expression("false() AND (true() OR true())")(dummy), false)
	TEST_EQUAL(Expression("true() AND (false() OR false())")(dummy), false)
	TEST_EQUAL(Expression("true() AND (false() OR true())")(dummy), true)
	TEST_EQUAL(Expression("true() AND (true() OR false())")(dummy),true)
	TEST_EQUAL(Expression("true() AND (true() OR true())")(dummy), true)
RESULT


CHECK(ExpressionPredicate* getPredicate(const String& name, const String& args = "") const throw())
	Expression e;
	e.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	ExpressionPredicate* ep1 = e.getPredicate(mickey_predicate_string);
	ExpressionPredicate* ep2 
		= (ExpressionPredicate*)MickeyPredicate::createDefault();
	bool test = (*ep1 == *ep2);
	TEST_EQUAL(test, true)
	delete ep1;
	delete ep2;
RESULT


CHECK(void registerPredicate(const String& name, CreationMethod creation_method) throw())
	Expression e;
	e.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	ExpressionPredicate* ep1 = e.getPredicate(mickey_predicate_string);
	ExpressionPredicate* ep2 
		= (ExpressionPredicate*)MickeyPredicate::createDefault();
	bool test = (*ep1 == *ep2);
	TEST_EQUAL(test, true)
	delete ep1;
	delete ep2;
RESULT


CHECK(void setExpression(const String& expression) throw(Exception::ParseError))
	String test_expression("connectedTo((-H))");
	Expression e;
	e.setExpression(test_expression);
	TEST_EQUAL(e.getExpressionString(), "connectedTo((-H))")
RESULT


CHECK(const String& getExpressionString() const throw())
	Expression e("(connectedTo((-H)))");
	TEST_EQUAL(e.getExpressionString(), "(connectedTo((-H)))")
RESULT


CHECK(const ExpressionTree* getExpressionTree() const throw())
	Expression e("connectedTo((-H))");
	const ExpressionTree* tree = e.getExpressionTree();
	TEST_NOT_EQUAL(tree, 0)
RESULT


CHECK(Expression& operator = (const Expression& expression) throw())
	Expression e1("connectedTo((-H))");
	Expression e2;
	bool test = (e1 == e2);
	TEST_NOT_EQUAL(test, true)

	e2 = e1;
	test = (e1 == e2);
	TEST_EQUAL(test, true)
RESULT


CHECK(void clear() throw())
	Expression empty;
	Expression nonempty("connectedTo((-H))");
	bool test = (empty == nonempty);
	TEST_NOT_EQUAL(test, true)

	nonempty.clear();
	test = (empty == nonempty);
	TEST_EQUAL(test, true)
RESULT


CHECK(BALL_CREATE(Expression))
	Expression empty;
	Expression nonempty("connectedTo((-H))");
	Expression* e = (Expression*) nonempty.create(false, true);
	TEST_NOT_EQUAL(e, 0)
	TEST_NOT_EQUAL(e, &nonempty)
	TEST_EQUAL(*e == empty, true)
	delete e;
	e = (Expression*) nonempty.create(true, false);
	TEST_EQUAL(*e == nonempty, true)
	TEST_NOT_EQUAL(e, &nonempty)
	delete e;
RESULT

CHECK(const StringHashMap<CreationMethod>& getCreationMethods() const throw())
	Expression nonempty("connectedTo((-H))");
	TEST_EQUAL(nonempty.getCreationMethods().size() > 0, true)

	Expression empty;
	TEST_EQUAL(empty.getCreationMethods().size() > 0, true)
RESULT

CHECK(typedefvoid* (*CreationMethod)())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
