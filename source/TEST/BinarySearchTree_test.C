// $Id: BinarySearchTree_test.C,v 1.3 2000/07/31 21:57:57 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/binarySearchTree.h>
#include <iostream>
///////////////////////////

using namespace BALL;

// helper class: a processor counting tree items
class BSTreeItemCollector
	: public UnaryProcessor<BSTreeItem>
{
	public:
	bool start()
	{
		// clear the item list
		list_.clear();
		return true;
	}

	bool finish()
	{
		return true;
	}

	Processor::Result operator () (BSTreeItem& item)
	{
		// store the item
		list_.push_back(&item);
		return Processor::CONTINUE;
	}

	list<BSTreeItem*> getList()
	{
		return list_;
	}
	
	private:
	list<BSTreeItem*>	list_;
};

BSTreeItem* test(list<BSTreeItem*>& mylist, list<BSTreeItem*>::iterator& list_it)
{
	if (list_it == mylist.end())
	{
		return 0;
	}
	BSTreeItem* temp = *list_it;
	list_it++;
	return temp;
}

START_TEST(class_name, "$Id: BinarySearchTree_test.C,v 1.3 2000/07/31 21:57:57 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace std;

BSTreeItem* itemp;
CHECK(BSTreeItem())
	itemp = new BSTreeItem();
	TEST_NOT_EQUAL(itemp, 0)
RESULT

CHECK(~BSTreeItem())
	delete itemp;
RESULT

CHECK(BSTreeItem(BSTreeItem* left_item, BSTreeItem* right_item, char color = BSTreeItem::BLACK))
  BSTreeItem* item;
  BSTreeItem* item2;
  BSTreeItem* item3;
	item = new BSTreeItem(item2, item3, BSTreeItem::RED);
	TEST_NOT_EQUAL(item, 0)
	TEST_EQUAL(item->getLeftChild(), item2)
	TEST_EQUAL(item->getRightChild(), item3)
RESULT
{
//			     	  	   		item
//	    			left              right
//               rleft    lright    rright
//                                     rrright
BSTreeItem item, left, right, rleft, lright, rright, rrright;
item  = BSTreeItem(&left, &right, BSTreeItem::RED);
left  = BSTreeItem(0, &rleft, BSTreeItem::RED);
rleft = BSTreeItem(0, 0, BSTreeItem::BLACK);
right = BSTreeItem(&lright, &rright, BSTreeItem::BLACK);
lright = BSTreeItem(0, 0, BSTreeItem::RED);
rright = BSTreeItem(0, &rrright, BSTreeItem::BLACK);
rrright = BSTreeItem(0, 0, BSTreeItem::BLACK);

cout <<endl;
cout << "item " << &item<<endl;
cout << "left " << &left<<endl;
cout << "rleft " <<&rleft <<endl;
cout << "right " <<&right <<endl;
cout << "lright " <<&lright <<endl;
cout << "rright " <<&rright <<endl;
cout << "rrright " <<&rrright <<endl;
cout <<endl;

CHECK(BSTreeItem(const BSTreeItem& item, bool /* deep */= true))
  BSTreeItem* item1;
	item1 = new BSTreeItem(item);
	TEST_NOT_EQUAL(item1, 0)
	TEST_EQUAL(item1->getLeftChild() , &left)
	TEST_EQUAL(item1->getRightChild(), &right)
RESULT

CHECK(getLeftChild())
	TEST_EQUAL(item.getLeftChild(), &left)
	TEST_EQUAL(left.getLeftChild(), 0)
RESULT

CHECK(setLeftChild(BSTreeItem* item))
  item.setLeftChild(0);
	TEST_EQUAL(item.getLeftChild(), 0)
  item.setLeftChild(&right);
	TEST_EQUAL(item.getLeftChild(), &right)
  item.setLeftChild(&left);
	TEST_EQUAL(item.getLeftChild(), &left)
RESULT

CHECK(getRightChild())
	TEST_EQUAL(item.getRightChild(), &right)
	TEST_EQUAL(rleft.getRightChild(), 0)
RESULT

CHECK(setRightChild(BSTreeItem *item))
  item.setRightChild(0);
	TEST_EQUAL(item.getRightChild(), 0)
  item.setRightChild(&left);
	TEST_EQUAL(item.getRightChild(), &left)
  item.setRightChild(&right);
	TEST_EQUAL(item.getRightChild(), &right)
RESULT

CHECK(getSize())
	TEST_EQUAL(item.getSize(), 7)
	TEST_EQUAL(rleft.getSize(), 1)
RESULT

CHECK(getHeight())
	TEST_EQUAL(item.getHeight(), 4)
	TEST_EQUAL(rleft.getHeight(), 1)
RESULT

CHECK(getMinimum())
	TEST_EQUAL(item.getMinimum(), &left)
	TEST_EQUAL(left.getMinimum(), &left)
RESULT

CHECK(getMaximum())
	TEST_EQUAL(item.getMaximum(), &rrright)
	TEST_EQUAL(rrright.getMaximum(), &rrright)
RESULT

CHECK(getParentOfMinimum())
	TEST_EQUAL(item.getParentOfMinimum(), &item)
	TEST_EQUAL(rrright.getParentOfMinimum(), 0)
RESULT

CHECK(getParentOfMaximum())
	TEST_EQUAL(item.getParentOfMaximum(), &rright)
	TEST_EQUAL(rrright.getParentOfMaximum(), 0)
RESULT

CHECK(getParentOfPredecessor())
	TEST_EQUAL(item.getParentOfPredecessor(), &left)
	TEST_EQUAL(left.getParentOfPredecessor(), 0)
RESULT

CHECK(getParentOfSuccessor())
	TEST_EQUAL(item.getParentOfSuccessor(), &right)
	TEST_EQUAL(left.getParentOfSuccessor(), &left)
	TEST_EQUAL(rrright.getParentOfSuccessor(), 0)
RESULT

CHECK(detachNode(BSTreeItem*& root, BSTreeItem* t, 
			 BSTreeItem* p, bool right_side))
	//BSTreeItem* pointer;
//	TEST_EQUAL(item.detachNode(pointer, &left, ), &right) //???
RESULT

CHECK(rotateRight())
  /*TEST_EQUAL(right.rotateRight(), &item)
	TEST_EQUAL(right.getLeftChild(), &lright)
	TEST_EQUAL(right.getRightChild(), &rright)*/ //??? segmentation fault
RESULT

CHECK(rotateLeft())
  //BAUSTELLE
RESULT

BSTreeItemCollector myproc;
list<BSTreeItem*>::iterator list_it;

CHECK(applyPreorder(UnaryProcessor<BSTreeItem>& processor))
	list<BSTreeItem*> mylist(myproc.getList());
	item.applyPreorder(myproc);
	list_it = mylist.begin();
//	TEST_EQUAL((list_it != mylist.end()) && (*list_it == &left), true)
	TEST_EQUAL(test(mylist, list_it), &left)
RESULT

CHECK(applyInorder(UnaryProcessor<BSTreeItem>& processor))
  //BAUSTELLE
RESULT

CHECK(applyPostorder(UnaryProcessor<BSTreeItem>& processor))
  //BAUSTELLE
RESULT

CHECK(applyLevelorder(UnaryProcessor<BSTreeItem>& processor))
  //BAUSTELLE
RESULT

CHECK(applyPreorderFlat(UnaryProcessor<BSTreeItem>& processor))
  //BAUSTELLE
RESULT

CHECK(applyInorderFlat(UnaryProcessor<BSTreeItem>& processor))
  //BAUSTELLE
RESULT

CHECK(applyPostorderFlat(UnaryProcessor<BSTreeItem>& processor))
  //BAUSTELLE
RESULT

CHECK(apply(UnaryProcessor<BSTreeItem>& processor))
  //BAUSTELLE
RESULT

CHECK(isLeaf() const )
	TEST_EQUAL(item.isLeaf(), false)
	TEST_EQUAL(left.isLeaf(), false)
	TEST_EQUAL(rleft.isLeaf(), true)
RESULT

CHECK(isInterior() const )
	TEST_EQUAL(item.isInterior(), true)
	TEST_EQUAL(left.isInterior(), true)
	TEST_EQUAL(rleft.isInterior(), false)
RESULT

CHECK(insertBalance(BSTreeItem* root, BSTreeItem::Pack& pp))
  //BAUSTELLE
RESULT

CHECK(removeBalance(BSTreeItem* root, BSTreeItem::Pack& pp))
  //BAUSTELLE
RESULT

CHECK(replace(BSTreeItem* root, BSTreeItem::Pack& pp))
  //BAUSTELLE
RESULT

CHECK(detachMinimum(BSTreeItem*& root))
  //BAUSTELLE
RESULT

CHECK(detachMaximum(BSTreeItem*& root))
  //BAUSTELLE
RESULT
}
// tests for class TBSTreeItem::

TBSTreeItem<int>* tbsitem;
CHECK(TBSTreeItem())
	tbsitem = new TBSTreeItem<int>;
	TEST_NOT_EQUAL(tbsitem, 0)
RESULT

CHECK(~TBSTreeItem())
  delete tbsitem;
RESULT

TBSTreeItem<int> item;

CHECK(TBSTreeItem(const DataType& data, TBSTreeItem* left_item, 
								 TBSTreeItem* right_item, char color = BSTreeItem::BLACK))
  
RESULT

CHECK(BALL_CREATE(TBSTreeItem))
	//
RESULT

CHECK(getData())
  //BAUSTELLE
RESULT

CHECK(getLeftChild())
  //BAUSTELLE
RESULT

CHECK(getRightChild())
  //BAUSTELLE
RESULT

CHECK(count(const DataType& data, const Comparator<DataType>* comparator))
  //BAUSTELLE
RESULT

CHECK(applyPreorder)
  //BAUSTELLE
RESULT

CHECK(applyInorder)
  //BAUSTELLE
RESULT

CHECK(applyPostorder)
  //BAUSTELLE
RESULT

CHECK(applyLevelorder)
  //BAUSTELLE
RESULT

CHECK(applyPreorderFlat)
  //BAUSTELLE
RESULT

CHECK(applyInorderFlat)
  //BAUSTELLE
RESULT

CHECK(applyPostorderFlat)
  //BAUSTELLE
RESULT

CHECK(apply)
  //BAUSTELLE
RESULT

// tests for class TBSTreeItem::TreeItemProcessor_::

CHECK(TBSTreeItem::TreeItemProcessor_::TreeItemProcessor_(UnaryProcessor<DataType>* processor = 0))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TreeItemProcessor_::Processor::Result operator () (BSTreeItem& item))
  //BAUSTELLE
RESULT

// tests for class TBSTreeItem::BSTreeIterator::

CHECK(TBSTreeItem::BSTreeIterator::BSTreeIterator(const BSTreeItem* item = 0, WalkOrder walk_order = BSTreeIterator::WALK_ORDER__PREORDER))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::BSTreeIterator::BSTreeIterator(const BSTreeIterator& iterator))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::BSTreeIterator::~BSTreeIterator())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::set(const BSTreeIterator& iterator))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::forward())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::bool operator == (const BSTreeIterator& iterator) const )
  //BAUSTELLE
RESULT

// tests for class TBSTreeItem::TBSTreeIterator::

CHECK(TBSTreeItem::TBSTreeIterator::TBSTreeIterator(const BSTreeItemType* itemType = 0))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTreeIterator::TBSTreeIterator(const BSTreeItemType* itemType, BSTreeIterator::WalkOrder walk_order))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTreeIterator::TBSTreeIterator(const TBSTreeIterator& iterator))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTreeIterator::~TBSTreeIterator())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTreeIterator::forward())
  //BAUSTELLE
RESULT

// tests for class TBSTreeItem::TBSTree::

CHECK(TBSTreeItem::TBSTree::BALL_CREATE(TBSTree))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::setComparator(const Comparator<DataType>& comparator))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::resetComparator())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::getComparator() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::getRoot() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::getHeight() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::getSize() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::getMinimum() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::getMaximum() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::find(const DataType& data) const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::count(const DataType& data) const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::insert(const DataType& data, bool multiple = true))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::detach(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::detachMinimum())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::detachMaximum())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::remove(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::removeAll(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::removeMinimum())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::removeMaximum())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::host(Visitor<TBSTree>& visitor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::bool operator == (const TBSTree& tree) const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::bool operator != (const TBSTree& tree) const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::has(const DataType& data) const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::isEmpty() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::isValid() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::dump(std::ostream& s = std::cout, Size depth = 0) const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::applyPreorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::applyInorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::applyPostorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::applyLevelorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::applyPreorderFlat(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::applyInorderFlat(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::applyPostorderFlat(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::apply(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

// tests for class TBSTreeItem::TBSTree::PreorderIteratorTraits_::

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::(PreorderIteratorTraits_)())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::PreorderIteratorTraits_(const TBSTree& tree))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::PreorderIteratorTraits_(const PreorderIteratorTraits_& traits))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::PreorderIteratorTraits_& operator =  (const PreorderIteratorTraits_& traits))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::getContainer() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::getPosition())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::bool operator == (const PreorderIteratorTraits_& traits) const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::isValid() const )
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::toBegin())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::toEnd())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::TBSTree::PreorderIteratorTraits_::getData() const )
  //BAUSTELLE
RESULT

// tests for class TBSTreeItem::InorderIteratorTraits_::

CHECK(TBSTreeItem::InorderIteratorTraits_::InorderIteratorTraits_())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::InorderIteratorTraits_::InorderIteratorTraits_(const TBSTree& tree))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::InorderIteratorTraits_::InorderIteratorTraits_(const InorderIteratorTraits_& traits))
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::InorderIteratorTraits_::toBegin())
  //BAUSTELLE
RESULT

CHECK(TBSTreeItem::beginInorder())
  //BAUSTELLE
RESULT

// tests for class PostorderIteratorTraits_::

CHECK(PostorderIteratorTraits_::PostorderIteratorTraits_())
  //BAUSTELLE
RESULT

CHECK(PostorderIteratorTraits_::PostorderIteratorTraits_(const TBSTree& tree))
  //BAUSTELLE
RESULT

CHECK(PostorderIteratorTraits_::PostorderIteratorTraits_(const PostorderIteratorTraits_& traits))
  //BAUSTELLE
RESULT

CHECK(PostorderIteratorTraits_::toBegin())
  //BAUSTELLE
RESULT

// tests for class LevelorderIteratorTraits_::

CHECK(LevelorderIteratorTraits_::LevelorderIteratorTraits_())
  //BAUSTELLE
RESULT

CHECK(LevelorderIteratorTraits_::LevelorderIteratorTraits_(const TBSTree& tree))
  //BAUSTELLE
RESULT

CHECK(LevelorderIteratorTraits_::LevelorderIteratorTraits_(const LevelorderIteratorTraits_& traits))
  //BAUSTELLE
RESULT

CHECK(LevelorderIteratorTraits_::toBegin())
  //BAUSTELLE
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
