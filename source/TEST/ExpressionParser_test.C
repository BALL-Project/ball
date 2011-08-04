// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

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

START_TEST(ExpressionParser)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// tests for class SyntaxTree::

typedef ExpressionParser::SyntaxTree SyntaxTree;
SyntaxTree* st_ptr = 0;

CHECK(SyntaxTree() throw())
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


CHECK(~SyntaxTree() throw())
	delete st_ptr;
RESULT

CHECK(Iterator begin() throw())
	SyntaxTree* child1 = new SyntaxTree;
	SyntaxTree* child2 = new SyntaxTree;
	SyntaxTree* child3 = new SyntaxTree;
	list<SyntaxTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	SyntaxTree st;
	st.children = children;
	SyntaxTree::Iterator test_it = st.begin();
	SyntaxTree::Iterator test_it2 = st.children.begin();
	bool test = (&*test_it == &*test_it2);
	TEST_EQUAL(test, true)


	test_it = st.begin();
	test = (*test_it == child1);
	TEST_EQUAL(test, true)
RESULT


CHECK(Iterator end() throw())
	SyntaxTree* child1 = new SyntaxTree;
	SyntaxTree* child2 = new SyntaxTree;
	SyntaxTree* child3 = new SyntaxTree;
	list<SyntaxTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	SyntaxTree st;
	SyntaxTree::Iterator test_it = st.end();
	st.children = children;
	test_it = --st.end();
	bool test = (*test_it == child3);
	TEST_EQUAL(test, true)
RESULT


CHECK(ConstIterator begin() const throw())
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


CHECK(ConstIterator end() const throw())
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

CHECK(SyntaxTree(SyntaxTree* left, SyntaxTree* right, ExpressionTree::Type type) throw())
	SyntaxTree* child1 = new SyntaxTree;
	SyntaxTree* child2 = new SyntaxTree;
	SyntaxTree tree(child1, child2, ExpressionTree::LEAF);
	TEST_EQUAL(tree.children.size(), 2)
	TEST_EQUAL(tree.type, ExpressionTree::LEAF)
RESULT

CHECK(SyntaxTree(const char* predicate_name, const char* args) throw())
	SyntaxTree tree("asdasd", "xcvxcv");
	TEST_EQUAL(tree.predicate, "asdasd")
	TEST_EQUAL(tree.argument, "xcvxcv")
RESULT

CHECK(void clear() throw())
	SyntaxTree tree("asdasd", "xcvxcv");
	TEST_EQUAL(tree.predicate, "asdasd")
	TEST_EQUAL(tree.argument, "xcvxcv")
	tree.clear();
	TEST_EQUAL(tree.predicate, "asdasd")
	TEST_EQUAL(tree.argument, "")
RESULT

CHECK(void dump(std::ostream& is = std::cout, Size depth = 0) const throw())
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	SyntaxTree tree("asdasd", "xcvxcv");
	TEST_EQUAL(tree.predicate, "asdasd")
	TEST_EQUAL(tree.argument, "xcvxcv")
	SyntaxTree* child1 = new SyntaxTree;
	tree.children.push_back(child1);
	tree.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(ExpressionParser_test.txt))
RESULT
// tests for ExpressionParser

ExpressionParser* exp;
CHECK(ExpressionParser())
 exp = new ExpressionParser;
RESULT

CHECK(~ExpressionParser())
	delete exp;
RESULT

ExpressionParser ep;
CHECK(ExpressionParser(const ExpressionParser& parser))
	ExpressionParser ep2(ep);
RESULT

CHECK(const SyntaxTree& getSyntaxTree() const throw(Exception::NullPointer))
	ExpressionParser empty;
	TEST_EXCEPTION(Exception::NullPointer, empty.getSyntaxTree())
RESULT

CHECK(void parse(const String& s) throw(Exception::ParseError))
	ExpressionParser parser;
	parser.parse("test('(H2)') AND element(H)");
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
