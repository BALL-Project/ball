// $Id: Line3_test.C,v 1.5 2000/05/26 19:25:02 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#	include <BALL/MATHS/vector3.h>
#	include <BALL/MATHS/line3.h>
///////////////////////////

START_TEST(class_name, "$Id: Line3_test.C,v 1.5 2000/05/26 19:25:02 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Vector3 v1, v2, v3, v4;

String filename;
using std::ofstream;
using std::ios;

CHECK(TLine3::BALL_CREATE(TLine3<T>))
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

CHECK(TLine3::TLine3())
  Line3* line;
	line = new Line3();
	TEST_NOT_EQUAL(line, 0)
RESULT

CHECK(TLine3::~TLine3())
  Line3* line;
	line = new Line3();
	delete line;
RESULT

Line3 line, line1, line2, line3;

CHECK(TLine3(const TVector3<T>& point, const TVector3<T>& vector, 
					 Form form = FORM__PARAMETER))
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line.get(v3, v4, Line3::FORM__PARAMETER);
	TEST_EQUAL(v1, v3)
	TEST_EQUAL(v2, v4)

	line = Line3(v1, v2, Line3::FORM__TWO_POINTS);
	line.get(v3, v4, Line3::FORM__TWO_POINTS);
	TEST_EQUAL(v1, v3)
	TEST_EQUAL(v2, v4)
RESULT

CHECK(bool operator ==(const TLine3& line) const )
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(0, 4, 2);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	line  = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line == line2, true)
	v2 = Vector3(0, 1, 2);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line == line2, false)
RESULT

CHECK(TLine3::swap(TLine3& line))
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

CHECK(TLine3::set(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER))
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line2.set(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line, line2)
	line = Line3(v1, v2, Line3::FORM__TWO_POINTS);
	line2.set(v1, v2, Line3::FORM__TWO_POINTS);
	TEST_EQUAL(line, line2)
RESULT

CHECK(TLine3::get(TLine3& line, bool /* deep */ = true) const )
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line2.get(line);
	TEST_EQUAL(line, line2)
RESULT

CHECK(normalize())
	v1 = Vector3(0, 0, 0);
	v2 = Vector3(4, 9, 16);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	line.normalize();
	v2.normalize();
	line.get(v3, v4, Line3::FORM__PARAMETER);
	TEST_EQUAL(v2, v4)
RESULT

CHECK(bool operator != (const TLine3& line) const )
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(0, 3, 2);
	line  = Line3(v1, v2, Line3::FORM__PARAMETER);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line != line2, false)
	v2 = Vector3(9, 1, 2);
	line2 = Line3(v1, v2, Line3::FORM__PARAMETER);
	TEST_EQUAL(line != line2, true)
RESULT

CHECK(has(const TVector3<T>& point) const )
	v1 = Vector3(0, 0, 0);
	v2 = Vector3(4, 4, 4);
	line = Line3(v1, v2, Line3::FORM__TWO_POINTS);
	v1 = Vector3(2, 2, 2);
	TEST_EQUAL(line.has(v1), true)
	v1 = Vector3(2, 2, 3);
	TEST_EQUAL(line.has(v1), false)
RESULT

CHECK(isValid() const)
	TEST_EQUAL(line.isValid(), true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TLine3<T>& line))
	std::ifstream instr("data/Line_test2.txt");
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
	TEST_FILE(filename.c_str(), "data/Line_test2.txt", false)
RESULT

CHECK(TAngle::dump(std::ostream& s = std::cout, Size depth = 0) const )
	v1 = Vector3(0, 1, 2);
	v2 = Vector3(3, 4, 5);
	line = Line3(v1, v2, Line3::FORM__PARAMETER);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	line.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Line_test.txt", true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
