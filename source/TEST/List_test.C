// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: List_test.C,v 1.7 2003/06/17 13:20:01 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/list.h>
#include <BALL/CONCEPT/visitor.h>
#include "ItemCollector.h"
///////////////////////////
using namespace BALL;
using namespace std;

class myVisitor 
	: public  Visitor<List<int> >
{
	public:

	int value;

	void visit(List<int>& list)
	{
		value = list.front();
	}
};

START_TEST(List<T>, "$Id: List_test.C,v 1.7 2003/06/17 13:20:01 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


List<int>*	lst_ptr;
CHECK(List() throw())
	lst_ptr = new List<int>;
	TEST_NOT_EQUAL(lst_ptr, 0)
RESULT

CHECK(~List() throw())
	delete lst_ptr;
RESULT

CHECK(Size getSize() const throw())
	List<int> int_list;
	TEST_EQUAL(int_list.getSize(), 0)
	int_list.push_back(1);
	TEST_EQUAL(int_list.getSize(), 1)
	int_list.push_back(2);
	TEST_EQUAL(int_list.getSize(), 2)
RESULT

CHECK(List(const List& new_list, bool /* deep = true */) throw())
	List<int> int_list;
	int_list.push_back(1);
	int_list.push_back(2);
	int_list.push_back(3);
	int_list.push_back(4);
	List<int> new_list(int_list);
	TEST_EQUAL(new_list.size(), int_list.size())
	TEST_EQUAL(new_list.front(), int_list.front())
	TEST_EQUAL(new_list.back(), int_list.back())
	int_list.clear();
	List<int> new_list2(int_list);
	TEST_EQUAL(new_list2.size(), int_list.size())
RESULT

List<int> int_list;

CHECK(void set(const List& list, bool /* deep */ = true) throw())
	List<int> int_list2;
	int_list.push_back(1);
	int_list.push_back(2);
	int_list.push_back(3);
	int_list.push_back(4);
	int_list2.set(int_list);
	TEST_EQUAL(int_list2.size(), 4)
RESULT


CHECK(const List& operator = (const List& list) throw())
	List<int> int_list2;
	int_list2 = int_list;
	TEST_EQUAL(int_list2.size(), 4)
RESULT


CHECK(void get(List& list, bool deep = true) const throw())
	List<int> int_list2;
	int_list.get(int_list2);
	TEST_EQUAL(int_list2.size(), 4)
RESULT

CHECK(void swap(List& list) throw())
	List<int> int_list2;
	int_list2.swap(int_list);
	TEST_EQUAL(int_list.size(), 0)
	TEST_EQUAL(int_list2.size(), 4)
RESULT

CHECK(bool isEmpty() const throw())
	TEST_EQUAL(int_list.isEmpty(), true)
	int_list.push_back(1);
	TEST_EQUAL(int_list.isEmpty(), false)	
RESULT

CHECK(bool remove(const Value& item) throw())
	List<int> int_list2;
	int_list2.push_back(1);
	int_list2.push_back(2);
	int_list2.push_back(3);
	bool result = int_list2.remove(2);
	TEST_EQUAL(result, true);
	TEST_EQUAL(int_list2.size(), 2);
	List<int>::Iterator it = int_list2.begin();
	TEST_EQUAL(*it, 1);
	++it;
	TEST_EQUAL(*it, 3);
RESULT

List<int> int_list1;

CHECK(bool operator == (const List<Value>& list) const throw())
	int_list1.push_back(1);
	int_list1.push_back(2);
	int_list1.push_back(3);
	List<int> int_list2(int_list1);

	TEST_EQUAL(int_list1 == int_list2, true);

	int_list2.remove(2);
	TEST_EQUAL(int_list1.size(), int_list2.size() +1)
	TEST_EQUAL(int_list1 == int_list2, false);

	int_list2.push_back(2);
	TEST_EQUAL(int_list1 == int_list2, false);

	int_list2.remove(3);
	int_list2.push_back(3);
	TEST_EQUAL(int_list1 == int_list2, true);
RESULT

CHECK(bool operator != (const List<Value>& list) const throw())
	int_list1.clear();
	int_list1.push_back(1);
	int_list1.push_back(2);
	int_list1.push_back(3);
	List<int> int_list2(int_list1);

	TEST_EQUAL(int_list1 != int_list2, false);

	int_list2.remove(2);
	TEST_EQUAL(int_list1.size(), int_list2.size() +1)
	TEST_EQUAL(int_list1 != int_list2, true);

	int_list2.push_back(2);
	TEST_EQUAL(int_list1 != int_list2, true);

	int_list2.remove(3);
	int_list2.push_back(3);
	TEST_EQUAL(int_list1 != int_list2, false);
RESULT


CHECK(void host(Visitor<List<Value> >& visitor) throw())
	myVisitor mv;
	TEST_EQUAL(int_list1.front(), 1);
	int_list1.host(mv);
	
	TEST_EQUAL(mv.value, 1)
RESULT


CHECK(bool apply(UnaryProcessor<Value>& processor) throw())
	ItemCollector<int> myproc;
	int_list1.apply(myproc);

	TEST_EQUAL(myproc.getSize(), 3)
	TEST_EQUAL(int_list1.size(), 3)

	TEST_EQUAL(*myproc.getPointer(), 1) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 2) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 3) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(void destroy() throw())
	List<int> int_list2;
	int_list.push_back(1);
	int_list.push_back(2);
	int_list.destroy();
	TEST_EQUAL(int_list.size(), 0)
RESULT

CHECK(BALL_CREATE_DEEP(List))
	List<int> int_list2;
	int_list.push_back(1);
	List<int>* l_ptr = (List<int>*)int_list.create(false, true);
	TEST_EQUAL(l_ptr->getSize(), 0)
	delete l_ptr;
	l_ptr = (List<int>*)int_list.create();
	TEST_EQUAL(l_ptr->getSize(), 1)
	delete l_ptr;
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
