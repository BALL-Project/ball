// $Id: SplayTree_test.C,v 1.2 2000/08/19 13:59:38 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/splayTree.h>
#include <BALL/DATATYPE/binarySearchTree.h>
#include <BALL/CONCEPT/visitor.h>

///////////////////////////

START_TEST(class_name, "$Id: SplayTree_test.C,v 1.2 2000/08/19 13:59:38 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

TSplayTree<int>* t;
const Comparator<int> comp = Comparator<int>();
const Comparator<int> comp2 = Comparator<int>();

CHECK(TSplayTree::TSplayTree())
	t = new TSplayTree<int>();
	TEST_NOT_EQUAL(t, 0)	
RESULT

CHECK(TSplayTree::~TSplayTree())
	delete t;
RESULT

TSplayTree<int> tree, tree2;
tree.setComparator(comp);

CHECK(TSplayTree::TSplayTree(const TSplayTree& splay_tree, bool deep = true))
	TSplayTree<int>* t;
	TEST_EQUAL(tree.getComparator(), &comp)
	t = new TSplayTree<int>(tree);
	TEST_NOT_EQUAL(t, 0)	
	TEST_EQUAL(t->getComparator(), &comp)
RESULT

CHECK(TSplayTree::set(const TSplayTree& splay_tree, bool deep = true))
  tree2.set(tree);
	TEST_EQUAL(tree2.getComparator(), &comp)
	tree2.setComparator(comp2);
RESULT

CHECK(TSplayTree::set(const TSplayTree &splay_tree, BSTreeIterator::WALK_ORDER__PREORDER))
  tree2.set(tree, BSTreeIterator::WALK_ORDER__PREORDER);
	TEST_EQUAL(tree2.getComparator(), &comp)
	tree2.setComparator(comp2);
RESULT

CHECK(TSplayTree::TSplayTree& operator = (const TSplayTree& splay_tree))
  tree2 = tree;
	TEST_EQUAL(tree2.getComparator(), &comp)
	tree2.setComparator(comp2);
RESULT

CHECK(TSplayTree::get(TSplayTree& splay_tree, bool deep = true) const )
  tree.get(tree2, true);
	TEST_EQUAL(tree2.getComparator(), &comp)
	tree2.setComparator(comp2);
RESULT

CHECK(TSplayTree::get(TSplayTree& splay_tree, BSTreeIterator::WALK_ORDER__PREORDER) const )
  tree.get(tree2, BSTreeIterator::WALK_ORDER__PREORDER);
	TEST_EQUAL(tree2.getComparator(), &comp)
	tree2.setComparator(comp2);
RESULT

//Visitor<TSplayTree> visitor;
//Visitor<int> visitor;

CHECK(TSplayTree::host(Visitor<TSplayTree>& visitor))
 // tree.visit(visitor);
RESULT

CHECK(TSplayTree::getMinimum() const )
  //BAUSTELLE
RESULT

CHECK(TSplayTree::getMaximum() const )
  //BAUSTELLE
RESULT

CHECK(TSplayTree::getMinimum())
  //BAUSTELLE
RESULT

CHECK(TSplayTree::getMaximum())
  //BAUSTELLE
RESULT

CHECK(TSplayTree::find(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TSplayTree::find(const DataType& data) const )
  //BAUSTELLE
RESULT

CHECK(TSplayTree::insert(const DataType& data, bool multiple = true))
  //BAUSTELLE
RESULT

CHECK(TSplayTree::insertNotSplayed(const DataType& data, bool multiple = true))
  //BAUSTELLE
RESULT

CHECK(TSplayTree::detach(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TSplayTree::detachMinimum())
  //BAUSTELLE
RESULT

CHECK(TSplayTree::detachMaximum())
  //BAUSTELLE
RESULT

CHECK(TSplayTree::remove(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TSplayTree::removeAll(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TSplayTree::removeMinimum())
  //BAUSTELLE
RESULT

CHECK(TSplayTree::removeMaximum())
  //BAUSTELLE
RESULT

CHECK(TSplayTree::has(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TSplayTree::has(const DataType& data) const )
  //BAUSTELLE
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
