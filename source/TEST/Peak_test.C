// $Id: Peak_test.C,v 1.5 2001/07/10 10:39:24 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include<BALL/NMR/peak.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include<BALL/KERNEL/atom.h>

///////////////////////////

START_TEST(Peak, "$Id: Peak_test.C,v 1.5 2001/07/10 10:39:24 oliver Exp $")

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

CHECK(Peak1D::getPosition()/setPosition() const )
	peak.setPosition(111.1);
	TEST_REAL_EQUAL(peak.getPosition(), 111.1)
RESULT

CHECK(Peak1D::getWidth()/setWidth() const )
	peak.setWidth(222.2);
	TEST_REAL_EQUAL(peak.getWidth(), 222.2)
RESULT

CHECK(Peak1D::getIntensity() const )
	peak.setIntensity(333.3);
	TEST_REAL_EQUAL(peak.getIntensity(), 333.3)
RESULT

CHECK(Peak1D::getAtom()/setAtom() const )
	TEST_EQUAL(peak.getAtom(), 0)
	peak.setAtom(&atom);
	TEST_EQUAL(peak.getAtom(), &atom)
RESULT

CHECK(Peak1D::Peak1D(const Peak1D& peak))
	Peak1D peak2(peak);
	TEST_REAL_EQUAL(peak2.getPosition(), 111.1)
	TEST_REAL_EQUAL(peak2.getWidth(), 222.2)
	TEST_REAL_EQUAL(peak2.getIntensity(), 333.3)
	TEST_EQUAL(peak2.getAtom(), &atom)
RESULT

CHECK(Peak1D::void operator = (const Peak1D& peak))
	Peak1D peak2 = peak;
	TEST_REAL_EQUAL(peak2.getPosition(), 111.1)
	TEST_REAL_EQUAL(peak2.getWidth(), 222.2)
	TEST_REAL_EQUAL(peak2.getIntensity(), 333.3)
	TEST_EQUAL(peak2.getAtom(), &atom)
RESULT

CHECK(Peak1D::bool operator == (const Peak1D& peak) const )
	Peak1D peak2 = peak;
	Peak1D peak3 = peak;
	TEST_EQUAL(peak2 == peak, true)
	TEST_EQUAL(peak2 == peak3, true)
	peak3.setWidth(0.0);
	TEST_EQUAL(peak2 == peak, true)
	TEST_EQUAL(peak2 == peak3, false)
	peak3.setWidth(peak2.getWidth());
	TEST_EQUAL(peak2 == peak3, true)
	peak3.setIntensity(0.0);
	TEST_EQUAL(peak2 == peak3, false)
	peak3.setIntensity(peak2.getIntensity());
	TEST_EQUAL(peak2 == peak3, true)
	peak3.setPosition(0.0);
	TEST_EQUAL(peak2 == peak3, false)
	peak3.setPosition(peak2.getPosition());
	TEST_EQUAL(peak2 == peak3, true)
	peak3.setAtom(0);
	TEST_EQUAL(peak2 == peak3, false)
	peak3.setAtom(peak2.getAtom());
	TEST_EQUAL(peak2 == peak3, true)
RESULT

CHECK(Peak1D::bool operator < (const Peak1D& peak) const )
	Peak1D peak2(peak);
	TEST_EQUAL(peak2 < peak, false)
	TEST_EQUAL(peak < peak2, false)
	peak2.setPosition(peak.getPosition() + 1.0);
	TEST_EQUAL(peak2 < peak, false)
	TEST_EQUAL(peak < peak2, true)
RESULT

CHECK(Peak1D::bool operator > (const Peak1D& peak) const )
	Peak1D peak2(peak);
	TEST_EQUAL(peak2 > peak, false)
	TEST_EQUAL(peak > peak2, false)
	peak2.setPosition(peak.getPosition() + 1.0);
	TEST_EQUAL(peak2 > peak, true)
	TEST_EQUAL(peak > peak2, false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
