#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>

#include <BALL/CONCEPT/classTest.h>

START_TEST(GeometricProperties, "")

using namespace BALL;

CHECK(setTorsionAngle unsuccessful)
	Molecule m;
	Atom a1, a2, a3, a4;
	a1.setPosition(Vector3(0.0f,1.0f,0.0f));
	a2.setPosition(Vector3(0.0f,0.0f,0.0f));
	a3.setPosition(Vector3(1.0f,0.0f,0.0f));
	a4.setPosition(Vector3(1.0f,-1.0f,0.0f));

	m.insert(a1);
	m.insert(a2);
	m.insert(a3);
	m.insert(a4);

	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);
	a4.createBond(a1);

	float angle = 90.0f;
	TEST_EQUAL(setTorsionAngle(a1, a2, a3, a4, Angle(angle, false)), false);
RESULT

CHECK(setTorsionAngle successful)
	Molecule m;
	Atom a1, a2, a3, a4;
	a1.setPosition(Vector3(0.0f, 1.0f, 0.0f));
	a2.setPosition(Vector3(0.0f, 0.0f, 0.0f));
	a3.setPosition(Vector3(1.0f, 0.0f, 0.0f));
	a4.setPosition(Vector3(1.0f,-1.0f, 0.0f));

	m.insert(a1);
	m.insert(a2);
	m.insert(a3);
	m.insert(a4);

	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);

	PRECISION(1e-4)
	float angle = 90.0f;
	TEST_EQUAL(setTorsionAngle(a1, a2, a3, a4, Angle(angle, false)), true);
	Angle result = calculateTorsionAngle(a1, a2, a3, a4);
	TEST_REAL_EQUAL(result.toDegree(), angle);

	angle = 120.0f;
	TEST_EQUAL(setTorsionAngle(a1, a2, a3, a4, Angle(angle, false)), true);
	result = calculateTorsionAngle(a1, a2, a3, a4);
	TEST_REAL_EQUAL(result.toDegree(), angle);

	angle = 37.0f;
	TEST_EQUAL(setTorsionAngle(a1, a2, a3, a4, Angle(angle, false)), true);
	result = calculateTorsionAngle(a1, a2, a3, a4);
	TEST_REAL_EQUAL(result.toDegree(), angle);

	angle = 0.0f;
	TEST_EQUAL(setTorsionAngle(a1, a2, a3, a4, Angle(angle, false)), true);
	result = calculateTorsionAngle(a1, a2, a3, a4);
	TEST_REAL_EQUAL(result.toDegree(), angle);
RESULT

END_TEST
