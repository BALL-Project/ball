// $Id: BinarySearchTree_test.C,v 1.14 2000/08/21 18:30:09 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/binarySearchTree.h>
#include <iostream>
#include <../source/TEST/ItemCollector.h>
///////////////////////////

using namespace BALL;

//			     	  	   		item
//	    			leftitem              rightitem
//               rleftitem    lrightitem    rrightitem
//                      llrightitem         rrrightitem

BSTreeItem item, leftitem, rightitem, rleftitem, lrightitem, llrightitem, rrightitem, rrrightitem;

void initialize()
{
	item  = BSTreeItem(&leftitem, &rightitem, (char) BSTreeItem::RED);
	leftitem  = BSTreeItem(0, &rleftitem, (char) BSTreeItem::RED);
	rleftitem = BSTreeItem(0, 0, (char) BSTreeItem::BLACK);
	rightitem = BSTreeItem(&lrightitem, &rrightitem, (char) BSTreeItem::BLACK);
	lrightitem = BSTreeItem(&llrightitem, 0, (char) BSTreeItem::RED);
	llrightitem = BSTreeItem(0, 0, (char) BSTreeItem::RED);
	rrightitem = BSTreeItem(0, &rrrightitem, (char) BSTreeItem::BLACK);
	rrrightitem = BSTreeItem(0, 0, (char) BSTreeItem::BLACK);
}

TBSTreeItem<int> item_, leftitem_, rightitem_, rleftitem_, lrightitem_, llrightitem_, rrightitem_, rrrightitem_;

void initialize_()
{
	item_  = TBSTreeItem<int>(1, &leftitem_, &rightitem_, (char) BSTreeItem::RED);
	leftitem_  = TBSTreeItem<int>(2, 0, &rleftitem_, (char) BSTreeItem::RED);
	rleftitem_ = TBSTreeItem<int>(3, 0, 0, (char) BSTreeItem::BLACK);
	rightitem_ = TBSTreeItem<int>(4, &lrightitem_, &rrightitem_, (char) BSTreeItem::BLACK);
	lrightitem_ = TBSTreeItem<int>(5, &llrightitem_, 0, (char) BSTreeItem::RED);
	llrightitem_ = TBSTreeItem<int>(6, 0, 0, (char) BSTreeItem::RED);
	rrightitem_ = TBSTreeItem<int>(7, 0, &rrrightitem_, (char) BSTreeItem::BLACK);
	rrrightitem_ = TBSTreeItem<int>(8, 0, 0, (char) BSTreeItem::BLACK);
}

START_TEST(class_name, "$Id: BinarySearchTree_test.C,v 1.14 2000/08/21 18:30:09 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace std;
{
BSTreeItem* itemp = 0;
CHECK(BSTreeItem())
	itemp = new BSTreeItem();
	TEST_NOT_EQUAL(itemp, 0)
RESULT

CHECK(~BSTreeItem())
	delete itemp;
RESULT

CHECK(BSTreeItem(BSTreeItem* left_item, BSTreeItem* rightitem_item, char color = (char) BSTreeItem::BLACK))
  BSTreeItem* item;
  BSTreeItem* item2;
  BSTreeItem* item3;
	item2 = new BSTreeItem(0, 0, (char) BSTreeItem::RED);
	item3 = new BSTreeItem(0, 0, (char) BSTreeItem::RED);
	item = new BSTreeItem(item2, item3, (char) BSTreeItem::RED);
	TEST_NOT_EQUAL(item, 0)
	TEST_EQUAL(item->getLeftChild(), item2)
	TEST_EQUAL(item->getRightChild(), item3)
	TEST_EQUAL(item->getColor(), (char)(char) BSTreeItem::RED)
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

CHECK(BSTreeItem(const BSTreeItem& item, bool = true))
  BSTreeItem* item1 = 0;
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
	TEST_EXCEPTION(Exception::IllegalTreeOperation, item.setLeftChild(&item))
	TEST_EXCEPTION(Exception::IllegalTreeOperation, item.setLeftChild(&rightitem))
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
	TEST_EXCEPTION(Exception::IllegalTreeOperation, item.setRightChild(&item))
	TEST_EXCEPTION(Exception::IllegalTreeOperation, item.setRightChild(&leftitem))
RESULT

CHECK(getColor())
	TEST_EQUAL(item.getColor(), (char) BSTreeItem::RED)
	TEST_EQUAL(rleftitem.getColor(), (char) BSTreeItem::BLACK)
RESULT

CHECK(setColor())
	item.setColor((char) BSTreeItem::BLACK);
	TEST_EQUAL(item.getColor(), (char) BSTreeItem::BLACK)
	item.setColor((char) BSTreeItem::RED);
	TEST_EQUAL(item.getColor(), (char) BSTreeItem::RED)
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

CHECK(detachNode(BSTreeItem*& root, BSTreeItem* t, BSTreeItem* p, bool right_side))
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
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyInorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyInorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPostorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPostorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyLevelorder(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyLevelorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPreorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPreorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyInorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyInorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(applyPostorderFlat(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.applyPostorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(apply(UnaryProcessor<BSTreeItem>& processor))
	myproc.start();
	item.apply(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(myproc.getPointer(), &item) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &leftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &lrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &llrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrightitem) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &rrrightitem) myproc.forward();
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

TBSTreeItem<int>* tbsitem = 0;
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


TBSTreeItem<int>* it = 0;
TBSTreeItem<int>* l = 0;
TBSTreeItem<int>* r = 0;

CHECK(TBSTreeItem(const DataType& data, TBSTreeItem* left_item, 
								 TBSTreeItem* rightitem_item, char color = (char) BSTreeItem::BLACK))
  it = new TBSTreeItem<int>(0, l, 0);
	TEST_EQUAL(it->getRightChild(), 0)
	delete it;
  l = new TBSTreeItem<int>(999, 0, 0);
  r = new TBSTreeItem<int>(9999, 0, 0);
  it = new TBSTreeItem<int>(99, l, r, (char) BSTreeItem::RED);
	TEST_EQUAL(it->getData(), 99)
	TEST_EQUAL(it->getLeftChild(), l)
	TEST_EQUAL(it->getRightChild(), r)
	TEST_EQUAL(it->getColor(), (char) BSTreeItem::RED)
RESULT

CHECK(BALL_CREATE_NODEEP(TBSTreeItem))
	TBSTreeItem<int>* v_ptr = (TBSTreeItem<int>*)it->create();
	TEST_EQUAL(v_ptr->getData(), 99)
	TEST_EQUAL(v_ptr->getLeftChild(), l)
	TEST_EQUAL(v_ptr->getRightChild(), r)
	TEST_EQUAL(v_ptr->getColor(), (char) BSTreeItem::RED)
	delete v_ptr;
RESULT

CHECK(getData())
	TEST_EQUAL(it->getData(), 99)	  
RESULT

CHECK(setData(const DataType& data))
	it->setData(999);	
	TEST_EQUAL(it->getData(), 999)	
	it->setData(99);
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
	initialize_();
	Comparator<int>* comp = 0;
	TEST_EXCEPTION(Exception::NullPointer, item_.count(1, 0))
	comp = new Comparator<int>();
	TEST_EQUAL(item_.count(1, comp), 1)
	TEST_EQUAL(item_.count(999, comp), 0)
RESULT

ItemCollector< int > myproc;

CHECK(applyPreorder)
	initialize();
	myproc.start();
	item_.applyPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 8)
	TEST_EQUAL(*(myproc.getPointer()), 1) myproc.forward();
	TEST_EQUAL(*(myproc.getPointer()), 2) myproc.forward();
	TEST_EQUAL(*(myproc.getPointer()), 3) myproc.forward();
	TEST_EQUAL(*(myproc.getPointer()), 4) myproc.forward();
	TEST_EQUAL(*(myproc.getPointer()), 5) myproc.forward();
	TEST_EQUAL(*(myproc.getPointer()), 6) myproc.forward();
	TEST_EQUAL(*(myproc.getPointer()), 7) myproc.forward();
	TEST_EQUAL(*(myproc.getPointer()), 8) myproc.forward();
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
{
BSTreeIterator* it = 0;

CHECK(BSTreeIterator::BSTreeIterator(const BSTreeItem* item = 0, WalkOrder walk_order = BSTreeIterator::WALK_ORDER__PREORDER))
	it = new BSTreeIterator();
	TEST_NOT_EQUAL(it, 0)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__PREORDER);
	TEST_NOT_EQUAL(it, 0)
RESULT

CHECK(BSTreeIterator::BSTreeIterator(const BSTreeIterator& iterator))
	BSTreeIterator* it2 = 0;
	it2 = new BSTreeIterator(*it);
	TEST_NOT_EQUAL(it2, 0)
RESULT

CHECK(BSTreeIterator::~BSTreeIterator())
	BSTreeIterator* it2 = 0;
	it2 = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__PREORDER);
  delete it2;
RESULT

BSTreeIterator it2;

CHECK(BSTreeItem::set(const BSTreeIterator& iterator))
	it2.set(*it);
RESULT

CHECK(BSTreeItem::set(const BSTreeItem* item, WalkOrder walk_order = BSTreeIterator::WALK_ORDER__PREORDER))
	it2.set(&item);
RESULT

CHECK(BSTreeIterator& operator = (const BSTreeIterator& iterator))
	it2 = *it;
RESULT

CHECK(clear())
	it2 = *it;
	it2.clear();
RESULT

CHECK(forward())
	TEST_EQUAL(it->forward(), &item)
	TEST_EQUAL(it->forward(), &leftitem)
	TEST_EQUAL(it->forward(), &rleftitem)
	TEST_EQUAL(it->forward(), &rightitem)
	delete it;
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__INORDER);
	TEST_EQUAL(it->forward(), &leftitem)
	TEST_EQUAL(it->forward(), &rleftitem)
	TEST_EQUAL(it->forward(), &item)

	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__POSTORDER);
	TEST_EQUAL(it->forward(), &rleftitem)
	TEST_EQUAL(it->forward(), &leftitem)
	TEST_EQUAL(it->forward(), &llrightitem)

	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(it->forward(), &item)
	TEST_EQUAL(it->forward(), &leftitem)
	TEST_EQUAL(it->forward(), &rightitem)
	TEST_EQUAL(it->forward(), &rleftitem)
RESULT

CHECK(BSTreeItem::bool operator == (const BSTreeIterator& iterator) const )
	it2.set(*it);
	TEST_EQUAL(*it == it2, true)
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it == it2, false)
	BSTreeIterator* it3;
//	it3 = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__INORDER); // Segfault ???
/*	TEST_EQUAL(it->forward(), &leftitem)
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it == it2, false)*/
RESULT

CHECK(BSTreeItem::bool operator != (const BSTreeIterator& iterator) const )/*
	it2.set(*it);
	TEST_EQUAL(*it != it2, false)
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it != it2, true)
	it = new BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__INORDER);
	it->forward();
	it2 = BSTreeIterator(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(*it != it2, true)*/
RESULT
}
// tests for class TBSTreeIterator::
{/*
TBSTreeIterator<int>* it;

CHECK(TBSTreeIterator::TBSTreeIterator(const BSTreeItemType* itemType = 0))
	it = new TBSTreeIterator<int>();
	TEST_NOT_EQUAL(it, 0)
	it = new TBSTreeIterator<int>(&item);
	TEST_NOT_EQUAL(it, 0)
RESULT

CHECK(TBSTreeIterator::TBSTreeIterator(const BSTreeItemType* itemType, BSTreeIterator::WalkOrder walk_order))
	it = new TBSTreeIterator<int>(&item_, BSTreeIterator::WALK_ORDER__PREORDER);
	TEST_NOT_EQUAL(it, 0)
RESULT

CHECK(TBSTreeIterator::TBSTreeIterator(const TBSTreeIterator& iterator))
	TBSTreeIterator<int>* it2 = 0;
	it2 = new TBSTreeIterator<int>(*it);
	TEST_NOT_EQUAL(it2, 0)
RESULT

CHECK(TBSTreeIterator::~TBSTreeIterator())
	TBSTreeIterator<int>* it2 = 0;
	it2 = new TBSTreeIterator<int>(&item, BSTreeIterator::WALK_ORDER__PREORDER);
  delete it2;
RESULT

CHECK(TBSTreeIterator::forward())
	TEST_EQUAL(it->forward(), &leftitem)
	delete it;
	it = new TBSTreeIterator<int>(&item, BSTreeIterator::WALK_ORDER__INORDER);
	TEST_EQUAL(it->forward(), &leftitem)
	it = new TBSTreeIterator<int>(&item, BSTreeIterator::WALK_ORDER__POSTORDER);
	TEST_EQUAL(it->forward(), &leftitem)
	it = new TBSTreeIterator<int>(&item, BSTreeIterator::WALK_ORDER__LEVELORDER);
	TEST_EQUAL(it->forward(), &leftitem)
RESULT*/
// TBSTreeIterator unnoetig ???

}
// tests for class TBSTree::

TBSTree<int>* t = 0;
const Comparator<int> comp = Comparator<int>();
const Comparator<int>* xxxx = &(RTTI::getDefault<Comparator<int> >());

CHECK(RTTI::getDefault<Comparator<int> >())
	TEST_NOT_EQUAL(xxxx, 0)
RESULT

CHECK(TBSTree())
	t = new TBSTree<int>();
	TEST_NOT_EQUAL(t, 0)
RESULT

CHECK(~TBSTree())
	delete t;
RESULT

CHECK(TBSTree(const TBSTree& tree, bool deep = true))/*
	t->insert(1);
	TBSTree<int>* t2 = 0;
	t = new TBSTree<int>(*t);
	TEST_NOT_EQUAL(t2, 0)
	TEST_EQUAL(t2->getSize(), 1)*/
RESULT

CHECK(TBSTree::BALL_CREATE(TBSTree))/*
	TBSTreeItem<int>* v_ptr = (TBSTreeItem<int>*)t->create();
	TEST_EQUAL(v_ptr->getSize(), 1)
	delete v_ptr;*/
RESULT

TBSTree<int> tree, tree2;

CHECK(TBSTree::setComparator(const Comparator<DataType>& comparator))
  tree.setComparator(comp);
	TEST_EQUAL(tree.getComparator(), &comp)
RESULT

CHECK(TBSTree::resetComparator())
  tree.resetComparator();
RESULT

CHECK(TBSTree::getComparator() const )
  tree.setComparator(comp);
	TEST_EQUAL(tree.getComparator(), &comp)
RESULT

CHECK(TBSTree::getRoot() const )
	TEST_EQUAL(tree.getRoot(), 0)
	tree.insert(1);
	TEST_NOT_EQUAL(tree.getRoot(), 0)
	if (tree.getRoot() != 0)
	{
		TEST_EQUAL(tree.getRoot()->getData(), 1)
	}
RESULT

CHECK(TBSTree::getHeight() const )
	tree.insert(2);
	tree.insert(3);
	tree.insert(4);
	TEST_EQUAL(tree.getHeight(), 4)
	TEST_EQUAL(tree2.getHeight(), 0)
RESULT

CHECK(TBSTree::getSize() const )
	TEST_EQUAL(tree.getSize(), 4)
	TEST_EQUAL(tree2.getSize(), 0)
RESULT

CHECK(TBSTree::clear())
	tree2.insert(12);
	tree2.clear();
	TEST_EQUAL(tree2.getSize(), 0)
	TEST_EQUAL(tree2.getRoot(), 0)
RESULT

CHECK(TBSTree::destroy())
	tree2.insert(12);
	tree2.destroy();
	TEST_EQUAL(tree2.getSize(), 0)
	TEST_EQUAL(tree2.getRoot(), 0)
RESULT

CHECK(TBSTree::set(const TBSTree& tree, bool deep = true))
	tree2.set(tree, true);
	TEST_EQUAL(tree2.getSize(), 4)
	TEST_EQUAL(tree2.getRoot()->getData(), tree2.getRoot()->getData())
	TEST_EQUAL(tree2.getComparator(), tree2.getComparator())
RESULT

CHECK(TBSTree::set(const TBSTree& tree, BSTreeIterator::WalkOrder walk_order))
	tree2.clear();
	tree2.set(tree, BSTreeIterator::WALK_ORDER__POSTORDER);
	TEST_EQUAL(tree2.getSize(), 4)
	TEST_EQUAL(tree2.getRoot()->getData(), tree2.getRoot()->getData())
	TEST_EQUAL(tree2.getComparator(), tree2.getComparator())
RESULT

CHECK(TBSTree::operator = (const TBSTree& tree))
	tree2.clear();
	tree2 = tree;
	TEST_EQUAL(tree2.getSize(), 4)
	TEST_EQUAL(tree2.getRoot()->getData(), tree2.getRoot()->getData())
	TEST_EQUAL(tree2.getComparator(), tree2.getComparator())
RESULT

CHECK(TBSTree::get(TBSTree& tree, bool deep = true) const)
	tree2.clear();
	tree.get(tree2, true);
	TEST_EQUAL(tree2.getSize(), 4)
	TEST_EQUAL(tree2.getRoot()->getData(), tree2.getRoot()->getData())
	TEST_EQUAL(tree2.getComparator(), tree2.getComparator())
RESULT

CHECK(TBSTree::get(TBSTree& tree, BSTreeIterator::WalkOrder walk_order) const)
	tree2.clear();
	tree.get(tree2, BSTreeIterator::WALK_ORDER__POSTORDER);
	TEST_EQUAL(tree2.getSize(), 4)
	TEST_EQUAL(tree2.getRoot()->getData(), tree2.getRoot()->getData())
	TEST_EQUAL(tree2.getComparator(), tree2.getComparator())
RESULT

CHECK(TBSTree::swap(TBSTree &tree))
	tree2.clear();
	tree2.swap(tree);
	TEST_EQUAL(tree2.getSize(), 4)
	TEST_EQUAL(tree.getSize(), 4)
	tree2.swap(tree);
RESULT

CHECK(TBSTree::getMinimum() const )
	TEST_EQUAL(*(tree.getMinimum()), 1)
	TEST_EQUAL(tree2.getMinimum(), 0)
RESULT

CHECK(TBSTree::getMaximum() const )
	TEST_EQUAL(*(tree.getMaximum()), 4)
	TEST_EQUAL(tree2.getMaximum(), 0)
RESULT

CHECK(TBSTree::find(const DataType& data) const )
	TEST_EQUAL(tree.find(5), 0)
	TEST_NOT_EQUAL(tree.find(4), 0)
	TEST_EQUAL(tree2.find(5), 0)
	TEST_EQUAL(tree2.find(0), 0)
RESULT

CHECK(TBSTree::count(const DataType& data) const )
	TEST_EQUAL(tree.count(5), 0)
	TEST_EQUAL(tree.count(1), 1)
RESULT

CHECK(TBSTree::insert(const DataType& data, bool multiple = true))
	tree2.clear();
	tree2.insert(1, false);
	tree2.insert(1, false);
	TEST_EQUAL(tree2.count(1), 1)
	tree2.insert(1);
	TEST_EQUAL(tree2.count(1), 2)
RESULT

CHECK(TBSTree::detach(const DataType& data))
	tree2.insert(2);
	TEST_EQUAL(tree2.detach(3), 0)
	TEST_NOT_EQUAL(tree2.detach(2)->getData(), 2)
	TEST_EQUAL(tree2.count(2), 1)
RESULT

CHECK(TBSTree::detachMinimum())
	tree2.clear();
	TEST_EQUAL(tree2.detachMinimum(), 0)
	TEST_EQUAL(tree.detachMinimum()->getData(), 1)
	TEST_EQUAL(tree.count(1), 0)
RESULT

CHECK(TBSTree::detachMaximum())
	TEST_EQUAL(tree2.detachMaximum(), 0)
	TEST_EQUAL(tree.detachMaximum()->getData(), 4)
	TEST_EQUAL(tree.count(4), 0)
RESULT

CHECK(TBSTree::remove(const DataType& data))
	tree2.clear();
	tree2.insert(2);
	TEST_EQUAL(tree2.remove(3), false)
	TEST_EQUAL(tree2.remove(2), true)
	TEST_EQUAL(tree2.count(2), 0)
RESULT

CHECK(TBSTree::removeAll(const DataType& data))
	tree2.insert(1);
	tree2.insert(2);
	tree2.insert(3);
	TEST_EQUAL(tree2.getSize(), 0)
RESULT

CHECK(TBSTree::removeMinimum())
	tree2.clear();
	TEST_EQUAL(tree2.removeMinimum(), false)
	tree2.insert(1);
	tree2.insert(2);
	tree2.insert(3);
	TEST_EQUAL(tree2.removeMinimum(), true)
	TEST_EQUAL(tree2.count(1), 0)
RESULT

CHECK(TBSTree::removeMaximum())
	tree2.clear();
	TEST_EQUAL(tree2.removeMaximum(), false)
	tree2.insert(1);
	tree2.insert(2);
	tree2.insert(3);
	TEST_EQUAL(tree2.removeMaximum(), true)
	TEST_EQUAL(tree2.count(3), 0)
RESULT

CHECK(TBSTree::host(Visitor<TBSTree>& visitor))
  //BAUSTELLE
RESULT

CHECK(TBSTree::bool operator == (const TBSTree& tree) const )
	tree2.clear();
	tree2.insert(1);
	tree2.insert(2);
	tree2.insert(3);
	tree.clear();
	tree.insert(3);
	tree.insert(1);
	tree.insert(2);
	TEST_EQUAL(tree == tree2, true)
	tree2.insert(1);
	TEST_EQUAL(tree == tree2, false)
	tree2.remove(1);
	TEST_EQUAL(tree == tree2, true)
RESULT

CHECK(TBSTree::bool operator != (const TBSTree& tree) const )
	TEST_EQUAL(tree != tree2, false)
	tree2.insert(1);
	TEST_EQUAL(tree != tree2, true)
RESULT

CHECK(TBSTree::has(const DataType& data) const )
	TEST_EQUAL(tree.has(1), true)
	TEST_EQUAL(tree.has(4), false)
RESULT

CHECK(TBSTree::isEmpty() const )
	tree2.clear();
	TEST_EQUAL(tree2.isEmpty(), true)
	tree2.insert(1);
	TEST_EQUAL(tree2.isEmpty(), false)
RESULT

CHECK(TBSTree::isValid() const )
	TEST_EQUAL(tree.isValid(), true)
RESULT

CHECK(TBSTree::dump(std::ostream& s = std::cout, Size depth = 0) const )
	tree.insert(4);
	tree.insert(5);
	tree.insert(6);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	tree.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/BinarySearchTree_test.txt", true)
RESULT

ItemCollector<int> myproc;

CHECK(TBSTree::applyPreorder(UnaryProcessor<DataType>& processor))
	myproc.start();
	tree.applyPostorderFlat(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 6)
//	TEST_EQUAL(myproc.getPointer(), &rleftitem) myproc.forward();
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
