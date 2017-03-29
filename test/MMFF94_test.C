// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

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

float diff(double original, double our)
{
	double x = original - our;
	x = fabs(x);
	return x / fabs(original);
}

START_TEST(MMFF94)

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

mmff.options[MMFF94::Option::VDW_CUTOFF] = 99;
mmff.options[MMFF94::Option::VDW_CUTON] = 98;
mmff.options[MMFF94::Option::ELECTROSTATIC_CUTOFF] = 99;
mmff.options[MMFF94::Option::ELECTROSTATIC_CUTON] = 99;
mmff.options[MMFF94::Option::NONBONDED_CUTOFF] = 199;
mmff.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = false;

CHECK(forces and energies equal in two consecutive runs)
	MOL2File f(BALL_TEST_DATA_PATH(MMFF94_test2.mol2));
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
CHECK(test 1.1: Stretches)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94_test1.hin));	
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
CHECK(test 1.2: Stretches compared to CHARMM implementation)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-stretch.hin));
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 2)

	mmff.setup(s);
	mmff.updateEnergy();
	mmff.updateForces();
	AtomIterator it = s.beginAtom();

	float charmm_energy = 147.96645 * Constants::JOULE_PER_CAL;
	TEST_EQUAL(diff(charmm_energy, sb.getStretchEnergy()) < 0.00001, true)

	// value from CHARMM:
	Vector3 charmm(-879.369641, 0, 0);
	charmm *= CHARMM_FORCES_FACTOR;

	TEST_REAL_EQUAL(it->getForce().getDistance(charmm), 0)
	TEST_EQUAL(diff(charmm.x, it->getForce().x) < 0.00001, true)
	it++;
	TEST_REAL_EQUAL(it->getForce().getDistance(-charmm), 0)
	TEST_EQUAL(diff(-charmm.x, it->getForce().x) < 0.00001, true)
RESULT


CHECK(test 1.3: Stretches with finite difference)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94_test1.hin));	
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
	mmff.updateEnergy();
	mmff.updateForces();
	PRECISION(1e-11)
	TEST_REAL_EQUAL(sb.getStretchEnergy(), 0)

	// low precision here:
	PRECISION(1e-13)

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

CHECK(test 2.1: Bends)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-bend.hin));
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

	PRECISION(0.5)

	float charmm_energy = 3.09341 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(mmff.getEnergy(), charmm_energy)
	TEST_EQUAL(diff(charmm_energy, mmff.getEnergy() < 0.0001), true)

	// gradient value in CHARMM (kcal /mol A) !:
	Vector3 v1(0., 27.3344889, 0.);
	Vector3 v2(27.3344889,-27.3344889, 0.);
	Vector3 v3(-27.3344889, 0., 0.);
	v1 *= -CHARMM_FORCES_FACTOR;
	v2 *= -CHARMM_FORCES_FACTOR;
	v3 *= -CHARMM_FORCES_FACTOR;

	PRECISION(2e-12)
	TEST_REAL_EQUAL(a1.getForce().x, v1.x)
	TEST_REAL_EQUAL(a1.getForce().y, v1.y)
	TEST_REAL_EQUAL(a1.getForce().z, v1.z)

	TEST_REAL_EQUAL(a2.getForce().x, v2.x)
	TEST_REAL_EQUAL(a2.getForce().y, v2.y)
	TEST_REAL_EQUAL(a2.getForce().z, v2.z)

	TEST_REAL_EQUAL(a3.getForce().x, v3.x)
	TEST_REAL_EQUAL(a3.getForce().y, v3.y)
	TEST_REAL_EQUAL(a3.getForce().z, v3.z)

	TEST_EQUAL(diff(v2.y, a2.getForce().y) < 0.001, true)


	a3.setPosition(Vector3(0, 2.96900, 0));

	v1.set(0., 27.3344889, 0.);
	v2.set(8.92122591,-27.3344889, 0.);
	v3.set(-8.92122591, 0., 0.);
	v1 *= -CHARMM_FORCES_FACTOR;
	v2 *= -CHARMM_FORCES_FACTOR;
	v3 *= -CHARMM_FORCES_FACTOR;

	mmff.updateForces();

	TEST_REAL_EQUAL(a1.getForce().x, v1.x)
	TEST_REAL_EQUAL(a1.getForce().y, v1.y)
	TEST_REAL_EQUAL(a1.getForce().z, v1.z)

	TEST_REAL_EQUAL(a2.getForce().x, v2.x)
	TEST_REAL_EQUAL(a2.getForce().y, v2.y)
	TEST_REAL_EQUAL(a2.getForce().z, v2.z)

	TEST_REAL_EQUAL(a3.getForce().x, v3.x)
	TEST_REAL_EQUAL(a3.getForce().y, v3.y)
	TEST_REAL_EQUAL(a3.getForce().z, v3.z)
	TEST_EQUAL(diff(v2.y, a2.getForce().y) < 0.001, true)
RESULT

CHECK(test 3.1: linear Bends)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-bend-lin.hin));
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

	TEST_EQUAL(diff(v2.y, a2.getForce().y) < 0.0001, true)

	float charmm_energy = 100.00715 * JOULE_PER_CAL;
	PRECISION(0.01)
	TEST_REAL_EQUAL(mmff.getBendEnergy(), charmm_energy)
	TEST_EQUAL(diff(charmm_energy, mmff.getBendEnergy()) < 0.00001, true)
RESULT

CHECK(force test 4.1: StretchBends)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-bend.hin));
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
	float charmm1 = -7.37396 * CHARMM_FORCES_FACTOR;

	Vector3 v1(charmm1, 0, 0);
	Vector3 v2(-charmm1, charmm1, 0);
	Vector3 v3(0, -charmm1, 0);

	PRECISION(2e-10)
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_EQUAL(diff(v1.x, a1.getForce().x) < 0.0001, true)
RESULT

CHECK(force test 4.2: StretchBends)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-bend2.hin));
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

	TEST_EQUAL(a1.getType(), 11)
	TEST_EQUAL(a2.getType(), 6)
	TEST_EQUAL(a3.getType(), 21)

	PRECISION(2e-11)

	// force value in CHARMM (kcal /mol A) !:
	// 15.37403 -24.48579  0.00000
  // -64.97696 29.61047  0.00000
  // 49.60293  -5.12468  0.00000
	Vector3 v1(15.37403, -24.48579, 0.00000);
 	Vector3 v2(-64.97696,  29.61047, 0.00000);
	Vector3 v3(49.60293, -5.12468, 0.00000);

	v1 *= -CHARMM_FORCES_FACTOR;
	v2 *= -CHARMM_FORCES_FACTOR;
	v3 *= -CHARMM_FORCES_FACTOR;

	PRECISION(2e-10)
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_EQUAL(diff(v1.x, a1.getForce().x) < 0.0001, true)

	// value from CHARMM:
	PRECISION(0.01)
	float charmm_energy = -25.34351 * JOULE_PER_CAL;
	TEST_REAL_EQUAL(mmff.getEnergy(), charmm_energy)
	TEST_EQUAL(diff(charmm_energy, mmff.getEnergy()) < 0.00001, true)
RESULT


CHECK(force test 4.3: StretchBends)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-bend3.hin));
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
	Vector3 v1(-72.39536, 11.80861,-36.9695);
	Vector3 v2(99.26076,-27.76734, 15.95874);
	Vector3 v3(-26.86540, 15.95874, 21.0108);
	v1 *= -CHARMM_FORCES_FACTOR;
	v2 *= -CHARMM_FORCES_FACTOR;
	v3 *= -CHARMM_FORCES_FACTOR;

	PRECISION(2e-10)
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_EQUAL(diff(v1.x, a1.getForce().x) < 0.0001, true)
RESULT

CHECK(force test 5.1: Planes)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-plane.hin));
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

	v1 *= -CHARMM_FORCES_FACTOR;
	v2 *= -CHARMM_FORCES_FACTOR;
	v3 *= -CHARMM_FORCES_FACTOR;
	v4 *= -CHARMM_FORCES_FACTOR;

	PRECISION(2e-14)

	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_EQUAL(diff(a1.getForce().x, v1.x) < 0.00001, true)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_REAL_EQUAL(a4.getForce().getDistance(v4), 0)

	// value from CHARMM:
	PRECISION(0.01)
	float charmm_energy =38.44301 * JOULE_PER_CAL;
	TEST_REAL_EQUAL(mmff.getEnergy(), charmm_energy)
	TEST_EQUAL(diff(charmm_energy, mmff.getEnergy()) < 0.00001, true)
RESULT

CHECK(force test 5.2: Planes)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-plane2.hin));
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

	// force value in CHARMM (kcal /mol A) are biased
	/*
	Vector3 v1(-2.22167, 10.47312, 0.00000);
 	Vector3 v2(-5.49903, -4.27990, -4.65629);
	Vector3 v3(7.72070, -6.19322,  0.00000);
	Vector3 v4(0.00000,  0.00000,  4.65629);
	*/

	// BALL values
	Vector3 v1(-6.56536115141,13.1307223028,  0.0);
	Vector3 v2(-4.37690730111,-13.1307223028,  0.0);
	Vector3 v3(10.9422684525,0.0 , 0.0);
	Vector3 v4(0.0,  0.0,  0.0);

	v1 *= CHARMM_FORCES_FACTOR;
	v2 *= CHARMM_FORCES_FACTOR;
	v3 *= CHARMM_FORCES_FACTOR;
	v4 *= CHARMM_FORCES_FACTOR;

	PRECISION(2e-13)

//   Log.error() << std::endl<< "#~~#   2 "  << a1.getForce()           << " "  << __FILE__ << "  " << __LINE__<< std::endl;
//   Log.error() << "#~~#   3 "  << v1           << " "  << __FILE__ << "  " << __LINE__<< std::endl;
	TEST_REAL_EQUAL(a1.getForce().getDistance(v1), 0)
	TEST_REAL_EQUAL(a2.getForce().getDistance(v2), 0)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_REAL_EQUAL(a4.getForce().getDistance(v4), 0)

	// value from CHARMM:
	PRECISION(0.01)
	float charmm_energy = 36.46189 * JOULE_PER_CAL;
	TEST_REAL_EQUAL(mmff.getEnergy(), charmm_energy)
	TEST_EQUAL(diff(charmm_energy, mmff.getEnergy()) < 0.00001, true)
RESULT

CHECK(force test 6: Torsions)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-torsion.hin));
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
	TEST_EQUAL(diff(v2.z, a2.getForce().z) < 0.00001, true)
	TEST_REAL_EQUAL(a3.getForce().getDistance(v3), 0)
	TEST_REAL_EQUAL(a4.getForce().getDistance(v4), 0)

	// value from CHARMM:
	PRECISION(0.01)
	float charmm_energy = 6.0 * JOULE_PER_CAL;
	TEST_REAL_EQUAL(mmff.getEnergy(), charmm_energy)
	TEST_EQUAL(diff(charmm_energy, mmff.getEnergy()) < 0.00001, true)
RESULT



CHECK(force test 7: VDW)
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-vdw2.hin));	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
 	a1.setFormalCharge(-1);
	Atom& a2 = *it;
 	a2.setFormalCharge(-1);

	mmff.options[MMFF94_ES_ENABLED] = "false";
	mmff.setup(s);
	enableOneComponent("MMFF94 NonBonded", mmff);
	mmff.updateEnergy();
	mmff.updateForces();

	PRECISION(0.1)
	
	float vdw_charmm = 64.46085 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(mmff.getEnergy(), vdw_charmm)
	TEST_EQUAL(diff(vdw_charmm, mmff.getEnergy()) < 0.00001, true)

	PRECISION(1e-12)

	float charmm_force = 208.73727 * CHARMM_FORCES_FACTOR;
	
	TEST_REAL_EQUAL(a1.getForce().x, -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x,  charmm_force);
	TEST_EQUAL(diff(-charmm_force, a1.getForce().x) < 0.000001, true)
	TEST_REAL_EQUAL(a1.getForce().y, 0)
	TEST_REAL_EQUAL(a2.getForce().y, 0)
	TEST_REAL_EQUAL(a1.getForce().z, 0)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

	TEST_EQUAL(a1.getForce(), -a2.getForce())

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
		double e1 = mmff.updateEnergy();
		a2.getPosition() += Vector3(delta, 0.0, 0.0);
		double e2 = mmff.updateEnergy();

		double dE = -(e2 - e1) / delta;
		TEST_REAL_EQUAL(force, dE * FORCES_FACTOR)
	}	
RESULT

CHECK(force test 8: ES CDIE)
	MMFF94NonBonded& nb = *(MMFF94NonBonded*)enableOneComponent("MMFF94 NonBonded", mmff);
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-vdw.hin));	
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
	mmff.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = false;

	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	TEST_REAL_EQUAL(a1.getCharge(), -1)
	TEST_REAL_EQUAL(a2.getCharge(), 2)

	PRECISION(0.01)
	double es_charmm = -323.97229 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)
	TEST_EQUAL(diff(es_charmm, nb.getESEnergy()) < 0.00001, true)
	PRECISION(2e-14)

	float charmm_force = -158.03526 * CHARMM_FORCES_FACTOR;

	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	TEST_EQUAL(diff(a1.getForce().x, -charmm_force) < 0.00001, true)
	PRECISION(2e-20)
	TEST_REAL_EQUAL(a1.getForce().y, 0)
	TEST_REAL_EQUAL(a2.getForce().y, 0)
	TEST_REAL_EQUAL(a1.getForce().z, 0)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

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

CHECK(force test 8.2: ES RDIE)
	mmff.options[MMFF94_VDW_ENABLED] = "false";
	mmff.options[MMFF94_ES_ENABLED] = "true";
	mmff.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = true;

	MMFF94NonBonded& nb = *(MMFF94NonBonded*)enableOneComponent("MMFF94 NonBonded", mmff);
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-vdw.hin));	
	System s;
	f >> s;
	f.close();

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	a2.setPosition(Vector3(10,0,0));

	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();

	// reduced precision: needed to interpolate values from CHARMM,
	// since SWITCHING could not be disabled
	PRECISION(0.1)
	double es_charmm =  -6.56550 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)
	PRECISION(2e-14)

	float charmm_force = -1.30756 * CHARMM_FORCES_FACTOR;

	PRECISION(2e-13)
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	TEST_REAL_EQUAL(a1.getForce().y, 0)
	TEST_REAL_EQUAL(a2.getForce().y, 0)
	TEST_REAL_EQUAL(a1.getForce().z, 0)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

	PRECISION(2e-12)
	a2.setPosition(Vector3(6,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	charmm_force = -5.99552 * CHARMM_FORCES_FACTOR;
	es_charmm = -18.12812 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	PRECISION(0.1)
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)
RESULT


CHECK(force test 9: ES SWITCH)
	mmff.options[MMFF94::Option::ELECTROSTATIC_CUTOFF] = 12;
	mmff.options[MMFF94::Option::ELECTROSTATIC_CUTON] = 8;
	mmff.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = false;
	mmff.options[MMFF94_VDW_ENABLED] = "false";
	mmff.options[MMFF94_ES_ENABLED] = "true";

	MMFF94NonBonded& nb = *(MMFF94NonBonded*)enableOneComponent("MMFF94 NonBonded", mmff);
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-vdw.hin));	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	a2.setPosition(Vector3(10,0,0));

	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();

	PRECISION(0.0001)
	double es_charmm = -2.71125 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)
	PRECISION(2e-14)

	float charmm_force = -3.77685 * CHARMM_FORCES_FACTOR;

	PRECISION(2e-15)
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	TEST_EQUAL(diff(a1.getForce().x, -charmm_force) < 0.00001, true)
	TEST_REAL_EQUAL(a1.getForce().y, 0)
	TEST_REAL_EQUAL(a2.getForce().y, 0)
	TEST_REAL_EQUAL(a1.getForce().z, 0)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

	// cutoff distance reached
	a2.setPosition(Vector3(12,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	TEST_REAL_EQUAL(nb.getESEnergy(), 0)
	TEST_REAL_EQUAL(a1.getForce().getLength(), 0)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 0)

	PRECISION(2e-16)
	a2.setPosition(Vector3(9,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	charmm_force = -7.16454 * CHARMM_FORCES_FACTOR;
	es_charmm = -8.16304 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	PRECISION(0.0001)
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)

	PRECISION(2e-16)
	a2.setPosition(Vector3(8,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	charmm_force = -10.24873 * CHARMM_FORCES_FACTOR;
	es_charmm = -16.94178 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	PRECISION(0.0001)
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)

	PRECISION(2e-16)
	a2.setPosition(Vector3(6,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	charmm_force = -18.14475 * CHARMM_FORCES_FACTOR;
	es_charmm = -44.21526 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	PRECISION(0.0001)
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)
RESULT

CHECK(force test 10: ES SWITCH RDIE)
	mmff.options[MMFF94::Option::ELECTROSTATIC_CUTOFF] = 12;
	mmff.options[MMFF94::Option::ELECTROSTATIC_CUTON] = 8;
	mmff.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = "true";

	MMFF94NonBonded& nb = *(MMFF94NonBonded*)enableOneComponent("MMFF94 NonBonded", mmff);
	HINFile f(BALL_TEST_DATA_PATH(MMFF94-vdw.hin));	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	a2.setPosition(Vector3(10,0,0));

	mmff.options[MMFF94_VDW_ENABLED] = "false";
	mmff.options[MMFF94_ES_ENABLED] = "true";

	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();

	PRECISION(0.0001)
	double es_charmm = -0.51364 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)
	TEST_EQUAL(diff(es_charmm, nb.getESEnergy()) < 0.00001, true)
	PRECISION(2e-14)

	float charmm_force = -0.75161 * CHARMM_FORCES_FACTOR;

	PRECISION(2e-15)
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	TEST_REAL_EQUAL(a1.getForce().y, 0)
	TEST_REAL_EQUAL(a2.getForce().y, 0)
	TEST_REAL_EQUAL(a1.getForce().z, 0)
	TEST_REAL_EQUAL(a2.getForce().z, 0)

	// cutoff distance reached
	a2.setPosition(Vector3(12,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	TEST_REAL_EQUAL(nb.getESEnergy(), 0)
	TEST_REAL_EQUAL(a1.getForce().getLength(), 0)
	TEST_REAL_EQUAL(a2.getForce().getLength(), 0)

	PRECISION(2e-15)
	a2.setPosition(Vector3(9,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	charmm_force = -1.58332 * CHARMM_FORCES_FACTOR;
	es_charmm = -1.66266 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	PRECISION(0.0001)
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)

	PRECISION(2e-16)
	a2.setPosition(Vector3(8,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	charmm_force = -2.54627 * CHARMM_FORCES_FACTOR;
	es_charmm = -3.72560 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	PRECISION(0.0001)
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)

	PRECISION(2e-15)
	a2.setPosition(Vector3(6,0,0));
	mmff.setup(s);
	mmff.updateForces();
	mmff.updateEnergy();
	charmm_force = -5.99826 * CHARMM_FORCES_FACTOR;
	es_charmm = -11.62162 * Constants::JOULE_PER_CAL;
	TEST_REAL_EQUAL(a1.getForce().x , -charmm_force);
	TEST_REAL_EQUAL(a2.getForce().x , charmm_force);
	PRECISION(0.0001)
	TEST_REAL_EQUAL(nb.getESEnergy(), es_charmm)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

