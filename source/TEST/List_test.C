// $Id: List_test.C,v 1.2 2000/01/15 19:01:34 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/list.h>

///////////////////////////

START_TEST(List<T>, "$Id: List_test.C,v 1.2 2000/01/15 19:01:34 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

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
int_list.push_back(3);
TEST_EQUAL(int_list.getSize(), 3)
int_list.push_back(4);
TEST_EQUAL(int_list.getSize(), 4)
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
