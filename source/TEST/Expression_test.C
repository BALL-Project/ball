// $Id: Expression_test.C,v 1.5 2001/07/11 17:06:20 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>
#include <list>

///////////////////////////

START_TEST(class_name, "$Id: Expression_test.C,v 1.5 2001/07/11 17:06:20 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class ExpressionPredicate::

CHECK(ExpressionPredicate::ExpressionPredicate() throw())
  //BAUSTELLE
RESULT

// Predicate for testing

class MickeyPredicate
	: public  ExpressionPredicate
{
	public:

	BALL_CREATE(MickeyPredicate)
	virtual bool operator () (const Atom& atom) const
	{
		return (atom.getElement().getSymbol() == "H");
	}
};

String mickey_predicate_string("isMickeyMouse");

ExpressionPredicate* ep_ptr;

CHECK(ExpressionPredicate::ExpressionPredicate(const ExpressionPredicate& predicate) throw())
	// BAUSTELLE: registerStandardPredicates_()
	ep_ptr = new ExpressionPredicate;
	TEST_NOT_EQUAL(ep_ptr, 0)
RESULT


CHECK(ExpressionPredicate::~ExpressionPredicate() throw())
	delete ep_ptr;
RESULT


CHECK(ExpressionPredicate::ExpressionPredicate(const String& argument) throw())
	// BAUSTELLE: registerStandardPredicates_()
	String argument("H");
	ExpressionPredicate ep1;
	TEST_NOT_EQUAL(ep1.getArgument(), argument)
	ExpressionPredicate ep2(argument);
	TEST_EQUAL(ep2.getArgument(), argument)
RESULT


CHECK(ExpressionPredicate::ExpressionPredicate& operator = (const ExpressionPredicate& predicate) throw())
	String arg("bla");
	ExpressionPredicate ep1(arg);
	ExpressionPredicate ep2;
	TEST_NOT_EQUAL(ep2.getArgument(), arg)
	ep2 = ep1;
	TEST_EQUAL(ep2.getArgument(), arg)
RESULT


CHECK(ExpressionPredicate::clear() throw())
	ExpressionPredicate empty;
	ExpressionPredicate not_empty("bla");
	bool test = (empty == not_empty);
	TEST_NOT_EQUAL(test, true)
	not_empty.clear();
	test = (empty == not_empty);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionPredicate::bool operator == (const ExpressionPredicate& predicate) const  throw())
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


CHECK(ExpressionPredicate::bool operator () (const Atom& atom) const  throw())
	Atom atom;
	ExpressionPredicate ep;
	TEST_EQUAL(ep.operator()(atom), true)
RESULT


CHECK(ExpressionPredicate::setArgument(const String& argument) throw())
	String arg = "BALL-test";
	ExpressionPredicate ep;
	TEST_NOT_EQUAL(ep.getArgument(), arg)
	ep.setArgument(arg);
	TEST_EQUAL(ep.getArgument(), arg)
RESULT


CHECK(ExpressionPredicate::getArgument() const  throw())
	String arg = "BALL-test";
	ExpressionPredicate ep;
	TEST_NOT_EQUAL(ep.getArgument(), arg)
	ep.setArgument(arg);
	TEST_EQUAL(ep.getArgument(), arg)
RESULT


// tests for class ExpressionTree::

ExpressionTree* et_ptr;

CHECK(ExpressionTree::ExpressionTree() throw())
	et_ptr = new ExpressionTree;
	TEST_NOT_EQUAL(et_ptr, 0)
	// BAUSTELLE
	TEST_EQUAL(et_ptr->getType(), ExpressionTree::INVALID)
	TEST_EQUAL(et_ptr->getNegate(), false)
	TEST_EQUAL(et_ptr->getPredicate(), 0)
	// TEST_EQUAL(et_ptr->getChildren(), )
RESULT


CHECK(ExpressionTree::~ExpressionTree() throw())
  delete et_ptr;
RESULT


CHECK(ExpressionTree::ExpressionTree(const ExpressionTree& tree) throw())
	ExpressionPredicate ep;
	ExpressionTree et1;
	ExpressionTree child;
	et1.setType(ExpressionTree::LEAF);
	et1.setNegate(true);
	et1.setPredicate(&ep);
	et1.appendChild(&child);

	ExpressionTree et2;
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	ExpressionTree et3(et1);
	test = (et1 == et3);
	TEST_NOT_EQUAL(test, true)
RESULT


CHECK(ExpressionTree::ExpressionTree(ExpressionPredicate* predicate, bool negate = false) throw())
	ExpressionPredicate ep;
	ExpressionTree et1;

	ExpressionTree et2(&ep, true);
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.setNegate(true);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.setPredicate(&ep);
	test = (et1 == et2);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree::ExpressionTree(Type type, list<ExpressionTree*> children, bool negate = false) throw())
	ExpressionTree child1;
	ExpressionTree child2;
	ExpressionTree child3;
	
	::std::list<const ExpressionTree*> children;
	children.push_back(&child1);
	children.push_back(&child2);
	children.push_back(&child3);

	ExpressionTree et1;

	ExpressionTree et2(ExpressionTree::LEAF, children, true);
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.setNegate(true);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et1.appendChild(&child1);
	test = (et1 == et2);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree::bool operator () (const Atom& atom) const  throw())
  //BAUSTELLE
RESULT


CHECK(ExpressionTree::bool operator == (const ExpressionTree& tree) const  throw())
	ExpressionPredicate ep;
	ExpressionTree et1;
	ExpressionTree child;
	et1.setType(ExpressionTree::LEAF);
	et1.setNegate(true);
	et1.setPredicate(&ep);
	et1.appendChild(&child);

	ExpressionTree et2;
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.setType(ExpressionTree::LEAF);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.setNegate(true);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.setPredicate(&ep);
	test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)

	et2.appendChild(&child);
	test = (et1 == et2);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree::setType(Type type) throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getType(), ExpressionTree::LEAF)
	et.setType(ExpressionTree::LEAF);
	TEST_EQUAL(et.getType(), ExpressionTree::LEAF)
RESULT


CHECK(ExpressionTree::getType() const  throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getType(), ExpressionTree::OR)
	et.setType(ExpressionTree::OR);
	TEST_EQUAL(et.getType(), ExpressionTree::OR)
RESULT


CHECK(ExpressionTree::setNegate(bool negate) throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getNegate(), true)
	et.setNegate(true);
	TEST_EQUAL(et.getNegate(), true)
RESULT


CHECK(ExpressionTree::getNegate() const  throw())
	ExpressionTree et;
	TEST_NOT_EQUAL(et.getNegate(), true)
	et.setNegate(true);
	TEST_EQUAL(et.getNegate(), true)
RESULT


CHECK(ExpressionTree::setPredicate(ExpressionPredicate* predicate) throw())
	ExpressionTree et;
	ExpressionPredicate ep;
	TEST_NOT_EQUAL(et.getPredicate(), &ep)
	et.setPredicate(&ep);
	TEST_EQUAL(et.getPredicate(), &ep)
RESULT


CHECK(ExpressionTree::getPredicate() const  throw())
	ExpressionPredicate ep;
	ExpressionTree et1;
	TEST_NOT_EQUAL(et1.getPredicate(), &ep)
	ExpressionTree et2(&ep, false);
	TEST_EQUAL(et2.getPredicate(), &ep)
RESULT


CHECK(ExpressionTree::appendChild(const ExpressionTree* child) throw())
	ExpressionTree child1;
	ExpressionTree child2;
	ExpressionTree child3;
	
	::std::list<const ExpressionTree*> children;
	children.push_back(&child1);
	children.push_back(&child2);
	children.push_back(&child3);

	ExpressionTree et;
	bool test = (et.getChildren() == children);
	TEST_NOT_EQUAL(test, true)

	et.appendChild(&child1);
	test = (et.getChildren() == children);
	TEST_NOT_EQUAL(test, true)

	et.appendChild(&child2);
	test = (et.getChildren() == children);
	TEST_NOT_EQUAL(test, true)

	et.appendChild(&child3);
	test = (et.getChildren() == children);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree::getChildren() const throw())
	ExpressionTree child1;
	ExpressionTree child2;
	ExpressionTree child3;
	
	::std::list<const ExpressionTree*> children;
	children.push_back(&child1);
	children.push_back(&child2);
	children.push_back(&child3);

	ExpressionTree et;
	bool test = (et.getChildren() == children);
	TEST_NOT_EQUAL(test, true)

	et.appendChild(&child1);
	test = (et.getChildren() == children);
	TEST_NOT_EQUAL(test, true)

	et.appendChild(&child2);
	test = (et.getChildren() == children);
	TEST_NOT_EQUAL(test, true)

	et.appendChild(&child3);
	test = (et.getChildren() == children);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree::ExpressionTree& operator = (const ExpressionTree& tree) throw())
	ExpressionPredicate ep;
	ExpressionTree et1(&ep, true);
	ExpressionTree et2;
	bool test = (et1 == et2);
	TEST_NOT_EQUAL(test, true)
	et2 = et1;
	test = (et1 == et2);
	TEST_EQUAL(test, true)
RESULT


CHECK(ExpressionTree::clear() throw())
	ExpressionPredicate ep;
	ExpressionTree nonempty(&ep, true);
	ExpressionTree empty;
	bool test = (empty == nonempty);
	TEST_NOT_EQUAL(test, true)
	nonempty.clear();
	test = (empty == nonempty);
	TEST_EQUAL(test, true)
RESULT


// tests for class SyntaxTree::

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


CHECK(SyntaxTree::SyntaxTree(const String& expression) throw())
	String teststring("BALL teststring");
	st_ptr = new SyntaxTree(teststring);
	TEST_NOT_EQUAL(st_ptr, 0)
	TEST_EQUAL(st_ptr->expression, teststring)
	TEST_EQUAL(st_ptr->argument, "")
	TEST_EQUAL(st_ptr->evaluated, false)
	TEST_EQUAL(st_ptr->negate, false)
	TEST_EQUAL(st_ptr->type, ExpressionTree::INVALID)
	list<SyntaxTree*> children;
	bool test = (st_ptr->children == children);
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::begin() throw())
	SyntaxTree child1;
	SyntaxTree child2;
	SyntaxTree child3;
	list<SyntaxTree*> children;
	children.push_back(&child1);
	children.push_back(&child2);
	children.push_back(&child3);

	SyntaxTree st;
	SyntaxTree::Iterator test_it = st.begin();
	bool test = (test_it == children.begin());
	TEST_NOT_EQUAL(test, true)

	/* BAUSTELLE
	st.children = children;
	*/
	test = (test_it == children.begin());
	Log.info() << "test " << test << endl;
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::end() throw())
	SyntaxTree child1;
	SyntaxTree child2;
	SyntaxTree child3;
	list<SyntaxTree*> children;
	children.push_back(&child1);
	children.push_back(&child2);
	children.push_back(&child3);

	SyntaxTree st;
	SyntaxTree::Iterator test_it = st.end();
	bool test = (test_it == children.end());
	TEST_NOT_EQUAL(test, true)

	/* BAUSTELLE
	st.children = children;
	*/
	test = (test_it == children.end());
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::begin() const  throw())
	SyntaxTree child1;
	SyntaxTree child2;
	SyntaxTree child3;
	list<SyntaxTree*> children;
	children.push_back(&child1);
	children.push_back(&child2);
	children.push_back(&child3);

	SyntaxTree st;
	SyntaxTree::ConstIterator test_it = st.begin();
	bool test = (test_it == children.begin());
	TEST_NOT_EQUAL(test, true)

	/* BAUSTELLE
	st.children = children;
	*/
	test = (test_it == children.begin());
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::end() const  throw())
	SyntaxTree child1;
	SyntaxTree child2;
	SyntaxTree child3;
	list<SyntaxTree*> children;
	children.push_back(&child1);
	children.push_back(&child2);
	children.push_back(&child3);

	SyntaxTree st;
	SyntaxTree::ConstIterator test_it = st.end();
	bool test = (test_it == children.end());
	TEST_NOT_EQUAL(test, true)

	/* BAUSTELLE
	st.children = children;
	*/
	test = (test_it == children.end());
	TEST_EQUAL(test, true)
RESULT


CHECK(SyntaxTree::mergeLeft(SyntaxTree* tree) throw())
	SyntaxTree* tree = new SyntaxTree;
	list<SyntaxTree*> test_list;
	test_list.push_front(tree);

	SyntaxTree st;
	st.mergeLeft(tree);
	bool test = (st.children == test_list);
	TEST_EQUAL(test, true);

	st.clear();
	test_list.clear();

	SyntaxTree* subtree1 = new SyntaxTree;
	SyntaxTree* subtree2 = new SyntaxTree;
	tree->children.push_front(subtree1);
	tree->children.push_front(subtree2);
	st.mergeLeft(tree);
	test_list.push_front(subtree1);
	test_list.push_front(subtree2);
	test = (st.children == test_list);
	TEST_EQUAL(test, true);
RESULT


CHECK(SyntaxTree::mergeRight(SyntaxTree* tree) throw())
	SyntaxTree* tree = new SyntaxTree;
	list<SyntaxTree*> test_list;
	test_list.push_front(tree);

	SyntaxTree st;
	st.mergeLeft(tree);
	bool test = (st.children == test_list);
	TEST_EQUAL(test, true);

	st.clear();
	test_list.clear();

	SyntaxTree* subtree1 = new SyntaxTree;
	SyntaxTree* subtree2 = new SyntaxTree;
	tree->children.push_front(subtree1);
	tree->children.push_front(subtree2);
	st.mergeLeft(tree);
	test_list.push_back(subtree1);
	test_list.push_back(subtree2);
	test = (st.children == test_list);
	TEST_EQUAL(test, true);
RESULT


CHECK(SyntaxTree::parse() throw())
	// This method calls expandBrackets_(), collapseANDs_() and
	// collapseORs_().
  // BAUSTELLE
RESULT


// tests for class Expression::

CHECK(Expression::(*CreationMethod)())
  //BAUSTELLE
RESULT


Expression* e_ptr = 0;

CHECK(Expression::Expression() throw())
	e_ptr = new Expression;
	TEST_NOT_EQUAL(e_ptr, 0)
	// BAUSTELLE:
	// test for registerStandardPredicates_()
RESULT


CHECK(Expression::~Expression() throw())
	delete e_ptr;
RESULT


CHECK(Expression::Expression(const Expression& expression) throw())
	// BAUSTELLE
RESULT


CHECK(Expression::Expression(const String& expression_string) throw())
	Expression e("(H)");
	StringHashMap<Expression::CreationMethod> test_map;
	bool test = (test_map == e.getCreationMethods());
	TEST_EQUAL(test, true)
RESULT


CHECK(Expression::hasPredicate(const String& name) const  throw())
	Expression e;
	e.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	TEST_EQUAL(e.hasPredicate("You don't have this predicate"), false)
	TEST_EQUAL(e.hasPredicate(mickey_predicate_string), true)
RESULT


CHECK(Expression::bool operator == (const Expression& expression) const  throw())
	Expression e1;
	Expression e2;
	bool test = (e1 == e2);
	TEST_EQUAL(test, true)

	e1.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	test = (e1 == e2);
	TEST_NOT_EQUAL(test, true)

	e2.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	test = (e1 == e2);
	TEST_EQUAL(test, true)
RESULT


CHECK(Expression::bool operator () (const Atom& atom) const  throw())
  //BAUSTELLE
RESULT


CHECK(Expression::getPredicate(const String& name, const String& args = "") const  throw())
	Expression e;
	e.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	ExpressionPredicate* ep = e.getPredicate(mickey_predicate_string);
	bool test = (ep == MickeyPredicate::createDefault());
	TEST_EQUAL(test, true)
RESULT


CHECK(Expression::registerPredicate(const String& name, CreationMethod creation_method) throw())
	Expression e;
	e.registerPredicate(mickey_predicate_string, MickeyPredicate::createDefault);
	ExpressionPredicate* ep = e.getPredicate(mickey_predicate_string);
	bool test = (ep == MickeyPredicate::createDefault());
	TEST_EQUAL(test, true)
RESULT


CHECK(Expression::setExpression(const String& expression) throw())
	Expression e;
	e.setExpression("connectedTo(H)");
	// BAUSTELLE
RESULT


CHECK(Expression::getExpressionString() const  throw())
	Expression e("connectedTo(H)");
	TEST_EQUAL("connectedTo(H)", e.getExpressionString())
RESULT


CHECK(Expression::getExpressionTree() const  throw())
	Expression e("connectedTo(H)");
	// BAUSTELLE
RESULT


CHECK(Expression::Expression& operator = (const Expression& expression) throw())
	Expression e1("connectedTo(H)");
	Expression e2;
	bool test = (e1 == e2);
	TEST_NOT_EQUAL(test, true)

	e2 = e1;
	test = (e1 == e2);
	TEST_EQUAL(test, true)
RESULT


CHECK(Expression::clear() throw())
	Expression empty;
	Expression nonempty("connectedTo(H)");
	bool test = (empty == nonempty);
	TEST_NOT_EQUAL(test, true)

	nonempty.clear();
	test = (empty == nonempty);
	TEST_EQUAL(test, true)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
