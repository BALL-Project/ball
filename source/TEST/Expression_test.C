// $Id: Expression_test.C,v 1.1 2000/05/19 15:58:33 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(Expression, "$Id: Expression_test.C,v 1.1 2000/05/19 15:58:33 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class ExpressionPredicate::

ExpressionPredicate* pred;
CHECK(ExpressionPredicate::ExpressionPredicate())
  pred = new ExpressionPredicate;
	TEST_NOT_EQUAL(pred, 0)
RESULT

CHECK(ExpressionPredicate::~ExpressionPredicate())
  delete pred;
RESULT

CHECK(ExpressionPredicate::ExpressionPredicate(const String& argument))
  pred = new ExpressionPredicate("element(H)");
	TEST_NOT_EQUAL(pred, 0)
	delete pred;
	pred = new ExpressionPredicate("");
	TEST_NOT_EQUAL(pred, 0)
	delete pred;
RESULT

CHECK(ExpressionPredicate::bool operator () (const Atom& atom) const )
	// abstract method
RESULT


CHECK(ExpressionPredicate::setArgument(const String& argument))
  ExpressionPredicate pred;
	pred.setArgument("test");
RESULT


// tests for class ExpressionTree

ExpressionTree* e_tree;
CHECK(ExpressionTree())
	e_tree = new ExpressionTree;
	TEST_NOT_EQUAL(e_tree, 0)
RESULT

CHECK(~ExpressionTree())
  delete e_tree;
RESULT

CHECK(ExpressionTree(ExpressionPredicate* predicate, bool negate = false))
  //BAUSTELLE
RESULT


CHECK(ExpressionTree(Type type, list<ExpressionTree*> children, bool negate = false))
  //BAUSTELLE
RESULT




CHECK(bool operator () (const Atom& atom) const )
  //BAUSTELLE
RESULT


CHECK(setType(Type type))
  //BAUSTELLE
RESULT


CHECK(setNegate(bool negate))
  //BAUSTELLE
RESULT


CHECK(setPredicate(ExpressionPredicate* predicate))
  //BAUSTELLE
RESULT


CHECK(appendChild(ExpressionTree* child))
  //BAUSTELLE
RESULT


// tests for class SyntaxTree::

CHECK(SyntaxTree::SyntaxTree())
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::SyntaxTree(const String& expression))
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::~SyntaxTree())
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::begin())
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::end())
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::begin() const )
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::end() const )
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::mergeLeft(SyntaxTree* tree))
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::mergeRight(SyntaxTree* tree))
  //BAUSTELLE
RESULT


CHECK(SyntaxTree::parse())
  //BAUSTELLE
RESULT


// tests for class Expression::

CHECK(Expression::(*CreationMethod)())
  //BAUSTELLE
RESULT


CHECK(Expression::Expression())
  //BAUSTELLE
RESULT


CHECK(Expression::Expression(const Expression& expression))
  //BAUSTELLE
RESULT


CHECK(Expression::Expression(const String& expression_string))
  //BAUSTELLE
RESULT


CHECK(Expression::~Expression())
  //BAUSTELLE
RESULT


CHECK(Expression::hasPredicate(const String& name))
  //BAUSTELLE
RESULT


CHECK(Expression::bool operator () (const Atom& atom) const )
  //BAUSTELLE
RESULT


CHECK(Expression::getPredicate(const String& name, const String& args = "") const )
  //BAUSTELLE
RESULT


CHECK(Expression::registerPredicate(const String& name, CreationMethod creation_method))
  //BAUSTELLE
RESULT


CHECK(Expression::setExpression(const String& expression))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
