#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>

#include <BALL/CONCEPT/classTest.h>

START_TEST(GeometricProperties)

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

CHECK(BoundingBoxProcessor::start())
	BoundingBoxProcessor proc;

	TEST_EQUAL(proc.start(), true);
	TEST_EQUAL(proc.getUpper(), Vector3(-std::numeric_limits<float>::max()));
	TEST_EQUAL(proc.getLower(), Vector3(std::numeric_limits<float>::max()));
RESULT

CHECK(BoundingBoxProcessor::start())
	BoundingBoxProcessor proc;

	TEST_EQUAL(proc.start(), true);
	TEST_EQUAL(proc.getUpper(), Vector3(-std::numeric_limits<float>::max()));
	TEST_EQUAL(proc.getLower(), Vector3(std::numeric_limits<float>::max()));
RESULT

CHECK(BoundingBoxProcessor::operator()(Atom&))
	BoundingBoxProcessor proc;

	Atom a;
	a.setPosition(Vector3(-1,2,3));

	proc(a);

	TEST_EQUAL(proc.getLower(), Vector3(-1,0,0));
	TEST_EQUAL(proc.getUpper(), Vector3(0,2,3));

	Atom b;
	b.setPosition(Vector3(-2,4,3));

	proc(b);

	TEST_EQUAL(proc.getLower(), Vector3(-2,0,0));
	TEST_EQUAL(proc.getUpper(), Vector3(0,4,3));
RESULT

CHECK(BoundingBoxProcessor::operator()(Vector3&))
	BoundingBoxProcessor proc;

	Vector3 a(-1,2,3);

	proc(a);

	TEST_EQUAL(proc.getLower(), Vector3(-1,0,0));
	TEST_EQUAL(proc.getUpper(), Vector3(0,2,3));

	Vector3 b(-2,4,3);

	proc(b);

	TEST_EQUAL(proc.getLower(), Vector3(-2,0,0));
	TEST_EQUAL(proc.getUpper(), Vector3(0,4,3));
RESULT

CHECK(Composite::apply(BoundingBoxProcessor3&))
	BoundingBoxProcessor proc;

	System s;
	Molecule m1;
  Molecule m2;
	Atom a;
	a.setPosition(Vector3(1,3,5));
	Atom b;
	b.setPosition(Vector3(-2,-4,-6));
	Atom c;
	c.setPosition(Vector3(10,0,-20));
	m1.insert(a);
	m1.insert(b);
	m2.insert(c);
	s.insert(m1);
	s.insert(m2);

	s.apply(proc);
	const SimpleBox3 &box = proc.getBox();
	TEST_EQUAL(box.a, Vector3(-2,-4,-20));
	TEST_EQUAL(box.b, Vector3(10,3,5));
RESULT
END_TEST
