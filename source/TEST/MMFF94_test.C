// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94_test.C,v 1.1.2.7 2006/06/24 19:20:19 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOL2File.h>

///////////////////////////
using namespace BALL;
using namespace BALL::Constants;


ForceFieldComponent* enableOneComponent(const String& comp, MMFF94& mmff)
{
	ForceFieldComponent* ffc = 0;
	for (Position p = 0; p < mmff.countComponents(); p++)
	{
		ForceFieldComponent& cp = *mmff.getComponent(p);
		cp.setEnabled(cp.getName() == comp);
		if (cp.getName() == comp) ffc = &cp;
	}
	return ffc;
}


// Conversion from kJ / (mol A) into Newton
double FORCES_FACTOR = 1000 * 10E10 / Constants::AVOGADRO;


START_TEST(MMFF94, "$Id: MMFF94_test.C,v 1.1.2.7 2006/06/24 19:20:19 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

MMFF94* mmffp;
CHECK(MMFF94())
	mmffp = new MMFF94();
	TEST_NOT_EQUAL(mmffp, 0)
RESULT

CHECK(~MMFF94())
	delete mmffp;
RESULT

CHECK(MMFF94(const MMFF94& force_field, bool clone_deep = true))
	MMFF94 a1;
	MMFF94 a2(a1);
RESULT
	
CHECK(specificSetup())
	MMFF94 a;
	a.specificSetup();
RESULT


MMFF94 mmff;

CHECK(forces and energies equal in two consecutive runs)
	HINFile f("data/MMFF94_test1.hin");	
	System s;
	f >> s;
	f.close();

	TEST_EQUAL(s.countAtoms(), 2)
	mmff.setup(s);
	Atom& atom1 = *s.getAtom(0);

	mmff.updateEnergy();
	mmff.updateForces();
	Vector3 f1 = atom1.getForce();
	float energy = mmff.getEnergy();
	TEST_EQUAL(Maths::isZero(energy), false)
	TEST_EQUAL(Maths::isZero(f1.getSquareLength()), false)

	atom1.setForce(Vector3(99.));
	mmff.updateEnergy();
	mmff.updateForces();
	Vector3 f2 = atom1.getForce();
	float energy2 = mmff.getEnergy();
	TEST_EQUAL(f1, f2)
	TEST_REAL_EQUAL(energy, energy2)
RESULT

ForceFieldComponent* ffc = enableOneComponent("MMFF94 StretchBend", mmff);
MMFF94StretchBend& sb = *((MMFF94StretchBend*) ffc);

CHECK(force test 1: Stretches)
	HINFile f("data/MMFF94_test1.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 2)

	mmff.setup(s);
	mmff.updateEnergy();
	sb.updateStretchForces();

	PRECISION(3e-5)
	// atoms in optimal distance
	AtomIterator it = s.beginAtom();
	TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(0.)), 0)
	it->setForce(Vector3(0.));
	it++;
	TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(0.)), 0)
	it->setForce(Vector3(0.));

	// move atom by 0.5 to far away
	it->setPosition(Vector3(2.646,0,0));
	mmff.updateEnergy();
	sb.updateStretchForces();

	it = s.beginAtom();
	TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(406.1635825 * FORCES_FACTOR, 0, 0)), 0)
	it++;
	TEST_REAL_EQUAL(it->getForce().getDistance(-Vector3(406.1635825 * FORCES_FACTOR, 0, 0)), 0)

	it->setPosition(Vector3(2.146,0,0));
RESULT

CHECK(force test 2: Stretches)
	HINFile f("data/MMFF94_test1.hin");	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	mmff.setup(s);

	sb.updateStretchForces();
	sb.updateStretchEnergy();

	PRECISION(2e-12)

	TEST_REAL_EQUAL(sb.getStretchEnergy(), 0)
	TEST_REAL_EQUAL(a1.getForce().getLength(), 0)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 0)

	float delta = sqrt(numeric_limits<float>::epsilon());

	// calculate the differential quotient of
	// the energy and compare it to the force
	PRECISION(2e-10)
	Vector3 pos = a2.getPosition();
	for (double d = .0; d <= 0.5; d += 0.01)
	{
		// move the atom to the new position
		a2.getPosition() = pos + Vector3(d, 0.0, 0.0);

		// calculate the force
		mmff.updateForces();
		double force = a2.getForce().x;

		// translate atom 2 by 0.0001 Angstrom to the left
		// and to the right to determine the differential quotient
		sb.updateEnergy();
		double dE = sb.getStretchEnergy();

		a2.getPosition() += Vector3(delta, 0.0, 0.0);
		mmff.updateEnergy();
		dE = -(sb.getStretchEnergy() - dE) / delta;
		TEST_REAL_EQUAL(force, dE * FORCES_FACTOR)
	}	
RESULT

CHECK(force test 3: Bends)
	HINFile f("data/MMFF94_test3.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 3)
	
	// Atoms are at (0,0,0), (0,-1,0), (1, 0, 0)

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;

	Vector3 v1 = a1.getPosition() - a2.getPosition();
	Vector3 v2 = a3.getPosition() - a2.getPosition();
	Vector3 axis = v1 % v2;

	Matrix4x4 m;
	Angle angle_0(104.893, false);
	// k = 1.413
	m.setRotation(angle_0, axis);

	Vector3 v3 = m * v1;
	v3 += a2.getPosition();
	a3.setPosition(v3);

	mmff.setup(s);

	sb.updateForces();
	a1.setForce(Vector3());
	a2.setForce(Vector3());
	a3.setForce(Vector3());

	sb.updateBendForces();
	sb.updateBendEnergy();

	PRECISION(2e-12)

	TEST_REAL_EQUAL(sb.getBendEnergy(), 0)
	TEST_REAL_EQUAL(a1.getForce().getLength(), 0)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 0)
	TEST_REAL_EQUAL(a3.getForce().getLength(), 0)

	float delta = 0.0000001;

	// calculate the differential quotient of
	// the energy and compare it to the force
	Vector3 pos = a3.getPosition();
	for (double d = .0; d <= 0.1; d += 0.01)
	{
		// move the atom to the new position
		a3.getPosition() = pos + Vector3(0., -d, 0.0);
		Angle angle = v1.getAngle(a3.getPosition() - a2.getPosition());

		// calculate the force
		sb.updateForces();
		a1.setForce(Vector3(0.));
		a2.setForce(Vector3(0.));
		a3.setForce(Vector3(0.));
		sb.updateBendForces();
		double force = a1.getForce().x;
		Vector3 fv = a1.getForce();
		TEST_REAL_EQUAL(fv.y, 0.)
		TEST_REAL_EQUAL(fv.z, 0.)
		TEST_REAL_EQUAL(a1.getForce().getSquareLength(), a3.getForce().getSquareLength())
		sb.updateEnergy();
		double dE = sb.getBendEnergy();

		// translate atom 3 by delta Angstrom in y-axis direction
		// to determine the differential quotient
		a3.getPosition() += Vector3(0.0, delta, 0.0);
		double angle2 = Angle(v1.getAngle(a3.getPosition() - a2.getPosition()) - angle).toDegree();
		mmff.updateEnergy();
		dE = (sb.getBendEnergy() - dE) / angle2;
		TEST_REAL_EQUAL(force, dE * FORCES_FACTOR)
	}	
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

