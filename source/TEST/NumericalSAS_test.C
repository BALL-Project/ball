// $Id: NumericalSAS_test.C,v 1.4 2000/06/06 13:19:04 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/MATHS/surface.h>
#include <BALL/DATATYPE/hashMap.h>
///////////////////////////

START_TEST(NumericalSAS, "$Id: NumericalSAS_test.C,v 1.4 2000/06/06 13:19:04 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(calculateSASArea(const	Composite&, float, int))
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	float area = calculateSASArea(f, 1.5, 624);

	PRECISION(0.001)
	TEST_REAL_EQUAL(area, 157.07963)
RESULT

CHECK(calculateSASAtomAreas())
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	HashMap<Atom*, float>	atom_map;

	float area = calculateSASAtomAreas(f, atom_map, 1.5, 624);

	PRECISION(0.001)
	TEST_REAL_EQUAL(area, 157.07963)
	TEST_REAL_EQUAL(atom_map[&a1], area / 2.0)
	TEST_REAL_EQUAL(atom_map[&a2], area / 2.0)
RESULT

CHECK(calculateSASPoints())
	Fragment	f;
	Atom a1, a2;
	a1.setRadius(1.0);
	a2.setRadius(1.0);
	a2.setPosition(Vector3(10.0, 0.0, 0.0));

	f.insert(a1);
	f.insert(a2);

	Surface surface;
	float area = calculateSASPoints(f, surface, 1.5, 624);

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
