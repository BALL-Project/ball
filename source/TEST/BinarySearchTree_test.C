// $Id: BinarySearchTree_test.C,v 1.11 2000/08/08 06:59:19 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/binarySearchTree.h>
#include <iostream>
#include <BALL/CONCEPT/comparator.h>
#include <BALL/DATATYPE/list.h>
///////////////////////////

using namespace BALL;

// helper class: a processor counting tree items
template<typename DataType>
class ItemCollector	
	: public UnaryProcessor<DataType>
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

	Processor::Result operator () (DataType& item)
	{	// store the item
		list_.push_back(&item);
		return Processor::CONTINUE;
	}

	List<DataType*> getList()
	{ // get a pointer to the list
		return list_;
	}
	
	DataType* getPointer()
	{	// get a pointer to the first element in the list
		if (list_it_ == list_.end())
		{
			return 0;
		}
		DataType* temp = *list_it_;
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
	List<DataType*>											list_;
	typename List<DataType*>::Iterator	list_it_;
};


//			     	  	   		item
//	    			leftitem              rightitem
//               rleftitem    lrightitem    rrightitem
//                      llrightitem         rrrightitem

BSTreeItem item, leftitem, rightitem, rleftitem, lrightitem, llrightitem, rrightitem, rrrightitem;

void initialize()
{
	item  = BSTreeItem(&leftitem, &rightitem, BSTreeItem::RED);
	leftitem  = BSTreeItem(0, &rleftitem, BSTreeItem::RED);
	rleftitem = BSTreeItem(0, 0, BSTreeItem::BLACK);
	rightitem = BSTreeItem(&lrightitem, &rrightitem, BSTreeItem::BLACK);
	lrightitem = BSTreeItem(&llrightitem, 0, BSTreeItem::RED);
	llrightitem = BSTreeItem(0, 0, BSTreeItem::RED);
	rrightitem = BSTreeItem(0, &rrrightitem, BSTreeItem::BLACK);
	rrrightitem = BSTreeItem(0, 0, BSTreeItem::BLACK);
}

TBSTreeItem<int> item_, leftitem_, rightitem_, rleftitem_, lrightitem_, llrightitem_, rrightitem_, rrrightitem_;

void initialize_()
{
	item_  = TBSTreeItem<int>(1, &leftitem_, &rightitem_, BSTreeItem::RED);
	leftitem_  = TBSTreeItem<int>(2, 0, &rleftitem_, BSTreeItem::RED);
	rleftitem_ = TBSTreeItem<int>(3, 0, 0, BSTreeItem::BLACK);
	rightitem_ = TBSTreeItem<int>(4, &lrightitem_, &rrightitem_, BSTreeItem::BLACK);
	lrightitem_ = TBSTreeItem<int>(5, &llrightitem_, 0, BSTreeItem::RED);
	llrightitem_ = TBSTreeItem<int>(6, 0, 0, BSTreeItem::RED);
	rrightitem_ = TBSTreeItem<int>(7, 0, &rrrightitem_, BSTreeItem::BLACK);
	rrrightitem_ = TBSTreeItem<int>(8, 0, 0, BSTreeItem::BLACK);
}

START_TEST(class_name, "$Id: BinarySearchTree_test.C,v 1.11 2000/08/08 06:59:19 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace std;
{
BSTreeItem* itemp;
CHECK(BSTreeItem())
	itemp = new BSTreeItem();
	TEST_NOT_EQUAL(itemp, 0)
RESULT

CHECK(~BSTreeItem())
	delete itemp;
RESULT

CHECK(BSTreeItem(BSTreeItem* leftitem_item, BSTreeItem* rightitem_item, char color = BSTreeItem::BLACK))
  BSTreeItem* item;
  BSTreeItem* item2;
  BSTreeItem* item3;
	item2 = new BSTreeItem(0, 0, BSTreeItem::RED);
	item3 = new BSTreeItem(0, 0, BSTreeItem::RED);
	item = new BSTreeItem(item2, item3, BSTreeItem::RED);
	TEST_NOT_EQUAL(item, 0)
	TEST_EQUAL(item->getLeftChild(), item2)
	TEST_EQUAL(item->getRightChild(), item3)
RESULT

initialize();
cout <<endl;
cout << "item " << &item<<endl;
cout << "leftitem " << &leftitem<<endl;
cout << "rleftitem " <<&rleftitem <<endl;
cout << "rightitem " <<&rightitem <<endl;
cout << "lrightitem " <<&lrightitem <<endl;
cout << "llrightitem " <<&llrightitem <<endl;
cout << "rrightitem " <<&rrightitem <<endl;
cout << "rrrightitem " <<&rrrightitem <<endl;
cout <<endl;

initialize_();
cout <<endl;
cout << "item_ " << &item_<<endl;
cout << "leftitem_ " << &leftitem_<<endl;
cout << "rleftitem_ " <<&rleftitem_ <<endl;
cout << "rightitem_ " <<&rightitem_ <<endl;
cout << "lrightitem_ " <<&lrightitem_ <<endl;
cout << "llrightitem_ " <<&llrightitem_ <<endl;
cout << "rrightitem_ " <<&rrightitem_ <<endl;
cout << "rrrightitem_ " <<&rrrightitem_ <<endl;
cout <<endl;


CHECK(BSTreeItem(const BSTreeItem& item, bool /* deep */= true))
  BSTreeItem* item1;
	item1 = new BSTreeItem(item);
	TEST_NOT_EQUAL(item1, 0)
	TEST_EQUAL(item1->getLeftChild() , &leftitem)
	TEST_EQUAL(item1->getRightChild(), &rightitem)
RESULT

CHECK(getLeftChild())
	TEST_EQUAL(item.getLeftChild(), &leftitem)
	TEST_EQUAL(leftitem.getLeftChild(), 0)
RESULT

CHECK(setLeftChild(BSTreeItem* item))
  item.setLeftChild(0);
	TEST_EQUAL(item.getLeftChild(), 0)
  item.setLeftChild(&leftitem);
	TEST_EQUAL(item.getLeftChild(), &leftitem)
	TEST_EXCEPTION(Exception::GeneralException, item.setLeftChild(&item))
	TEST_EXCEPTION(Exception::GeneralException, item.setLeftChild(&rightitem))
	//!!!! Du kannst nicht einfach auf GeneralException testen!
	//!!!! Du musst die genaue Exception-Klasse angeben, sonst klappt das nicht.
RESULT

CHECK(getRightChild())
	TEST_EQUAL(item.getRightChild(), &rightitem)
	TEST_EQUAL(rleftitem.getRightChild(), 0)
RESULT

CHECK(setRightChild(BSTreeItem *item))
  item.setRightChild(0);
	TEST_EQUAL(item.getRightChild(), 0)
  item.setRightChild(&rightitem);
	TEST_EQUAL(item.getRightChild(), &rightitem)
	TEST_EXCEPTION(Exception::GeneralException, item.setRightChild(&item))
	TEST_EXCEPTION(Exception::GeneralException, item.setRightChild(&leftitem))
	//!!!! Du kannst nicht einfach auf GeneralException testen!
	//!!!! Du musst die genaue Exception-Klasse angeben, sonst klappt das nicht.
RESULT

CHECK(getSize())
	TEST_EQUAL(item.getSize(), 8)
	TEST_EQUAL(rleftitem.getSize(), 1)
RESULT

CHECK(getHeight())
	TEST_EQUAL(item.getHeight(), 4)
	TEST_EQUAL(rleftitem.getHeight(), 1)
RESULT

CHECK(getMinimum())
	TEST_EQUAL(item.getMinimum(), &leftitem)
	TEST_EQUAL(leftitem.getMinimum(), &leftitem)
RESULT

CHECK(getMaximum())
	TEST_EQUAL(item.getMaximum(), &rrrightitem)
	TEST_EQUAL(rrrightitem.getMaximum(), &rrrightitem)
RESULT

CHECK(getParentOfMinimum())
	TEST_EQUAL(item.getParentOfMinimum(), &item)
	TEST_EQUAL(rrrightitem.getParentOfMinimum(), 0)
RESULT

CHECK(getParentOfMaximum())
	TEST_EQUAL(item.getParentOfMaximum(), &rrightitem)
	TEST_EQUAL(rrrightitem.getParentOfMaximum(), 0)
RESULT

CHECK(getParentOfPredecessor())
	TEST_EQUAL(item.getParentOfPredecessor(), &leftitem)
	TEST_EQUAL(leftitem.getParentOfPredecessor(), 0)
RESULT

CHECK(getParentOfSuccessor())
	TEST_EQUAL(item.getParentOfSuccessor(), &lrightitem)
	TEST_EQUAL(leftitem.getParentOfSuccessor(), &leftitem)
	TEST_EQUAL(rrrightitem.getParentOfSuccessor(), 0)
RESULT

CHECK(detachNode(BSTreeItem*& root, BSTreeItem* t, BSTreeItem* p, bool rightitem_side))
	BSTreeItem* x = 0;
	TEST_EQUAL(item.detachNode(x, &rightitem, &item, true), &rightitem)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(item.getRightChild(), &rrightitem)
	TEST_EQUAL(item.getLeftChild(), &leftitem)
	TEST_EQUAL(rightitem.getRightChild(), 0) // ??? rrightitem child von rightitem und item
	TEST_EQUAL(rightitem.getLeftChild(), &lrightitem)
	TEST_EQUAL(rrightitem.getRightChild(), &rrrightitem)
	initialize();

	TEST_EQUAL(item.detachNode(x, &rightitem, &item, false), &rightitem)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(item.getLeftChild(), &rrightitem)
	TEST_EQUAL(item.getRightChild(), &rrrightitem) // ??? rightitem nach wie vor child von item
	initialize();

	TEST_EQUAL(item.detachNode(x, &rrightitem, &rightitem, true), &rrightitem)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(rightitem.getRightChild(), &rrrightitem)
	TEST_EQUAL(rrightitem.getLeftChild(), 0)
	TEST_EQUAL(rrightitem.getRightChild(), 0) // s.o.
	initialize();

	TEST_EQUAL(item.detachNode(x, &rrrightitem, &rrightitem, true), &rrrightitem)
	TEST_EQUAL(x, 0)
	TEST_EQUAL(rrightitem.getRightChild(), 0)
	TEST_EQUAL(rrightitem.getLeftChild(), 0)
	TEST_EQUAL(rrrightitem.getRightChild(), 0)
	TEST_EQUAL(rrrightitem.getLeftChild(), 0)
	initialize();

	TEST_EQUAL(item.detachNode(x, 0, &item, true), 0)
	TEST_EQUAL(x, 0)
	initialize();

	TEST_EQUAL(item.detachNode(x, &rightitem, 0, false), &rightitem)
	TEST_EQUAL(x, &rrightitem)
	initialize();
RESULT

CHECK(rotateRight())
  TEST_EQUAL(rightitem.rotateRight(), &lrightitem)
	TEST_EQUAL(item.getRightChild(), &lrightitem)
	TEST_EQUAL(lrightitem.getLeftChild(), &llrightitem)
	TEST_EQUAL(lrightitem.getRightChild(), &rightitem)	
	TEST_EQUAL(rightitem.getLeftChild(), 0)
	TEST_EQUAL(rightitem.getRightChild(), &rrightitem)	
	initialize();
RESULT

CHECK(rotateLeft())
  TEST_EQUAL(rightitem.rotateLeft(), &rrightitem)
	TEST_EQUAL(item.getRightChild(), &rrightitem)
	TEST_EQUAL(rrightitem.getLeftChild(), &rightitem)
	TEST_EQUAL(rrightitem.getRightChild(), &rrrightitem)	
	TEST_EQUAL(rightitem.getLeftChild(), &lrightitem)
	TEST_EQUAL(rightitem.getRightChild(), 0)	
	initialize();
RESULT

ItemCollector<BSTreeItem> myproc;

// von oben nach links unten, rechts und hoch
CHECK(applyPreorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyInorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyInorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPostorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPostorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyLevelorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyLevelorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPreorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPreorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyInorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyInorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPostorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPostorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(apply(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.apply(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item)
	TEST_EQUAL(myproc.getPointer(), &leftitem)
	TEST_EQUAL(myproc.getPointer(), &rleftitem)
	TEST_EQUAL(myproc.getPointer(), &rightitem)
	TEST_EQUAL(myproc.getPointer(), &lrightitem)
	TEST_EQUAL(myproc.getPointer(), &llrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrightitem)
	TEST_EQUAL(myproc.getPointer(), &rrrightitem)
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(isLeaf() const )
	TEST_EQUAL(item.isLeaf(), false)
	TEST_EQUAL(leftitem.isLeaf(), false)
	TEST_EQUAL(rleftitem.isLeaf(), true)
RESULT

CHECK(isInterior() const )
	TEST_EQUAL(item.isInterior(), true)
	TEST_EQUAL(leftitem.isInterior(), true)
	TEST_EQUAL(rleftitem.isInterior(), false)
RESULT

CHECK(insertBalance(BSTreeItem* root, BSTreeItem::Pack& pp))
  //BAUSTELLE
RESULT

CHECK(removeBalance(BSTreeItem* root, BSTreeItem::Pack& pp))
//	TEST_EQUAL(item.removeBalance(item, pack), 0)
RESULT

CHECK(replace(BSTreeItem* root, BSTreeItem::Pack& pp))
  //BAUSTELLE
RESULT

CHECK(detachMinimum(BSTreeItem*& root))
	BSTreeItem* root = &item;
	TEST_EQUAL(item.detachMinimum(root), &leftitem) //warum pointer angeben, warum nicht auch die addresse eines objekts???
	TEST_EQUAL(item.getLeftChild(), &rleftitem)
	TEST_EQUAL(leftitem.getLeftChild(), 0)
	TEST_EQUAL(leftitem.getRightChild(), 0) //??? rleftitem child von item und leftitem
	initialize();

	root = &leftitem;
	TEST_EQUAL(item.detachMinimum(root), &leftitem)
	initialize();

	root = &lrightitem;
	TEST_EQUAL(item.detachMinimum(root), &llrightitem)
	initialize();

	root = &rleftitem;
	TEST_EQUAL(item.detachMinimum(root), &rleftitem)
	initialize();
RESULT

CHECK(detachMaximum(BSTreeItem*& root))
//	BSTreeItem* root = &item;
//	TEST_EQUAL(item.detachMaximum(root), &rrrightitem) 
//warum pointer angeben???
// =>segfault
/*	TEST_EQUAL(rightitem.getRightChild(), &rrightitem)
	TEST_EQUAL(rrightitem.getRightChild(), 0)
	TEST_EQUAL(rrightitem.getLeftChild(), 0)

	TEST_EQUAL(rrrightitem.getRightChild(), 0)
	TEST_EQUAL(rrrightitem.getLeftChild(), 0)

	initialize();
	root = &leftitem;
	TEST_EQUAL(item.detachMaximum(root), &rleftitem)

	initialize();
	root = &lrightitem;
	TEST_EQUAL(item.detachMaximum(root), &llrightitem)

	initialize();
	root = &rrrightitem;
	TEST_EQUAL(item.detachMaximum(root), &rrrightitem)*/
RESULT
}
// tests for class TBSTreeItem::
{
TBSTreeItem<int>* tbsitem;
CHECK(TBSTreeItem())
	tbsitem = new TBSTreeItem<int>;
	TEST_NOT_EQUAL(tbsitem, 0)
	tbsitem->getData();
	TEST_EQUAL(tbsitem->getLeftChild(), 0)
	TEST_EQUAL(tbsitem->getRightChild(), 0)
RESULT

CHECK(~TBSTreeItem())
  delete tbsitem;
RESULT

// !!!!!
// das sind wieder Pointer die wild in der Gegend rumzeigen - die 
// muessen auf irgendwas zeigen!
TBSTreeItem<int>* it;
TBSTreeItem<int>* l;
TBSTreeItem<int>* r;

CHECK(TBSTreeItem(const DataType& data, TBSTreeItem* leftitem_item, 
								 TBSTreeItem* rightitem_item, char color = BSTreeItem::BLACK))
  it = new TBSTreeItem<int>(0, l, 0);
	// HIER gibt's naemlich sons spaetestens Probleme!!!!!
	TEST_EQUAL(it->getRightChild(), 0)
	delete it;
  l = new TBSTreeItem<int>(999, 0, 0);
  r = new TBSTreeItem<int>(9999, 0, 0);
  it = new TBSTreeItem<int>(99, l, r, BSTreeItem::RED);
	TEST_EQUAL(it->getData(), 99)
	TEST_EQUAL(it->getLeftChild(), l)
	TEST_EQUAL(it->getRightChild(), r)
	TEST_EQUAL(it->getColor(), (char)BSTreeItem::RED)
RESULT

CHECK(BALL_CREATE_NODEEP(TBSTreeItem))
	TBSTreeItem<int>* v_ptr = (TBSTreeItem<int>*)it->create();
	TEST_EQUAL(v_ptr->getData(), 99)
	TEST_EQUAL(v_ptr->getLeftChild(), l)
	TEST_EQUAL(v_ptr->getRightChild(), r)
	TEST_EQUAL(v_ptr->getColor(), (char)BSTreeItem::RED)
	delete v_ptr;
RESULT

CHECK(getData())
	TEST_EQUAL(it->getData(), 99)	  
RESULT

CHECK(getLeftChild())
	TEST_EQUAL(it->getLeftChild(), l)
	TEST_EQUAL(it->getLeftChild()->getData(), 999)
  it = new TBSTreeItem<int>(0, 0, 0);
	TEST_EQUAL(it->getLeftChild(), 0)
RESULT

CHECK(getRightChild())
  it = new TBSTreeItem<int>(0, l, r);
	TEST_EQUAL(it->getRightChild(), r)
	TEST_EQUAL(it->getRightChild()->getData(), 9999)
  it = new TBSTreeItem<int>(0, l, 0);
	TEST_EQUAL(it->getRightChild(), 0)
RESULT

CHECK(setLeftChild())
  it = new TBSTreeItem<int>();
	it->setLeftChild(0);
	TEST_EQUAL(it->getLeftChild(), 0)
	it->setLeftChild(l);
	TEST_EQUAL(it->getLeftChild(), l)
RESULT

CHECK(setRightChild())
  it = new TBSTreeItem<int>();
	it->setRightChild(l);
	TEST_EQUAL(it->getRightChild(), l)
	it->setRightChild(0);
	TEST_EQUAL(it->getRightChild(), 0)
RESULT

CHECK(count(const DataType& data, const Comparator<DataType>* comparator))
	initialize_();	// warum umbedingt Comparator notwendig ???
	Comparator<int>* comp;
	TEST_EXCEPTION(Exception::NullPointer, item_.count(1, 0))
	comp = new Comparator<int>();
	TEST_EQUAL(item_.count(1, comp), 1)
	TEST_EQUAL(item_.count(999, comp), 0)
RESULT

//ItemCollector< TBSTreeItem<int> > myproc;
ItemCollector< int > myproc;

CHECK(applyPreorder)
	initialize();
	myproc.start();
	item_.applyPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(*(myproc.getPointer()), 1)
	TEST_EQUAL(*(myproc.getPointer()), 2)
	TEST_EQUAL(*(myproc.getPointer()), 3)
	TEST_EQUAL(*(myproc.getPointer()), 4)
	TEST_EQUAL(*(myproc.getPointer()), 5)
	TEST_EQUAL(*(myproc.getPointer()), 6)
	TEST_EQUAL(*(myproc.getPointer()), 7)
	TEST_EQUAL(*(myproc.getPointer()), 8)
	TEST_EQUAL(myproc.getPointer(), 0)
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
}
// tests for class BSTreeIterator::

BSTreeIterator* it;

CHECK(BSTreeIterator::BSTreeIterator(const BSTreeItem* item = 0, WalkOrder walk_order = BSTreeIterator::WALK_ORDER__PREORDER))
	it = new BSTreeIterator();
	TEST_NOT_EQUAL(it, 0)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__PREORDER);
	TEST_NOT_EQUAL(it, 0)
RESULT

CHECK(BSTreeIterator::BSTreeIterator(const BSTreeIterator& iterator))
	BSTreeIterator* it2;
	it2 = new BSTreeIterator(*it);
	TEST_NOT_EQUAL(it2, 0)
RESULT

CHECK(BSTreeIterator::~BSTreeIterator())
	BSTreeIterator* it2;
	it2 = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__PREORDER);
  delete it2;
RESULT

BSTreeIterator it2;

CHECK(TBSTreeItem::set(const BSTreeIterator& iterator))
	it2.set(*it);
RESULT

CHECK(TBSTreeItem::set(const BSTreeItem* item, WalkOrder walk_order = BSTreeIterator::WALK_ORDER__PREORDER))
	it2.set(&item);
RESULT

CHECK(BSTreeIterator& operator = (const BSTreeIterator& iterator))
	it2 = *it;
RESULT

CHECK(clear())
	it2 = *it;
	it2.clear();
RESULT

CHECK(TBSTreeItem::forward())
	TEST_EQUAL(it->forward(), &leftitem)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__INORDER);
	TEST_EQUAL(it->forward(), &leftitem)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__POSTORDER);
	TEST_EQUAL(it->forward(), &leftitem)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(it->forward(), &leftitem)
RESULT

CHECK(TBSTreeItem::bool operator == (const BSTreeIterator& iterator) const )
	it2.set(*it);
	TEST_EQUAL(*it == it2, true)
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it == it2, false)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__INORDER); // Segfault ???

/*	TEST_EQUAL(it->forward(), &leftitem)
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it == it2, false)*/
RESULT

CHECK(TBSTreeItem::bool operator != (const BSTreeIterator& iterator) const )/*
	it2.set(*it);
	TEST_EQUAL(*it != it2, false)
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it != it2, true)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__INORDER);
	it->forward();
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it != it2, true)*/
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
	void* ptr_b = (void*)&rleftitem;
	TEST_EQUAL(ptr_a, ptr_b)*/

