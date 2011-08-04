// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#	include <BALL/MATHS/vector3.h>
#	include <BALL/MATHS/line3.h>
///////////////////////////

START_TEST(Line3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Vector3 v1, v2, v3, v4;

String filename;

CHECK(BALL_CREATE(TLine3<T>))
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	Line3 v(v1, v2);
	Line3* v_ptr = (Line3*)v.create(false, true);
	TEST_EQUAL(v_ptr->p, v3)
	TEST_EQUAL(v_ptr->d, v3)
	delete v_ptr;
	v_ptr = (Line3*)v.create();
	TEST_EQUAL(v_ptr->p, v1)
	TEST_EQUAL(v_ptr->d, v2)
	delete v_ptr;
RESULT

Line3* line3_ptr = 0;
CHECK(TLine3() throw())
	line3_ptr = new Line3;
	TEST_NOT_EQUAL(line3_ptr, 0)
RESULT

CHECK(~TLine3() throw())
	delete line3_ptr;
RESULT

Line3 line, line1, line2, line3;

CHECK(TLine3(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER) throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line.get(v3, v4, Line3::FORM__PARAMETER);
	TEST_EQUAL(v1, v3)
	TEST_EQUAL(v2, v4)

	line = Line3(v1, v2, Line3::FORM__TWO_POINTS);
	line.get(v1, v2);
	v3.set(0, 1, 2);
	v4.set(3, 3, 3);
	TEST_EQUAL(v1, v3)
	TEST_EQUAL(v2, v4)
RESULT

CHECK(bool operator == (const TLine3& line) const throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(0, 4, 2);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	line  = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line == line2, true)
	v2 = Vector3(0, 1, 2);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line == line2, false)
RESULT

CHECK(void swap(TLine3& line) throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	v3 = Vector3(6, 7, 8);
	v4 = Vector3(9, 10, 11);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line1 = Line3(v1, v2, Line3::FORM__PARAMETER);

	line2 = Line3(v3, v4, Line3::FORM__PARAMETER);
	line3 = Line3(v3, v4, Line3::FORM__PARAMETER);

	line.swap(line2);
	TEST_EQUAL(line1, line2)
	TEST_EQUAL(line, line3)
RESULT

CHECK(void set(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER) throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line2.set(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line, line2)
	line = Line3(v1, v2, Line3::FORM__TWO_POINTS);
	line2.set(v1, v2, Line3::FORM__TWO_POINTS);
	TEST_EQUAL(line, line2)
RESULT

CHECK(void get(TLine3& line) const throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line2.get(line);
	TEST_EQUAL(line, line2)
RESULT

CHECK(void normalize() throw())
	v1 = Vector3(0, 0, 0);
	v2 = Vector3(4, 9, 16);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line.normalize();
	v2.normalize();
	line.get(v3, v4, Line3::FORM__PARAMETER);
	TEST_EQUAL(v2, v4)
RESULT

CHECK(bool operator != (const TLine3& line) const throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(0, 3, 2);
	line  = Line3(v1, v2, Line3::FORM__PARAMETER);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line != line2, false)
	v2 = Vector3(9, 1, 2);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line != line2, true)
RESULT

CHECK(bool has(const TVector3<T>& point) const throw())
	v1 = Vector3(0, 0, 0);
	v2 = Vector3(4, 4, 4);
	line = Line3(v1, v2, Line3::FORM__TWO_POINTS);
	v1 = Vector3(2, 2, 2);
	TEST_EQUAL(line.has(v1), true)
	v1 = Vector3(2, 2, 3);
	TEST_EQUAL(line.has(v1), false)
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(line.isValid(), true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TLine3<T>& line))
	std::ifstream instr(BALL_TEST_DATA_PATH(Line_test2.txt));
	line = Line3();
	instr >> line;
	instr.close();
	v3 = Vector3(0, 1, 2);
	v4 = Vector3(3, 4, 5);
	line2 = Line3(v3, v4, Line3::FORM__PARAMETER);
	TEST_EQUAL(line, line2)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TLine3<T>& line))
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << line;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Line_test2.txt))
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	line.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Line_test.txt))
RESULT

CHECK(TLine3(const TLine3& line) throw())
	Line3 line2(line);
	TEST_EQUAL(line2, line)
RESULT

CHECK(TLine3& operator = (const TLine3& line) throw())
	Line3 line2 = line;
	TEST_EQUAL(line2, line)
RESULT

CHECK(void clear() throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line.clear();
	Line3 line2;
	TEST_EQUAL(line, line2)
RESULT

CHECK(void get(TVector3<T>& point, TVector3<T>& vector, Form form = FORM__PARAMETER) const throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	Vector3 a,b;
	line.get(a, b, Line3::FORM__PARAMETER);
	TEST_EQUAL(a, v1)
	TEST_EQUAL(b, v2)

	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__TWO_POINTS);
	line.get(a, b, Line3::FORM__TWO_POINTS);
	TEST_EQUAL(a, v1)
	TEST_EQUAL(b, v2)
RESULT

CHECK(void set(const TLine3& line) throw())
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	Line3 line2;
	line2.set(line);
	TEST_EQUAL(line2, line)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
