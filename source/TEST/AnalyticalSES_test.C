// $Id: AnalyticalSES_test.C,v 1.5 2000/06/15 17:24:08 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/analyticalSES.h>
#include <BALL/KERNEL/fragment.h>
///////////////////////////

START_TEST(AnalyticalSES, "$Id: AnalyticalSES_test.C,v 1.5 2000/06/15 17:24:08 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(calculateSESArea(const BaseFragment& fragment, float probe_radius))
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	float area = calculateSESArea(f, 1.5);

	PRECISION(0.001)
	TEST_REAL_EQUAL(area, 25.13274)

	a2.setPosition(Vector3(1.0, 0.0, 0.0));

	area = calculateSESArea(f, 1.5);
	TEST_REAL_EQUAL(area, 18.722)
RESULT

CHECK(calculateSESVolume(const BaseFragment& fragment, float probe_radius))
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	float volume = calculateSESVolume(f, 1.5);

	PRECISION(0.001)
	TEST_REAL_EQUAL(volume, 8.37758)

	a2.setPosition(Vector3(1.0, 0.0, 0.0));

	volume = calculateSESVolume(f, 1.5);
	TEST_REAL_EQUAL(volume, 7.16437)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
