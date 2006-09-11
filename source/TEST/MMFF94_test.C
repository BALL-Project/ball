// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94_test.C,v 1.1.2.25 2006/09/11 21:27:32 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/PTE.h>

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
const double FORCES_FACTOR = 1000 * 1E10 / Constants::AVOGADRO;
// CHARMM forces to BALL forces
const double CHARMM_FORCES_FACTOR = Constants::JOULE_PER_CAL * FORCES_FACTOR;

START_TEST(MMFF94, "$Id: MMFF94_test.C,v 1.1.2.25 2006/09/11 21:27:32 amoll Exp $")

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
const float delta = 0.00001;

CHECK(forces and energies equal in two consecutive runs)
	MOL2File f("data/MMFF94_test2.mol2");
	System s;
	f >> s;
	f.close();

	TEST_EQUAL(s.countAtoms(), 10)
	mmff.setup(s);

	mmff.updateEnergy();
	mmff.updateForces();
	Atom& atom1 = *s.getAtom(0);
	Vector3 f1 = atom1.getForce();
	float energy = mmff.getEnergy();
	TEST_EQUAL(Maths::isZero(energy), false)
	TEST_EQUAL(f1.getLength() > 0., true)

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

// optimal values and old values comared:
CHECK(force test 0: Stretches)
	HINFile f("data/MMFF94_test1.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 2)

	mmff.setup(s);
	mmff.updateEnergy();
	sb.updateStretchForces();

	PRECISION(1e-11)
	// atoms in optimal distance -> forces should be (almost) zero
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
RESULT

// compare values to CHARMM:
CHECK(force test 1: Stretches)
	HINFile f("data/MMFF94-stretch.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 2)

	mmff.setup(s);
	mmff.updateEnergy();
	sb.updateStretchForces();

	AtomIterator it = s.beginAtom();
	// value from CHARMM:
	Vector3 charmm(0, 0, -40.2555071);
	charmm *= CHARMM_FORCES_FACTOR;
	TEST_REAL_EQUAL(it->getForce().getDistance(-charmm), 0)
	it++;
	TEST_REAL_EQUAL(it->getForce().getDistance(charmm), 0)
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

	sb.updateEnergy();
	TEST_REAL_EQUAL(sb.getStretchEnergy(), 0)
	sb.updateStretchForces();
	sb.updateStretchEnergy();

	// low precision here:
	PRECISION(1e-11)

	TEST_REAL_EQUAL(sb.getStretchEnergy(), 0)
	TEST_REAL_EQUAL(a1.getForce().getLength(), 0)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 0)

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

CHECK(force test 3.1: Bends)
	HINFile f("data/MMFF94-bend.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 3)
	
	// create references to the atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;

	mmff.options[MMFF94_STRETCHES_ENABLED] = "false";
	mmff.options[MMFF94_BENDS_ENABLED] = "true";
	mmff.options[MMFF94_STRETCHBENDS_ENABLED] = "false";
	mmff.setup(s);
	enableOneComponent("MMFF94 StretchBend", mmff);

	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-11)

	// value in CHARMM (kcal /mol A) !:
	// 0.  -27.3344889  0.
	// -27.3344889 27.3344889  0.
	// 27.3344889  0.  0.
	TEST_REAL_EQUAL(a1.getForce().x, 0)
	TEST_REAL_EQUAL(a1.getForce().y, -1.89975213338e-09)
	TEST_REAL_EQUAL(a1.getForce().z, 0)

	TEST_REAL_EQUAL(a2.getForce().x, -1.89975213338e-09)
	TEST_REAL_EQUAL(a2.getForce().y, 1.89975213338e-09)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

	TEST_REAL_EQUAL(a3.getForce().x, 1.89975213338e-09)
	TEST_REAL_EQUAL(a3.getForce().y, 0)
	TEST_REAL_EQUAL(a3.getForce().z, 0)

	a3.setPosition(Vector3(0, 2.96900, 0));
	mmff.updateForces();

	TEST_REAL_EQUAL(a1.getForce().x, 0)
	TEST_REAL_EQUAL(a1.getForce().y, -1.89975213338e-09)
	TEST_REAL_EQUAL(a1.getForce().z, 0)

	TEST_REAL_EQUAL(a2.getForce().x, -6.20026863185e-10)
	TEST_REAL_EQUAL(a2.getForce().y, 1.89975213338e-09)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

	TEST_REAL_EQUAL(a3.getForce().x, 6.20026863185e-10)
	TEST_REAL_EQUAL(a3.getForce().y, 0)
	TEST_REAL_EQUAL(a3.getForce().z, 0)
RESULT

// finite differences dont work for the bends:
/*
CHECK(force test 3.2: Bends)
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
//    		TEST_REAL_EQUAL(force, dE * FORCES_FACTOR)
	}	
RESULT
*/

CHECK(force test 3.3: linear Bends)
	HINFile f("data/MMFF94-bend-lin.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 3)
	
	// create references to the atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;

	mmff.options[MMFF94_STRETCHES_ENABLED] = "false";
	mmff.options[MMFF94_BENDS_ENABLED] = "true";
	mmff.options[MMFF94_STRETCHBENDS_ENABLED] = "false";
	mmff.setup(s);
	enableOneComponent("MMFF94 StretchBend", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-11)

	// gradient value in CHARMM (kcal /mol A) !:
	// LBEND  1  0.  85.0400921  0.
 	// LBEND  2  85.0400921 -85.0400921  0.
  // LBEND  3 -85.0400921  0.  0.
	float charmm = -85.0400921 * CHARMM_FORCES_FACTOR;
	Vector3 v1(0, charmm, 0);
	Vector3 v2(charmm, -charmm, 0);
	Vector3 v3(-charmm, 0, 0);

	PRECISION(2e-10)
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)

	PRECISION(10)
	TEST_REAL_EQUAL(mmff.getEnergy(), 100.00715 * JOULE_PER_CAL)
RESULT

CHECK(force test 4.1: StretchBends)
	HINFile f("data/MMFF94-bend.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 3)
	
	// create references to the atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;

	mmff.options[MMFF94_STRETCHES_ENABLED] = "false";
	mmff.options[MMFF94_BENDS_ENABLED] = "false";
	mmff.options[MMFF94_STRETCHBENDS_ENABLED] = "true";
	mmff.setup(s);
	enableOneComponent("MMFF94 StretchBend", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-11)

	// force value in CHARMM (kcal /mol A) !:
	//  7.37396 0       0
	// -7.37396 7.37396 0
	// 0       -7.37396 0
	float charmm1 = 7.37396 * CHARMM_FORCES_FACTOR;

	Vector3 v1(charmm1, 0, 0);
	Vector3 v2(-charmm1, charmm1, 0);
	Vector3 v3(0, -charmm1, 0);

	PRECISION(2e-10)
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
RESULT


CHECK(force test 4.2: StretchBends)
	HINFile f("data/MMFF94-bend2.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 3)
	
	// create references to the atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;

	mmff.options[MMFF94_STRETCHES_ENABLED] = "false";
	mmff.options[MMFF94_BENDS_ENABLED] = "false";
	mmff.options[MMFF94_STRETCHBENDS_ENABLED] = "true";
	mmff.setup(s);
	enableOneComponent("MMFF94 StretchBend", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-11)

	// force value in CHARMM (kcal /mol A) !:
	// 15.37403 -24.48579  0.00000
  // -64.97696 29.61047  0.00000
  // 49.60293  -5.12468  0.00000
	Vector3 v1(15.37403, -24.48579, 0.00000);
 	Vector3 v2(-64.97696,  29.61047, 0.00000);
	Vector3 v3(49.60293, -5.12468, 0.00000);

	v1 *= CHARMM_FORCES_FACTOR;
	v2 *= CHARMM_FORCES_FACTOR;
	v3 *= CHARMM_FORCES_FACTOR;

	PRECISION(2e-10)
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)

	// value from CHARMM:
	PRECISION(10)
	TEST_REAL_EQUAL(mmff.getEnergy(), -25.34351 * JOULE_PER_CAL)
RESULT

CHECK(force test 5.1: Planes)
	HINFile f("data/MMFF94-plane.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 4)
	
	// create references to the atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;
	Atom& a4 = *it++;

	mmff.setup(s);
	enableOneComponent("MMFF94 OutOfPlaneBend", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-11)

	// force value in CHARMM (kcal /mol A) !:
  Vector3 v1(13.75731,-11.19557,-11.33832);
	Vector3 v2(1.99175, 27.36249, -2.42734);
	Vector3 v3(-10.13007, 10.36824, 17.52184);
	Vector3 v4(-5.61899,-26.53516, -3.75618);

	v1 *= CHARMM_FORCES_FACTOR;
	v2 *= CHARMM_FORCES_FACTOR;
	v3 *= CHARMM_FORCES_FACTOR;
	v4 *= CHARMM_FORCES_FACTOR;

	PRECISION(2e-14)
/*
Log.error() << std::endl << "#~~#   1 "   << a1.getForce()        << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << "#~~#   1 " <<    v1       << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << std::endl << "#~~#   2 "   << a2.getForce()        << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << "#~~#   2 " <<    v2       << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << std::endl << "#~~#   3 "   << a3.getForce()        << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << "#~~#   3 " <<    v3       << " "  << __FILE__ << "  " << __LINE__<< std::endl;
*/
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_REAL_EQUAL(a4.getForce().getDistance(v4), 0)

	// value from CHARMM:
	PRECISION(1)
	TEST_REAL_EQUAL(mmff.getEnergy(), 38.44301 * JOULE_PER_CAL)
RESULT

CHECK(force test 5.2: Planes)
	HINFile f("data/MMFF94-plane2.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 4)
	
	// create references to the atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;
	Atom& a4 = *it++;

	mmff.setup(s);
	enableOneComponent("MMFF94 OutOfPlaneBend", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-11)

	// force value in CHARMM (kcal /mol A) !:
	Vector3 v1(-2.22167, 10.47312, 0.00000);
 	Vector3 v2(-5.49903, -4.27990, -4.65629);
	Vector3 v3(7.72070, -6.19322,  0.00000);
	Vector3 v4(0.00000,  0.00000,  4.65629);

	v1 *= CHARMM_FORCES_FACTOR;
	v2 *= CHARMM_FORCES_FACTOR;
	v3 *= CHARMM_FORCES_FACTOR;
	v4 *= CHARMM_FORCES_FACTOR;

	PRECISION(2e-10)
Log.error() << std::endl << "#~~#   1 "   << a1.getForce()        << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << "#~~#   1 " <<    v1       << " "  << __FILE__ << "  " << __LINE__<< std::endl;

Log.error() << std::endl << "#~~#   2 "   << a2.getForce()        << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << "#~~#   2 " <<    v2       << " "  << __FILE__ << "  " << __LINE__<< std::endl;

Log.error() << std::endl << "#~~#   3 "   << a3.getForce()        << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << "#~~#   3 " <<    v3       << " "  << __FILE__ << "  " << __LINE__<< std::endl;

Log.error() << std::endl << "#~~#   4 "   << a4.getForce()        << " "  << __FILE__ << "  " << __LINE__<< std::endl;
Log.error() << "#~~#   4 " <<    v4       << " "  << __FILE__ << "  " << __LINE__<< std::endl;
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_REAL_EQUAL(a4.getForce().getDistance(v4), 0)

	// value from CHARMM:
	PRECISION(1)
	TEST_REAL_EQUAL(mmff.getEnergy(), 36.46189 * JOULE_PER_CAL)
RESULT

CHECK(force test 6: Torsions)
	HINFile f("data/MMFF94-torsion.hin");
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 4)
	
	// create references to the atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it++;
	Atom& a3 = *it++;
	Atom& a4 = *it++;

	mmff.setup(s);
	enableOneComponent("MMFF94 Torsion", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-11)

	// gradient value in CHARMM (kcal /mol A) !:
	Vector3 v1(0.,  0.,  12.);
	Vector3 v2(-5.19556474E-16, 0.,-12.);
	Vector3 v3(-6., 0.,-6.);
	Vector3 v4(6., 0., 6.);

	v1 *= -CHARMM_FORCES_FACTOR;
	v2 *= -CHARMM_FORCES_FACTOR;
	v3 *= -CHARMM_FORCES_FACTOR;
	v4 *= -CHARMM_FORCES_FACTOR;

	PRECISION(1e-16)
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_REAL_EQUAL(a4.getForce().getDistance(v4), 0)

	// value from CHARMM:
	PRECISION(0.01)
	TEST_REAL_EQUAL(mmff.getEnergy(), 6.00000 * JOULE_PER_CAL)
RESULT




CHECK(force test 7: VDW)
	HINFile f("data/MMFF94-vdw.hin");	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	mmff.options[MMFF94_ES_ENABLED] = "false";
	mmff.setup(s);
	MMFF94NonBonded& nb = *(MMFF94NonBonded*)enableOneComponent("MMFF94 NonBonded", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(2e-12)

	/*
	TEST_REAL_EQUAL(a1.getForce().getLength(), 3.7209408887406425E-09)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 3.7209408887406425E-09)
	TEST_REAL_EQUAL(a2.getForce().y, 0)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

	TEST_EQUAL(a1.getForce(), -a2.getForce())

	// calculate the differential quotient of
	// the energy and compare it to the force
	PRECISION(2e-12)
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
		double e1 = mmff.updateEnergy();
		a2.getPosition() += Vector3(delta, 0.0, 0.0);
		double e2 = mmff.updateEnergy();

		double dE = -(e2 - e1) / delta;
		TEST_REAL_EQUAL(force, dE * FORCES_FACTOR)
	}	
*/

	MOL2File mol("data/MMFF94-vdw2.mol2");
	mol >> s;
	mol.close();
	mmff.setup(s);
	enableOneComponent("MMFF94 NonBonded", mmff);
	mmff.updateForces();
	PRECISION(2e-12)

	/** original values in CHARMM implmentation (kJ / mol A):
	    I   DX      DY        DZ
--------------------------------------------------------------------------------
   1    0.000    0.000    0.000
   2   -0.001   -0.011    0.027
   3    0.000    0.000    0.000
   4    0.00383389638   0.00304170822  -0.0145154378
	 5   -0.00262223589   0.00805800434  -0.0127604689
   5   -0.003    0.008   -0.013
	*/

	// values obtained in BALLView:
	Vector3 vs[5];
	vs[0] = Vector3(0.);
	vs[1] = Vector3(8.41826337371e-14,-7.71175224373e-13,1.89504878803e-12);
	vs[2] = Vector3(0.);
	vs[3] = Vector3(2.66367629993e-13,2.11328784333e-13,-1.00848908217e-12);
	vs[4] = Vector3(-1.82184996256e-13,5.5984644004e-13,-8.86559760067e-13);

	AtomIterator ait = s.beginAtom();
	for (Position p = 0; p < 5; p ++)
	{
		TEST_REAL_EQUAL(ait->getForce().x , vs[0].x)
		TEST_REAL_EQUAL(ait->getForce().y , vs[0].y)
		TEST_REAL_EQUAL(ait->getForce().z , vs[0].z)
		ait++;
	}
RESULT

CHECK(force test 8: ES)
	MMFF94NonBonded& nb = *(MMFF94NonBonded*)enableOneComponent("MMFF94 NonBonded", mmff);
	HINFile f("data/MMFF94-vdw.hin");	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	mmff.options[MMFF94_VDW_ENABLED] = "false";
	mmff.options[MMFF94_ES_ENABLED] = "true";
	mmff.setup(s);

	a1.setCharge(0);
	a2.setCharge(0);

	mmff.updateForces();
	mmff.updateEnergy();

	PRECISION(2e-12)

	TEST_REAL_EQUAL(mmff.getEnergy(), 0)
	TEST_REAL_EQUAL(a1.getForce().getLength(), 0)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 0)

	a1.setFormalCharge(-1);
	a2.setFormalCharge(2);
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	TEST_REAL_EQUAL(a1.getCharge(), -1)
	TEST_REAL_EQUAL(a2.getCharge(), 2)

	PRECISION(2e-4)
	TEST_REAL_EQUAL(nb.getESEnergy(), -1355.5)
	PRECISION(2e-10)

	TEST_REAL_EQUAL(a1.getForce().getLength(), 1.09798e-08)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 1.09798e-08)

//   	TEST_REAL_EQUAL(a1.getForce().getLength() * CHARMM_FORCES_FACTOR, 162.57780)
//   	TEST_REAL_EQUAL(a2.getForce().getLength() * CHARMM_FORCES_FACTOR, 162.57780)

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
		nb.updateEnergy();
		double dE = nb.getESEnergy();

		a2.getPosition() += Vector3(delta, 0.0, 0.0);
		mmff.updateEnergy();
		dE = -(nb.getESEnergy() - dE) / delta;
		TEST_REAL_EQUAL(force, dE * FORCES_FACTOR)
	}	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

