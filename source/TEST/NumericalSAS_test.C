// $Id: NumericalSAS_test.C,v 1.3 2000/06/01 10:35:19 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/MATHS/surface.h>
#include <BALL/DATATYPE/hashMap.h>
///////////////////////////

START_TEST(NumericalSAS, "$Id: NumericalSAS_test.C,v 1.3 2000/06/01 10:35:19 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(calculateNumericalSASArea(const	Composite&, float, int))
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	float area = calculateNumericalSASArea(f, 1.5, 624);

	PRECISION(0.001)
	TEST_REAL_EQUAL(area, 157.07963)
RESULT

CHECK(calculateNumericalSASAtomAreas())
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	HashMap<Atom*, float>	atom_map;

	float area = calculateNumericalSASAtomAreas(f, atom_map, 1.5, 624);

	PRECISION(0.001)
	TEST_REAL_EQUAL(area, 157.07963)
	TEST_REAL_EQUAL(atom_map[&a1], area / 2.0)
	TEST_REAL_EQUAL(atom_map[&a2], area / 2.0)
RESULT

CHECK(calculateNumericalSASPoints())
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	Surface surface;
	float area = calculateNumericalSASPoints(f, surface, 1.5, 624);

	PRECISION(0.001)
	TEST_REAL_EQUAL(area, 157.07963)
	TEST_EQUAL(surface.vertex.size(), 1284)
	TEST_EQUAL(surface.normal.size(), 1284)

	// sum up all normals to check for integrality of the 
	// surface elements
	float surface_elements = 0;
	for (Position i = 0; i < surface.normal.size(); i++)
	{
		surface_elements += surface.normal[i].getLength();
	}
	TEST_REAL_EQUAL(surface_elements, area)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
