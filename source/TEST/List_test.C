// $Id: List_test.C,v 1.5 2000/12/01 14:57:48 amoll Exp $
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

START_TEST(List<T>, "$Id: List_test.C,v 1.5 2000/12/01 14:57:48 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


List<int>*	lst_ptr;
CHECK(List<int>::List())											
	lst_ptr = new List<int>;
	TEST_NOT_EQUAL(lst_ptr, 0)
RESULT

CHECK(List<int>::~List())
	delete lst_ptr;
RESULT

CHECK(List<int>::getSize())
	List<int> int_list;
	TEST_EQUAL(int_list.getSize(), 0)
	int_list.push_back(1);
	TEST_EQUAL(int_list.getSize(), 1)
	int_list.push_back(2);
	TEST_EQUAL(int_list.getSize(), 2)
RESULT

CHECK(List<int>::List(const List&))
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

CHECK(set)
	List<int> int_list2;
	int_list.push_back(1);
	int_list.push_back(2);
	int_list.push_back(3);
	int_list.push_back(4);
	int_list2.set(int_list);
	TEST_EQUAL(int_list2.size(), 4)
RESULT


CHECK(operator = )
	List<int> int_list2;
	int_list2 = int_list;
	TEST_EQUAL(int_list2.size(), 4)
RESULT

CHECK(get)
	List<int> int_list2;
	int_list.get(int_list2);
	TEST_EQUAL(int_list2.size(), 4)
RESULT

CHECK(swap)
	List<int> int_list2;
	int_list2.swap(int_list);
	TEST_EQUAL(int_list.size(), 0)
	TEST_EQUAL(int_list2.size(), 4)
RESULT

CHECK(isEmpty)
	TEST_EQUAL(int_list.isEmpty(), true)
	int_list.push_back(1);
	TEST_EQUAL(int_list.isEmpty(), false)	
RESULT

CHECK(remove)
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

CHECK(operator ==/!=)
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


CHECK(host(Visitor<List<Value> >& visitor))
	myVisitor mv;
	TEST_EQUAL(int_list1.front(), 1);
	int_list1.host(mv);
	
	TEST_EQUAL(mv.value, 1)
RESULT

CHECK(apply(UnaryProcessor<List<Value> >& processor))
	ItemCollector<int> myproc;
	int_list1.apply(myproc);

	TEST_EQUAL(myproc.getSize(), 3)
	TEST_EQUAL(int_list1.size(), 3)

	TEST_EQUAL(*myproc.getPointer(), 1) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 2) myproc.forward();
	TEST_EQUAL(*myproc.getPointer(), 3) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
