// $Id: AnalyticalSES_test.C,v 1.8 2001/06/05 15:53:33 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/analyticalSES.h>
#include <BALL/KERNEL/fragment.h>
///////////////////////////

START_TEST(AnalyticalSES, "$Id: AnalyticalSES_test.C,v 1.8 2001/06/05 15:53:33 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(calculateSESArea(const AtomContainer& fragment, float probe_radius))
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

CHECK(calculateSESAtomAreas(const AtomContainer& fragment, HashMap<Atom*, float>& atom_areas, float probe_radius))
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	HashMap<const Atom*, float> atom_areas;
	float area = calculateSESAtomAreas(f, atom_areas, 1.5);
	
	PRECISION(0.001)
	TEST_REAL_EQUAL(area, 25.13274)

	// verify the contents of the hash map
	TEST_EQUAL(atom_areas.size(), 2)
	TEST_EQUAL(atom_areas.has(&a1), true)
	TEST_EQUAL(atom_areas.has(&a2), true)
	
	// verify the atom surface fractions
	TEST_REAL_EQUAL(atom_areas[&a1], 12.56637)
	TEST_REAL_EQUAL(atom_areas[&a2], 12.56637)
	

	// second case: overlapping atoms -> adds toroidal surface patches
	a2.setPosition(Vector3(1.0, 0.0, 0.0));

	area = calculateSESAtomAreas(f, atom_areas, 1.5);
	TEST_REAL_EQUAL(area, 18.722)

	// verify the contents of the hash map
	TEST_EQUAL(atom_areas.size(), 2)
	TEST_EQUAL(atom_areas.has(&a1), true)
	TEST_EQUAL(atom_areas.has(&a2), true)
	
	// verify the atom surface fractions
	TEST_REAL_EQUAL(atom_areas[&a1], 9.361)
	TEST_REAL_EQUAL(atom_areas[&a2], 9.361)

	// third case: three overlapping atoms -> adds concaver surface patches
	// the three atoms form a equilateral triangle
	float x = cos(Angle(30, false));
	float y = sin(Angle(30, false));
	a1.setPosition(Vector3(0.0, 1.0, 0.0));
	a2.setPosition(Vector3( x,  -y,  0.0));
	Atom a3;
	a3.setPosition(Vector3(-x,  -y,  0.0));
	a3.setRadius(1.0);
	f.insert(a3);

	area = calculateSESAtomAreas(f, atom_areas, 1.5);
	TEST_REAL_EQUAL(area, 30.5307)

	// verify the contents of the hash map
	TEST_EQUAL(atom_areas.size(), 3)
	TEST_EQUAL(atom_areas.has(&a1), true)
	TEST_EQUAL(atom_areas.has(&a2), true)
	TEST_EQUAL(atom_areas.has(&a3), true)
	
	// verify the atom surface fractions
	TEST_REAL_EQUAL(atom_areas[&a1], 10.1769)
	TEST_REAL_EQUAL(atom_areas[&a2], 10.1769)
	TEST_REAL_EQUAL(atom_areas[&a3], 10.1769)
RESULT


CHECK(calculateSESVolume(const AtomContainer& fragment, float probe_radius))
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
