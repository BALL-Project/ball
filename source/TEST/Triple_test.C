// $Id: Triple_test.C,v 1.1 2001/07/20 08:09:04 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/triple.h>
///////////////////////////

START_TEST(Triple, "$Id: Triple_test.C,v 1.1 2001/07/20 08:09:04 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Triple::BALL_CREATE(Triple))
	Triple<int, int, int> a(0, 1, 2);
	Triple<int, int, int>* v_ptr = (Triple<int, int, int>*)a.create();
	TEST_EQUAL(v_ptr->first, 0)
	delete v_ptr;
RESULT

Triple<int, int, int>* q;
CHECK(Triple::Triple() throw())
	q = new Triple<int, int, int>;
	TEST_NOT_EQUAL(0, q);
	q->first = 1;
	q->second = 2;
	q->third = 3;
	TEST_EQUAL(q->first, 1)
	TEST_EQUAL(q->second, 2)
	TEST_EQUAL(q->third, 3)
RESULT

CHECK(Triple::~Triple() throw())
	Triple<int, int, int>* q = new Triple<int, int, int>;
	delete q;
RESULT

CHECK(Triple::Triple(const Triple& triple, bool deep = true) throw())
	Triple<int, int, int> q1 = *q;
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(Triple::Triple(const first& new_first, const second& new_second, const third& new_third, const fourth& new_fourth) throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(Triple::clear() throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	q1.clear();
	TEST_EQUAL(q1.first, 0)
	TEST_EQUAL(q1.second, 0)
	TEST_EQUAL(q1.third, 0)
RESULT

CHECK(Triple::Triple& operator = (const Triple& triple) throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(Triple::set(const first& first, const second& second, const third& third, const fourth& fourth) throw())
	Triple<int, int, int> q1;
	q1.set(1, 2, 3);
	TEST_EQUAL(q1.first, 1)
	TEST_EQUAL(q1.second, 2)
	TEST_EQUAL(q1.third, 3)
RESULT

CHECK(Triple::get(first& first, second& second, third& third, fourth& fourth) const  throw())
	Triple<int, int, int> q1 = Triple<int, int, int>(1, 2, 3);
	int a, b, c;
	q1.get(a, b, c);
	TEST_EQUAL(a, 1)
	TEST_EQUAL(b, 2)
	TEST_EQUAL(c, 3)
RESULT

CHECK(Triple::bool operator == (const Triple& triple) const  throw())
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

CHECK(Triple::bool operator != (const Triple& triple) const  throw())
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
	q1.set(0, 1, 2);
	TEST_EQUAL(q1 < q2, false)
	TEST_EQUAL(q2 < q1, true)
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
	q1.set(0, 1, 2);
	TEST_EQUAL(q1 > q2, true)
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
