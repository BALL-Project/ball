// $Id: ExpressionParser_test.C,v 1.4 2002/01/28 00:43:55 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/expressionParser.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <list>

/////////////////

using namespace BALL;

///////////////////////////

START_TEST(ExpressionParser, "$Id: ExpressionParser_test.C,v 1.4 2002/01/28 00:43:55 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK(ExpressionParser::parse(const String& s))
	ExpressionParser parser;
	parser.parse("pases('(H2)') AND element('H'))");
	parser.getSyntaxTree().dump();
RESULT

// tests for class SyntaxTree::

typedef ExpressionParser::SyntaxTree SyntaxTree;
SyntaxTree* st_ptr = 0;

CHECK(SyntaxTree::SyntaxTree() throw())
	st_ptr = new SyntaxTree;
	TEST_NOT_EQUAL(st_ptr, 0)
	TEST_EQUAL(st_ptr->expression, "")
	TEST_EQUAL(st_ptr->argument, "")
	TEST_EQUAL(st_ptr->evaluated, false)
	TEST_EQUAL(st_ptr->negate, false)
	TEST_EQUAL(st_ptr->type, ExpressionTree::INVALID)
	list<SyntaxTree*> children;
	bool test = (st_ptr->children == children);
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::~SyntaxTree() throw())
	delete st_ptr;
RESULT

CHECK(SyntaxTree::begin() throw())
	SyntaxTree* child1 = new SyntaxTree;
	SyntaxTree* child2 = new SyntaxTree;
	SyntaxTree* child3 = new SyntaxTree;
	list<SyntaxTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	SyntaxTree st;
	SyntaxTree::Iterator test_it = st.begin();
	bool test = (*test_it == child1);
	TEST_NOT_EQUAL(test, true)

	st.children = children;
	test_it = st.begin();
	test = (*test_it == child1);
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::end() throw())
	SyntaxTree* child1 = new SyntaxTree;
	SyntaxTree* child2 = new SyntaxTree;
	SyntaxTree* child3 = new SyntaxTree;
	list<SyntaxTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	SyntaxTree st;
	SyntaxTree::Iterator test_it = st.end();

	// ?????
	// Dunno what's happening here: test_it seems to be glued to child2.
	st.children = children;
	test_it = --st.end();
	bool test = (*test_it == child3);
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::begin() const  throw())
	SyntaxTree* child1 = new SyntaxTree;
	SyntaxTree* child2 = new SyntaxTree;
	SyntaxTree* child3 = new SyntaxTree;
	list<SyntaxTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	SyntaxTree st;
	SyntaxTree::ConstIterator test_it = st.begin();
	bool test = (test_it == st.end());
	TEST_EQUAL(test, true)

	st.children = children;
	test_it = st.begin();
	test = (*test_it == child1);
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::end() const  throw())
	SyntaxTree* child1 = new SyntaxTree;
	SyntaxTree* child2 = new SyntaxTree;
	SyntaxTree* child3 = new SyntaxTree;
	list<SyntaxTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	SyntaxTree st;
	SyntaxTree::ConstIterator test_it = st.end();

	st.children = children;
	test_it = --st.end();
	bool test = (*test_it == child3);
	TEST_EQUAL(test, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
