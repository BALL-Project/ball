// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/triple.h>
///////////////////////////

START_TEST(Triple)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BALL_CREATE(Triple))
	Triple<int, int, int> a(0, 1, 2);
	Triple<int, int, int>* v_ptr = (Triple<int, int, int>*)a.create();
	TEST_EQUAL(v_ptr->first, 0)
	delete v_ptr;
RESULT

Triple<int, int, int>* q;
CHECK(Triple() throw())
	q = new Triple<int, int, int>;
	TEST_NOT_EQUAL(0, q);
	q->first = 1;
	q->second = 2;
	q->third = 3;
	TEST_EQUAL(q->first, 1)
	TEST_EQUAL(q->second, 2)
	TEST_EQUAL(q->third, 3)
RESULT

CHECK(~Triple() throw())
	delete q;
RESULT

Triple<int, int, int> my_q;
my_q.first = 1;
my_q.second = 2;
my_q.third = 3;

CHECK(Triple(const Triple& triple, bool deep = true) throw())
	Triple<int, int, int> q1(my_q);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(Triple(const T1& new_first, const T2& new_second, const T3& new_third) throw())
	Triple<int, int, int> q1(1, 2, 3);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(void clear() throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	q1.clear();
	TEST_EQUAL(q1.first, 0)
	TEST_EQUAL(q1.second, 0)
	TEST_EQUAL(q1.third, 0)
RESULT

CHECK(const Triple& operator = (const Triple& triple) throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(void set(const T1& t1, const T2& t2, const T3& t3) throw())
	Triple<int, int, int> q1;
	q1.set(1, 2, 3);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(void get(T1& first, T2& second, T3& third) const throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	int a, b, c;
	q1.get(a, b, c);
	TEST_EQUAL(a, 1)
	TEST_EQUAL(b, 2)
	TEST_EQUAL(c, 3)
RESULT

CHECK(bool operator == (const Triple& triple) const throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	Triple<int, int, int> q2 = Triple<int, int, int>(1, 2, 3);
	TEST_EQUAL(q1 == q2, true)
	q2.second = 22;
	TEST_EQUAL(q1 == q2, false)
	q2.second = 2;
	q2.third = 22;
	TEST_EQUAL(q1 == q2, false)
	q2.third = 3;
	q2.first = 22;
	TEST_EQUAL(q1 == q2, false)
RESULT

CHECK(bool operator != (const Triple& triple) const throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	Triple<int, int, int> q2 = Triple<int, int, int>(1, 2, 3);
	TEST_EQUAL(q1 != q2, false)
	q2.second = 22;
	TEST_EQUAL(q1 != q2, true)
RESULT									

CHECK(bool operator < (const Triple& triple) const throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	Triple<int, int, int> q2 = Triple<int, int, int>(1, 2, 4);
	TEST_EQUAL(q1 < q2, true)
	TEST_EQUAL(q2 < q1, false)
	q2.set(1, 2, 3);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, false)
	q2.set(0, 1, 2);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, true)
	q1.set(0, 1, 2);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, false)
RESULT

CHECK(bool operator > (const Triple& triple) const throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	Triple<int, int, int> q2 = Triple<int, int, int>(1, 2, 4);
	TEST_EQUAL(q1 > q2, false)
	TEST_EQUAL(q2 > q1, true)
	q2.set(1, 2, 3);
	TEST_EQUAL(q1 > q2, false)
	TEST_EQUAL(q2 > q1, false)
	q2.set(0, 1, 2);
	TEST_EQUAL(q1 > q2, true)
	TEST_EQUAL(q2 > q1, false)
	q1.set(0, 1, 2);
	TEST_EQUAL(q1 > q2, false)
	TEST_EQUAL(q2 > q1, false)
RESULT

CHECK(bool operator <= (const Triple& triple) const throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	Triple<int, int, int> q2 = Triple<int, int, int>(1, 2, 4);
	TEST_EQUAL(q1 <= q2, true)
	TEST_EQUAL(q2 <= q1, false)
	q2.set(1, 2, 3);
	TEST_EQUAL(q1 <= q2, true)
	TEST_EQUAL(q2 <= q1, true)
	q2.set(0, 1, 2);
	TEST_EQUAL(q1 <= q2, false)
	TEST_EQUAL(q2 <= q1, true)
RESULT

CHECK(bool operator >= (const Triple& triple) const throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	Triple<int, int, int> q2 = Triple<int, int, int>(1, 2, 4);
	TEST_EQUAL(q1 >= q2, false)
	TEST_EQUAL(q2 >= q1, true)
	q2.set(1, 2, 3);
	TEST_EQUAL(q1 >= q2, true)
	TEST_EQUAL(q2 >= q1, true)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
