// $Id: Peak_test.C,v 1.3 2000/09/22 12:16:49 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include<BALL/NMR/peak.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include<BALL/KERNEL/atom.h>

///////////////////////////

START_TEST(Peak, "$Id: Peak_test.C,v 1.3 2000/09/22 12:16:49 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Peak1D* p;

CHECK(Peak1D::Peak1D())
	p = new Peak1D();
	TEST_NOT_EQUAL(p, 0)
RESULT

CHECK(Peak1D::~Peak1D())
  delete p;
RESULT

Peak1D peak;
const Atom atom;

	PRECISION(0.0001)

CHECK(Peak1D::getValue()/setValue() const )
	peak.setValue(111.1);
	TEST_REAL_EQUAL(peak.getValue(), 111.1)
RESULT

CHECK(Peak1D::getWidth()/setWidth() const )
	peak.setWidth(222.2);
	TEST_REAL_EQUAL(peak.getWidth(), 222.2)
RESULT

CHECK(Peak1D::getHeight() const )
	peak.setHeight(333.3);
	TEST_REAL_EQUAL(peak.getHeight(), 333.3)
RESULT

CHECK(Peak1D::getAtom()/setAtom() const )
	TEST_EQUAL(peak.getAtom(), 0)
	peak.setAtom(&atom);
	TEST_EQUAL(peak.getAtom(), &atom)
RESULT

CHECK(Peak1D::Peak1D(const Peak1D& peak))
	Peak1D peak2(peak);
	TEST_REAL_EQUAL(peak2.getValue(), 111.1)
	TEST_REAL_EQUAL(peak2.getWidth(), 222.2)
	TEST_REAL_EQUAL(peak2.getHeight(), 333.3)
	TEST_EQUAL(peak2.getAtom(), &atom)
RESULT

CHECK(Peak1D::void operator = (const Peak1D& peak))
	Peak1D peak2 = peak;
	TEST_REAL_EQUAL(peak2.getValue(), 111.1)
	TEST_REAL_EQUAL(peak2.getWidth(), 222.2)
	TEST_REAL_EQUAL(peak2.getHeight(), 333.3)
	TEST_EQUAL(peak2.getAtom(), &atom)
RESULT

CHECK(Peak1D::bool operator == (const Peak1D& peak) const )
  //BAUSTELLE
RESULT

CHECK(Peak1D::bool operator < (const Peak1D& peak) const )
  //BAUSTELLE
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
