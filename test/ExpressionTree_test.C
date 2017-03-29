// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/KERNEL/expression.h>

/////////////////

using namespace BALL;

///////////////////////////

START_TEST(Expression)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// tests for class ExpressionTree::

ExpressionTree* et_ptr;

CHECK(ExpressionTree() throw())
	et_ptr = new ExpressionTree;
	TEST_NOT_EQUAL(et_ptr, 0)
	TEST_EQUAL(et_ptr->getType(), ExpressionTree::INVALID)
	TEST_EQUAL(et_ptr->getNegate(), false)
	TEST_EQUAL(et_ptr->getPredicate(), 0)
	list<const ExpressionTree*> test_list;
	bool test = (et_ptr->getChildren() == test_list);
	TEST_EQUAL(test, true)
RESULT


CHECK(~ExpressionTree() throw())
  delete et_ptr;
RESULT


CHECK(ExpressionTree(const ExpressionTree& tree) throw())
	ExpressionPredicate* ep = new ExpressionPredicate;
	ExpressionTree et1;
	ExpressionTree* child = new ExpressionTree;
	et1.setType(ExpressionTree::LEAF);
	et1.setNegate(true);
	et1.setPredicate(ep);
	et1.appendChild(child);

	ExpressionTree et2;
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	ExpressionTree et3(et1);
	test = (et1 == et3);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree(ExpressionPredicate* predicate, bool negate = false) throw())
	ExpressionPredicate* ep = new ExpressionPredicate;
	ExpressionTree et1;

	ExpressionTree et2(ep, true);
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.setNegate(true);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.setPredicate(new ExpressionPredicate);
	test = (et1 == et2);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree(Type type, list<const ExpressionTree*> children, bool negate = false) throw())
	ExpressionTree* child1 = new ExpressionTree;
	ExpressionTree* child2 = new ExpressionTree;
	ExpressionTree* child3 = new ExpressionTree;
	
	std::list<const ExpressionTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	ExpressionTree et1;

	ExpressionTree et2(ExpressionTree::LEAF, children, true);
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.setType(ExpressionTree::LEAF);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.setNegate(true);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)
RESULT


CHECK(bool operator () (const Atom& atom) const throw())
	ExpressionTree et;
RESULT


CHECK(bool operator == (const ExpressionTree& tree) const throw())
	ExpressionPredicate* ep = new ExpressionPredicate;
	ExpressionTree et1;
	ExpressionTree* child = new ExpressionTree;
	et1.setType(ExpressionTree::LEAF);
	et1.setNegate(true);
	et1.setPredicate(ep);
	et1.appendChild(child);

	ExpressionTree et2;
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.setType(ExpressionTree::LEAF);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.setNegate(true);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.setPredicate(new ExpressionPredicate);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.appendChild(new ExpressionTree);
	test = (et1 == et2);
	TEST_EQUAL(test, true)
RESULT

CHECK(bool operator != (const ExpressionTree& tree) const throw())
 	ExpressionPredicate* ep = new ExpressionPredicate;
	ExpressionTree et1;
	ExpressionTree* child = new ExpressionTree;
	et1.setType(ExpressionTree::LEAF);
	et1.setNegate(true);
	et1.setPredicate(ep);
	et1.appendChild(child);

	ExpressionTree et2;
	TEST_EQUAL(et2 != et1, true)

	et2.setType(ExpressionTree::LEAF);
	TEST_EQUAL(et2 != et1, true)

	et2.setNegate(true);
	TEST_EQUAL(et2 != et1, true)

	et2.setPredicate(new ExpressionPredicate);
	TEST_EQUAL(et2 != et1, true)

	et2.appendChild(new ExpressionTree);
	TEST_EQUAL(et2 != et1, false)
RESULT


CHECK(void setType(Type type) throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getType(), ExpressionTree::LEAF)
	et.setType(ExpressionTree::LEAF);
	TEST_EQUAL(et.getType(), ExpressionTree::LEAF)
RESULT


CHECK(Type getType() const throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getType(), ExpressionTree::OR)
	et.setType(ExpressionTree::OR);
	TEST_EQUAL(et.getType(), ExpressionTree::OR)
RESULT


CHECK(void setNegate(bool negate) throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getNegate(), true)
	et.setNegate(true);
	TEST_EQUAL(et.getNegate(), true)
RESULT


CHECK(bool getNegate() const throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getNegate(), true)
	et.setNegate(true);
	TEST_EQUAL(et.getNegate(), true)
RESULT


CHECK(void setPredicate(ExpressionPredicate* predicate) throw())
	ExpressionTree et;
	ExpressionPredicate* ep = new ExpressionPredicate;
	TEST_NOT_EQUAL(et.getPredicate(), ep)
	et.setPredicate(ep);
	TEST_EQUAL(et.getPredicate(), ep)
RESULT


CHECK(ExpressionPredicate* getPredicate() const throw())
	ExpressionPredicate* ep = new ExpressionPredicate;
	ExpressionTree et1;
	TEST_NOT_EQUAL(et1.getPredicate(), ep)
	ExpressionTree et2(ep, false);
	TEST_EQUAL(et2.getPredicate(), ep)
RESULT


CHECK(void appendChild(const ExpressionTree* child) throw())
	ExpressionTree* child1 = new ExpressionTree;
	ExpressionTree* child2 = new ExpressionTree;
	ExpressionTree* child3 = new ExpressionTree;
	
	std::list<const ExpressionTree*> children;
	children.push_back(child1);
	children.push_back(child2);
	children.push_back(child3);

	ExpressionTree et(ExpressionTree::OR, children);
	bool test = (et.getChildren() == children);
	TEST_EQUAL(test, true)

	et.appendChild(new ExpressionTree);
	test = (et.getChildren() == children);
	TEST_EQUAL(test, false)
RESULT


CHECK(const list<const ExpressionTree*>& getChildren() const throw())
	ExpressionTree et2;
	TEST_EQUAL(et2.getChildren().size(), 0)
RESULT


CHECK(ExpressionTree& operator = (const ExpressionTree& tree) throw())
	ExpressionTree et1(new ExpressionPredicate, true);
	ExpressionTree et2;
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)
	et2 = et1;
	test = (et1 == et2);
	TEST_EQUAL(test, true)
RESULT


CHECK(void clear() throw())
	ExpressionTree et1(new ExpressionPredicate, true);
	ExpressionTree empty, nonempty;
	nonempty.setType(ExpressionTree::LEAF);
	bool test = (empty == nonempty);
	TEST_NOT_EQUAL(test, true)
	nonempty.clear();
	test = (empty == nonempty);
	TEST_EQUAL(test, true)
RESULT

CHECK(BALL_CREATE(ExpressionTree))
 	ExpressionPredicate* ep = new ExpressionPredicate;
	ExpressionTree et1;
	ExpressionTree* child = new ExpressionTree;
	et1.setType(ExpressionTree::LEAF);
	et1.setNegate(true);
	et1.setPredicate(ep);
	et1.appendChild(child);
	ExpressionTree empty;
	ExpressionTree* test = (ExpressionTree*) et1.create(false, true);
	TEST_EQUAL(*test == empty, true)
	delete test;
	test = (ExpressionTree*) et1.create(true, false);
	TEST_EQUAL(*test == et1, true)
	delete test;
RESULT

CHECK(void dump(std::ostream& is = std::cout, Size depth = 0) const throw())
	ExpressionPredicate* ep = new ExpressionPredicate;
	ExpressionTree et1;
	ExpressionTree* child = new ExpressionTree;
	et1.setType(ExpressionTree::LEAF);
	et1.setNegate(true);
	et1.setPredicate(ep);
	et1.appendChild(child);
	String filename;
	NEW_TMP_FILE(filename);
	std::ofstream outfile(filename.c_str(), std::ios::out);
	et1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(ExpressionTree_test.txt))
RESULT

////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
