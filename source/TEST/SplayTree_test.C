// $Id: SplayTree_test.C,v 1.1 2000/08/09 10:18:58 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/splayTree.h>

///////////////////////////

START_TEST(class_name, "$Id: SplayTree_test.C,v 1.1 2000/08/09 10:18:58 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
	
CHECK(TSplayTree::TSplayTree())
  //BAUSTELLE
RESULT


CHECK(TSplayTree::TSplayTree(const TSplayTree& splay_tree, bool deep = true))
  //BAUSTELLE
RESULT


CHECK(TSplayTree::~TSplayTree())
  //BAUSTELLE
RESULT


CHECK(TSplayTree::set(const TSplayTree& splay_tree, bool deep = true))
  //BAUSTELLE
RESULT


CHECK(TSplayTree::set(const TSplayTree &splay_tree, BSTreeIterator::WalkOrder walk_order))
  //BAUSTELLE
RESULT


CHECK(TSplayTree::TSplayTree& operator = (const TSplayTree& splay_tree))
  //BAUSTELLE
RESULT


CHECK(TSplayTree::get(TSplayTree& splay_tree, bool deep = true) const )
  //BAUSTELLE
RESULT


CHECK(TSplayTree::get(TSplayTree& splay_tree, BSTreeIterator::WalkOrder walk_order) const )
  //BAUSTELLE
RESULT


CHECK(TSplayTree::getMinimum())
  //BAUSTELLE
RESULT


CHECK(TSplayTree::getMinimum() const )
  //BAUSTELLE
RESULT


CHECK(TSplayTree::getMaximum())
  //BAUSTELLE
RESULT


CHECK(TSplayTree::getMaximum() const )
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


CHECK(TSplayTree::host(Visitor<TSplayTree>& visitor))
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
