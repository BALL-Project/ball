// $Id: Surface_test.C,v 1.1 2001/12/27 00:27:29 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/surface.h>

///////////////////////////

START_TEST(Surface, "$Id: Surface_test.C,v 1.1 2001/12/27 00:27:29 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


CHECK(Surface()	throw())
RESULT

CHECK(Surface(const Surface& surface)	throw())
RESULT

CHECK(~Surface() throw())
RESULT

CHECK(void set(const Surface& box) throw())
RESULT

CHECK(const Surface& operator = (const Surface& box) throw())
RESULT

CHECK(void get(Surface& box) const throw())
RESULT

CHECK(void clear() throw())
RESULT

CHECK(void readMSMSFile(const String& vert_filename, const String& face_filename) throw(Exception::FileNotFound))
RESULT

CHECK(float getArea() const throw())
RESULT

CHECK(bool operator == (const Surface& surface) const throw())
RESULT

CHECK(bool operator != (const Surface& surface) const throw())
RESULT

CHECK(bool isValid() const throw())
RESULT
											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
