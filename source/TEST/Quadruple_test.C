// $Id: Quadruple_test.C,v 1.1 2000/12/15 13:06:13 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#	include <BALL/DATATYPE/quadruple.h>
///////////////////////////

START_TEST(class_name, "$Id: Quadruple_test.C,v 1.1 2000/12/15 13:06:13 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Quadruple::BALL_CREATE(Quadruple))
  //BAUSTELLE
RESULT

Quadruple<int, int, int, int>* q;
CHECK(Quadruple::Quadruple() throw())
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

CHECK(Quadruple::~Quadruple() throw())
	Quadruple<int, int, int, int>* q = new Quadruple<int, int, int, int>;
	delete q;
RESULT

CHECK(Quadruple::Quadruple(const Quadruple& quadruple, bool deep = true) throw())
	Quadruple<int, int, int, int> q1 = *q;
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(Quadruple::Quadruple(const first& new_first, const second& new_second, const third& new_third, const fourth& new_fourth) throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(Quadruple::clear() throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	q1.clear();
	TEST_EQUAL(q1.first, 0)
	TEST_EQUAL(q1.second, 0)
	TEST_EQUAL(q1.third, 0)
	TEST_EQUAL(q1.fourth, 0)
RESULT

CHECK(Quadruple::set(const Quadruple& quadruple) throw())
  //BAUSTELLE
RESULT

CHECK(Quadruple::Quadruple& operator = (const Quadruple& quadruple) throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(Quadruple::get(Quadruple& quadruple) const  throw())
  //BAUSTELLE
RESULT


CHECK(Quadruple::set(const first& first, const second& second, const third& third, const fourth& fourth) throw())
	Quadruple<int, int, int, int> q1;
	q1.set(1, 2, 3, 4);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
	TEST_EQUAL(q1.fourth, 4)
RESULT

CHECK(Quadruple::get(first& first, second& second, third& third, fourth& fourth) const  throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	int a, b, c, d;
	q1.get(a, b, c, d);
	TEST_EQUAL(a, 1)
	TEST_EQUAL(b, 2)
	TEST_EQUAL(c, 3)
	TEST_EQUAL(d, 4)
RESULT

CHECK(Quadruple::bool operator == (const Quadruple& quadruple) const  throw())
	Quadruple<int, int, int, int> q1 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	Quadruple<int, int, int, int> q2 = Quadruple<int, int, int, int>(1, 2, 3, 4);
	TEST_EQUAL(q1 == q2, true)
	q2.second = 22;
	TEST_EQUAL(q1 == q2, false)
RESULT

CHECK(Quadruple::bool operator != (const Quadruple& quadruple) const  throw())
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
	TEST_EQUAL(q1 >= q2, true)
	TEST_EQUAL(q2 >= q1, false)
	q2.set(1, 2, 3, 4);
	TEST_EQUAL(q1 >= q2, true)
	TEST_EQUAL(q2 >= q1, true)
	q2.set(0, 1, 2, 3);
	TEST_EQUAL(q1 >= q2, false)
	TEST_EQUAL(q2 >= q1, true)
	q1.set(0, 1, 2, 3);
	TEST_EQUAL(q1 >= q2, false)
	TEST_EQUAL(q2 >= q1, false)
	q1.set(0, 1, 2, 4);
	TEST_EQUAL(q1 >= q2, false)
	TEST_EQUAL(q2 >= q1, true)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
