// $Id: BinarySearchTree_test.C,v 1.7 2000/08/05 23:57:36 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/binarySearchTree.h>
#include <iostream>
///////////////////////////

using namespace BALL;

// helper class: a processor counting tree items
class BSTreeItemCollector	: public UnaryProcessor<BSTreeItem>
{
	public:
	bool start()
	{
		// clear the item list
		list_.clear();
		list_it_ = list_.begin();
		return true;
	}

	bool finish()
	{
		return true;
	}

	Processor::Result operator () (BSTreeItem& item)
	{	// store the item
		list_.push_back(&item);
		return Processor::CONTINUE;
	}

	list<BSTreeItem*> getList()
	{ // get a pointer to the list
		return list_;
	}
	
	BSTreeItem* getPointer()
	{	// get a pointer to the first element in the list
		if (list_it_ == list_.end())
		{
			return 0;
		}
		BSTreeItem* temp = *list_it_;
		list_it_++;
		return temp;
	}
	Size getSize()
	{	// get the size of the list
		return list_.size();
	}
	
	void reset()
	{ // reset the iterator to the first element of the list
		list_it_ = list_.begin();
	}

	private:
	list<BSTreeItem*>	list_;
	list<BSTreeItem*>::iterator list_it_;
};

//			     	  	   		item
//	    			left              right
//               rleft    lright    rright
//                      llright         rrright

BSTreeItem item, left, right, rleft, lright, llright, rright, rrright;

void initialize()
{
	item  = BSTreeItem(&left, &right, BSTreeItem::RED);
	left  = BSTreeItem(0, &rleft, BSTreeItem::RED);
	rleft = BSTreeItem(0, 0, BSTreeItem::BLACK);
	right = BSTreeItem(&lright, &rright, BSTreeItem::BLACK);
	lright = BSTreeItem(&llright, 0, BSTreeItem::RED);
	llright = BSTreeItem(0, 0, BSTreeItem::RED);
	rright = BSTreeItem(0, &rrright, BSTreeItem::BLACK);
	rrright = BSTreeItem(0, 0, BSTreeItem::BLACK);
}


START_TEST(class_name, "$Id: BinarySearchTree_test.C,v 1.7 2000/08/05 23:57:36 amoll Exp $")

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

initialize();
cout <<endl;
cout << "item " << &item<<endl;
cout << "left " << &left<<endl;
cout << "rleft " <<&rleft <<endl;
cout << "right " <<&right <<endl;
cout << "lright " <<&lright <<endl;
cout << "llright " <<&llright <<endl;
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
  item.setLeftChild(&left);
	TEST_EQUAL(item.getLeftChild(), &left)
	TEST_EXCEPTION(Exception::GeneralException, item.setLeftChild(&item))
	TEST_EXCEPTION(Exception::GeneralException, item.setLeftChild(&right))
RESULT

CHECK(getRightChild())
	TEST_EQUAL(item.getRightChild(), &right)
	TEST_EQUAL(rleft.getRightChild(), 0)
RESULT

CHECK(setRightChild(BSTreeItem *item))
  item.setRightChild(0);
	TEST_EQUAL(item.getRightChild(), 0)
  item.setRightChild(&right);
	TEST_EQUAL(item.getRightChild(), &right)
	TEST_EXCEPTION(Exception::GeneralException, item.setRightChild(&item))
	TEST_EXCEPTION(Exception::GeneralException, item.setRightChild(&left))
RESULT

CHECK(getSize())
	TEST_EQUAL(item.getSize(), 8)
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
	TEST_EQUAL(item.getParentOfSuccessor(), &lright)
	TEST_EQUAL(left.getParentOfSuccessor(), &left)
	TEST_EQUAL(rrright.getParentOfSuccessor(), 0)
RESULT

CHECK(detachNode(BSTreeItem*& root, BSTreeItem* t, BSTreeItem* p, bool right_side))
	BSTreeItem* x = 0;
	TEST_EQUAL(item.detachNode(x, &right, &item, true), &right)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(item.getRightChild(), &rright)
	TEST_EQUAL(item.getLeftChild(), &left)
	TEST_EQUAL(right.getRightChild(), 0) // ??? rright child von right und item
	TEST_EQUAL(right.getLeftChild(), &lright)
	TEST_EQUAL(rright.getRightChild(), &rrright)
	initialize();

	TEST_EQUAL(item.detachNode(x, &right, &item, false), &right)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(item.getLeftChild(), &rright)
	TEST_EQUAL(item.getRightChild(), &rrright) // ??? right nach wie vor child von item
	initialize();

	TEST_EQUAL(item.detachNode(x, &rright, &right, true), &rright)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(right.getRightChild(), &rrright)
	TEST_EQUAL(rright.getLeftChild(), 0)
	TEST_EQUAL(rright.getRightChild(), 0) // s.o.
	initialize();

	TEST_EQUAL(item.detachNode(x, &rrright, &rright, true), &rrright)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(rright.getRightChild(), 0)
	TEST_EQUAL(rright.getLeftChild(), 0)
	TEST_EQUAL(rrright.getRightChild(), 0)
	TEST_EQUAL(rrright.getLeftChild(), 0)
	initialize();

	TEST_EQUAL(item.detachNode(x, 0, &item, true), 0)
	TEST_EQUAL(x, 0)
	initialize();

	TEST_EQUAL(item.detachNode(x, &right, 0, false), &right)
	TEST_EQUAL(x, &rright)
	initialize();
RESULT

CHECK(rotateRight())/*
  TEST_EQUAL(right.rotateRight(), &lright)
	right = *right.rotateRight();
  TEST_EQUAL(lright.getColor(), BSTreeItem::RED)
	TEST_EQUAL(lright.getLeftChild(), &rright)
	TEST_EQUAL(lright.getRightChild(), &right)
	TEST_EQUAL(lright.getLeftChild()->getRightChild(), &rrright)

  TEST_EQUAL(lright.rotateRight(), &lright)
  TEST_EQUAL(lright.getColor(), BSTreeItem::BLACK)
	TEST_EQUAL(lright.getRightChild(), &rrright)

  TEST_EQUAL(rright.rotateRight(), &rright)
  TEST_EQUAL(rright.getColor(), BSTreeItem::BLACK)
	TEST_EQUAL(rright.getLeftChild()->getRightChild(), 0)
	initialize();*/
RESULT

CHECK(rotateLeft())/*
  TEST_EQUAL(item.rotateLeft(), &right)
	TEST_EQUAL(right.getLeftChild(), &item)
	TEST_EQUAL(item.getLeftChild(), 0)	
	TEST_EQUAL(item.getRightChild(), &lright)	
	TEST_EQUAL(right.getRightChild(), &rright)	*/
	initialize();
RESULT

BSTreeItemCollector myproc;

// von oben nach links unten, rechts und hoch
CHECK(applyPreorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyInorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyInorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPostorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPostorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyLevelorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyLevelorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPreorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPreorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyInorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyInorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPostorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPostorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(apply(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.apply(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &left)
	TEST_EQUAL(myproc.getPointer(), &rleft)
	TEST_EQUAL(myproc.getPointer(), &right)
	TEST_EQUAL(myproc.getPointer(), &lright)
	TEST_EQUAL(myproc.getPointer(), &llright)
	TEST_EQUAL(myproc.getPointer(), &rright)
	TEST_EQUAL(myproc.getPointer(), &rrright)
	TEST_EQUAL(myproc.getPointer(), 0)
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
	BSTreeItem* root = &item;
	TEST_EQUAL(item.detachMinimum(root), &left) //warum pointer angeben, warum nicht auch die addresse eines objekts???
	TEST_EQUAL(item.getLeftChild(), &rleft)
	TEST_EQUAL(left.getLeftChild(), 0)
	TEST_EQUAL(left.getRightChild(), 0) //??? rleft child von item und left

	initialize();
	root = &left;
	TEST_EQUAL(item.detachMinimum(root), &left)

	initialize();
	root = &lright;
	TEST_EQUAL(item.detachMinimum(root), &llright)

	initialize();
	root = &rleft;
	TEST_EQUAL(item.detachMinimum(root), &rleft)
RESULT

CHECK(detachMaximum(BSTreeItem*& root))
	BSTreeItem* root = &item;/*
	TEST_EQUAL(item.detachMaximum(root), &rrright) //warum pointer angeben???
	TEST_EQUAL(right.getRightChild(), &rright)
	TEST_EQUAL(rright.getRightChild(), 0)
	TEST_EQUAL(rright.getLeftChild(), 0)

	TEST_EQUAL(rrright.getRightChild(), 0)
	TEST_EQUAL(rrright.getLeftChild(), 0)

	initialize();
	root = &left;
	TEST_EQUAL(item.detachMaximum(root), &rleft)

	initialize();
	root = &lright;
	TEST_EQUAL(item.detachMaximum(root), &llright)

	initialize();
	root = &rrright;
	TEST_EQUAL(item.detachMaximum(root), &rrright)*/
RESULT

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

// tests for class TBSTreeIterator::

CHECK(TBSTreeIterator::TBSTreeIterator(const BSTreeItemType* itemType = 0))
  //BAUSTELLE
RESULT

CHECK(TBSTreeIterator::TBSTreeIterator(const BSTreeItemType* itemType, BSTreeIterator::WalkOrder walk_order))
  //BAUSTELLE
RESULT

CHECK(TBSTreeIterator::TBSTreeIterator(const TBSTreeIterator& iterator))
  //BAUSTELLE
RESULT

CHECK(TBSTreeIterator::~TBSTreeIterator())
  //BAUSTELLE
RESULT

CHECK(TBSTreeIterator::forward())
  //BAUSTELLE
RESULT

// tests for class TBSTree::

CHECK(TBSTree::BALL_CREATE(TBSTree))
  //BAUSTELLE
RESULT

CHECK(TBSTree::setComparator(const Comparator<DataType>& comparator))
  //BAUSTELLE
RESULT

CHECK(TBSTree::resetComparator())
  //BAUSTELLE
RESULT

CHECK(TBSTree::getComparator() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::getRoot() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::getHeight() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::getSize() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::getMinimum() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::getMaximum() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::find(const DataType& data) const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::count(const DataType& data) const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::insert(const DataType& data, bool multiple = true))
  //BAUSTELLE
RESULT

CHECK(TBSTree::detach(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TBSTree::detachMinimum())
  //BAUSTELLE
RESULT

CHECK(TBSTree::detachMaximum())
  //BAUSTELLE
RESULT

CHECK(TBSTree::remove(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TBSTree::removeAll(const DataType& data))
  //BAUSTELLE
RESULT

CHECK(TBSTree::removeMinimum())
  //BAUSTELLE
RESULT

CHECK(TBSTree::removeMaximum())
  //BAUSTELLE
RESULT

CHECK(TBSTree::host(Visitor<TBSTree>& visitor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::bool operator == (const TBSTree& tree) const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::bool operator != (const TBSTree& tree) const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::has(const DataType& data) const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::isEmpty() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::isValid() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::dump(std::ostream& s = std::cout, Size depth = 0) const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::applyPreorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::applyInorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::applyPostorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::applyLevelorder(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::applyPreorderFlat(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::applyInorderFlat(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::applyPostorderFlat(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::apply(UnaryProcessor<DataType>& processor))
  //BAUSTELLE
RESULT

// tests for class TBSTree::PreorderIteratorTraits_::

CHECK(TBSTree::PreorderIteratorTraits_::(PreorderIteratorTraits_)())
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::PreorderIteratorTraits_(const TBSTree& tree))
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::PreorderIteratorTraits_(const PreorderIteratorTraits_& traits))
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::PreorderIteratorTraits_& operator =  (const PreorderIteratorTraits_& traits))
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::getContainer() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::getPosition())
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::bool operator == (const PreorderIteratorTraits_& traits) const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::isValid() const )
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::toBegin())
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::toEnd())
  //BAUSTELLE
RESULT

CHECK(TBSTree::PreorderIteratorTraits_::getData() const )
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

/*
	void* ptr_a = (void*)(item.detachMinimum(root));
	void* ptr_b = (void*)&rleft;
	TEST_EQUAL(ptr_a, ptr_b)*/

