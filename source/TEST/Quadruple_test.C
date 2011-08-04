// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#	include <BALL/DATATYPE/quadruple.h>
///////////////////////////

START_TEST(Quadruple)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BALL_CREATE(Quadruple))
	Quadruple<int, int, int, int> a(0, 1, 2, 3);
	Quadruple<int, int, int, int>* v_ptr = (Quadruple<int, int, int, int>*)a.create();
	TEST_EQUAL(v_ptr->first, 0)
	delete v_ptr;
RESULT

Quadruple<int, int, int, int>* q;
CHECK(Quadruple() throw())
	q = new Quadruple<int, int, int, int>;
	TEST_NOT_EQUAL(0, q);
	q->first = 1;
	q->second = 2;
	q->third = 3;
	q->fourth = 4;
	TEST_EQUAL(q->first, 1)
	TEST_EQUAL(q->second, 2)
	TEST_EQUAL(q->third, 3)
	TEST_EQUAL(q->fourth, 4)
RESULT

CHECK(~Quadruple() throw())
	delete q;
RESULT

Quadruple<int, int, int, int> my_q;
my_q.first = 1;
my_q.second = 2;
my_q.third = 3;
my_q.fourth = 4;

CHECK(Quadruple(const Quadruple& quadruple, bool deep = true) throw())
	Quadruple<int, int, int, int> q1(my_q);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(Quadruple(const T1& new_first, const T2& new_second, const T3& new_third, const T4& new_fourth) throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(void clear() throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	q1.clear();
	TEST_EQUAL(q1.first, 0)
	TEST_EQUAL(q1.second, 0)
	TEST_EQUAL(q1.third, 0)
	TEST_EQUAL(q1.fourth, 0)
RESULT

CHECK(const Quadruple& operator = (const Quadruple& quadruple) throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(void set(const T1& t1, const T2& t2, const T3& t3, const T4& t4) throw())
	Quadruple<int, int, int, int> q1;
	q1.set(1, 2, 3, 4);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(void get(T1& first, T2& second, T3& third, T4& fourth) const throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	int a, b, c, d;
	q1.get(a, b, c, d);
	TEST_EQUAL(a, 1)
	TEST_EQUAL(b, 2)
	TEST_EQUAL(c, 3)
	TEST_EQUAL(d, 4)
RESULT

CHECK(bool operator == (const Quadruple& quadruple) const throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	Quadruple<int, int, int, int> q2 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	TEST_EQUAL(q1 == q2, true)
	q2.second = 22;
	TEST_EQUAL(q1 == q2, false)
RESULT

CHECK(bool operator != (const Quadruple& quadruple) const throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	Quadruple<int, int, int, int> q2 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	TEST_EQUAL(q1 != q2, false)
	q2.second = 22;
	TEST_EQUAL(q1 != q2, true)
RESULT									

CHECK(bool operator < (const Quadruple& quadruple) const throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	Quadruple<int, int, int, int> q2 = Quadruple<int, int, int, int>(1, 2, 4, 3);
	TEST_EQUAL(q1 < q2, true)
	TEST_EQUAL(q2 < q1, false)
	q2.set(1, 2, 3, 4);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, false)
	q2.set(0, 1, 2, 3);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, true)
	q1.set(0, 1, 2, 3);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, false)
	q1.set(0, 1, 2, 4);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, true)
RESULT

CHECK(bool operator > (const Quadruple& quadruple) const throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	Quadruple<int, int, int, int> q2 = Quadruple<int, int, int, int>(1, 2, 4, 3);
	TEST_EQUAL(q1 > q2, false)
	TEST_EQUAL(q2 > q1, true)
	q2.set(1, 2, 3, 4);
	TEST_EQUAL(q1 > q2, false)
	TEST_EQUAL(q2 > q1, false)
	q2.set(0, 1, 2, 3);
	TEST_EQUAL(q1 > q2, true)
	TEST_EQUAL(q2 > q1, false)
	q1.set(0, 1, 2, 3);
	TEST_EQUAL(q1 > q2, false)
	TEST_EQUAL(q2 > q1, false)
	q1.set(0, 1, 2, 4);
	TEST_EQUAL(q1 > q2, true)
	TEST_EQUAL(q2 > q1, false)
RESULT

CHECK(bool operator <= (const Quadruple& quadruple) const throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	Quadruple<int, int, int, int> q2 = Quadruple<int, int, int, int>(1, 2, 4, 3);
	TEST_EQUAL(q1 <= q2, true)

	TEST_EQUAL(q2 <= q1, false)
	q2.set(1, 2, 3, 4);
	TEST_EQUAL(q1 <= q2, true)
	TEST_EQUAL(q2 <= q1, true)
	q2.set(0, 1, 2, 3);
	TEST_EQUAL(q1 <= q2, false)
	TEST_EQUAL(q2 <= q1, true)
	q1.set(0, 1, 2, 4);
	TEST_EQUAL(q1 <= q2, false)
	TEST_EQUAL(q2 <= q1, true)
RESULT

CHECK(bool operator >= (const Quadruple& quadruple) const throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	Quadruple<int, int, int, int> q2 = Quadruple<int, int, int, int>(1, 2, 4, 3);
	TEST_EQUAL(q1 >= q2, false)
	TEST_EQUAL(q2 >= q1, true)
	q2.set(1, 2, 3, 4);
	TEST_EQUAL(q1 >= q2, true)
	TEST_EQUAL(q2 >= q1, true)
	q2.set(0, 1, 2, 3);
	TEST_EQUAL(q1 >= q2, true)
	TEST_EQUAL(q2 >= q1, false)
	q1.set(0, 1, 2, 4);
	TEST_EQUAL(q1 >= q2, true)
	TEST_EQUAL(q2 >= q1, false)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
