// $Id: AmberFF_test.C,v 1.10 2000/05/23 10:23:44 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/FORMAT/HINFile.h>
///////////////////////////

START_TEST(AmberFF, "$Id: AmberFF_test.C,v 1.10 2000/05/23 10:23:44 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::Constants;

AmberFF* amber;
CHECK(AmberFF())
	amber = new AmberFF();
	TEST_NOT_EQUAL(amber, 0)
RESULT

CHECK(~AmberFF())
	delete amber;
RESULT

CHECK(AmberFF(const AmberFF& force_field, bool clone_deep = true))
	AmberFF a1;
	AmberFF a2(a1);
RESULT
	
CHECK(specificSetup())
	AmberFF a;
	a.specificSetup();
RESULT

AmberFF amber91;
amber91.options[AmberFF::Option::FILENAME] = "Amber/amber91.ini";
amber91.options[AmberFF::Option::ASSIGN_CHARGES] = "false";

CHECK(energy test 1 (Single Stretch) [AMBER91])
	// read a file containing a single stretch
	HINFile f("data/AmberFF_test_1.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 2)

	// setup the force field and
	// calculate forces and energy
	amber91.setup(s);
	amber91.updateEnergy();
	amber91.updateForces();

	PRECISION(5e-2)
	TEST_REAL_EQUAL(amber91.getEnergy(), 25.552734)
	TEST_REAL_EQUAL(amber91.getRMSGradient(), 228.7267908)
	TEST_REAL_EQUAL(amber91.getStretchEnergy(), 25.552734)
	TEST_REAL_EQUAL(amber91.getBendEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getVdWEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getESEnergy(), 0.0)
RESULT

CHECK(energy test 2 (Bend) [AMBER91])
	HINFile f("data/AmberFF_test_2.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 3)

	amber91.setup(s);
	amber91.updateEnergy();
	amber91.updateForces();

	PRECISION(5e-2)
	TEST_REAL_EQUAL(amber91.getEnergy(), 7.895902544)
	TEST_REAL_EQUAL(amber91.getRMSGradient(), 51.16447077)
	TEST_REAL_EQUAL(amber91.getStretchEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getBendEnergy(), 7.895902544)
	TEST_REAL_EQUAL(amber91.getTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getVdWEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getESEnergy(), 0.0)
RESULT

CHECK(energy test 3 (VdW) [AMBER91])
	HINFile f("data/AmberFF_test_3.hin");	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	amber91.setup(s);
	amber91.updateEnergy();
	amber91.updateForces();

	TEST_REAL_EQUAL(amber91.getEnergy(), 21.57722272)
	TEST_REAL_EQUAL(amber91.getRMSGradient(), 67.57709778)
	TEST_REAL_EQUAL(amber91.getStretchEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getBendEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getTorsionEnergy(), 0.0)
	TEST_REAL_EQUAL(amber91.getVdWEnergy(), 21.57722272)
	TEST_REAL_EQUAL(amber91.getESEnergy(), 0.0)
RESULT

CHECK(energy test 4 (Torsion) [AMBER91])
	HINFile f("data/AmberFF_test_4.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 4)

	amber91.setup(s);
	amber91.updateEnergy();
	amber91.updateForces();

	TEST_REAL_EQUAL(amber91.getEnergy(), 16.95386504)
	TEST_REAL_EQUAL(amber91.getRMSGradient(), 14.28469482)
	TEST_REAL_EQUAL(amber91.getStretchEnergy(), 2e-6)
	TEST_REAL_EQUAL(amber91.getBendEnergy(), 4e-6)
	TEST_REAL_EQUAL(amber91.getTorsionEnergy(), 17.04302192)
	TEST_REAL_EQUAL(amber91.getVdWEnergy(), -0.08817236)
	TEST_REAL_EQUAL(amber91.getESEnergy(), 0.0)
RESULT

CHECK(energy test 5 (AlaGlySer) [AMBER91])
	HINFile f("data/AlaGlySer.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 31)

	AmberFF amber91;
	amber91.options[AmberFF::Option::FILENAME] = "Amber/amber91.ini";
	amber91.options[AmberFF::Option::ASSIGN_CHARGES] = "false";

	amber91.setup(s);
	amber91.updateEnergy();
	amber91.updateForces();

	PRECISION(5e-2)
	TEST_REAL_EQUAL(amber91.getEnergy(), -314.12)
	TEST_REAL_EQUAL(amber91.getRMSGradient(), 35.0358)
	TEST_REAL_EQUAL(amber91.getStretchEnergy(), 3.00453)
	TEST_REAL_EQUAL(amber91.getBendEnergy(), 8.59268)
	TEST_REAL_EQUAL(amber91.getTorsionEnergy(), 0.0489528)
	TEST_REAL_EQUAL(amber91.getVdWEnergy(), 21.03)
	TEST_REAL_EQUAL(amber91.getESEnergy(), -346.797)
RESULT

AmberFF amber94;
amber94.options[AmberFF::Option::FILENAME] = "Amber/amber94.ini";
amber94.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
amber94.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
amber94.options[AmberFF::Option::ASSIGN_TYPES] = "true";
amber94.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
amber94.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";

CHECK(energy test 6 (AlaGlySer) [AMBER94])
	HINFile f("data/AlaGlySer2.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 31)

	amber94.setup(s);
	amber94.updateEnergy();
	amber94.updateForces();

	PRECISION(5e-2)
	TEST_REAL_EQUAL(amber94.getEnergy(), -91.2239)
	TEST_REAL_EQUAL(amber94.getRMSGradient(), 41.7585)
	TEST_REAL_EQUAL(amber94.getStretchEnergy(), 3.754722)
	TEST_REAL_EQUAL(amber94.getBendEnergy(), 9.1094048)
	TEST_REAL_EQUAL(amber94.getTorsionEnergy(), 14.3574)
	TEST_REAL_EQUAL(amber94.getVdWEnergy(), 45.4436)
	TEST_REAL_EQUAL(amber94.getESEnergy(), -163.8814224)
RESULT

CHECK(force test 1 (Torsion) [AMBER94])
	HINFile f("data/AMBER_test_1.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 4)

	amber94.setup(s);
	amber94.removeComponent("Amber NonBonded");
	amber94.removeComponent("Amber Bend");
	amber94.removeComponent("Amber Stretch");
	amber94.updateEnergy();
	amber94.updateForces();

	PRECISION(1e-12)
	AtomIterator it = s.beginAtom();
	for (; +it; ++it)
	{
		if (it->getName() == "C")
		{
			TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(5.65872e-10,7.01203e-13,-3.2631e-10)), 0)
		}
		if (it->getName() == "O")
		{
			TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(-3.76732e-10,2.77556e-17,2.17502e-10)), 0)
		}
		if (it->getName() == "N")
		{
			TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(9.69443e-11,2.64282e-10,9.07063e-11)), 0)
		}
		if (it->getName() == "H")
		{
			TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(-2.86084e-10,-2.64983e-10,1.81024e-11)), 0)
		}
	}
RESULT

CHECK(force test 2: ES switching function [AMBER91/CDIEL])
	HINFile f("data/AmberFF_test_3.hin");	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// set the cutoffs and cutons (VdW is switched off)
	amber91.options[AmberFF::Option::VDW_CUTON] = 0.05;
	amber91.options[AmberFF::Option::VDW_CUTOFF] = 0.1;
	amber91.options[AmberFF::Option::ELECTROSTATIC_CUTON] = 2.0;
	amber91.options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = 4.0;
	amber91.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = "false";	
	amber91.setup(s);

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	// move atom 1 into the origin and assign charges
	a1.setPosition(Vector3(0.0, 0.0, 0.0));
	a1.setCharge(1.35);
	a2.setCharge(-1.2);

	// calculate the differential quotient of
	// the energy and compare it to the force
	PRECISION(10)
	for (double d = 1.5; d <= 4.5; d += 0.01)
	{
		// move the atom to the new position
		a2.setPosition(Vector3(d, 0.0, 0.0));

		// calculate the force
		amber91.updateForces();
		double force = a2.getForce().x * Constants::NA / 1e13;

		// translate atom 2 by 0.0001 Angstrom to the left
		// and to the right to determine the differential quotient
		double dE = amber91.updateEnergy();
		a2.setPosition(Vector3(d - 0.0001, 0.0, 0.0));
		dE = (amber91.updateEnergy() - dE) / 0.0001;
		TEST_REAL_EQUAL(force, dE)
	}	
RESULT

CHECK(force test 3: ES switching function [AMBER91/RDIEL])
	HINFile f("data/AmberFF_test_3.hin");	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// set the cutoffs and cutons (VdW is switched off)
	amber91.options[AmberFF::Option::VDW_CUTON] = 0.05;
	amber91.options[AmberFF::Option::VDW_CUTOFF] = 0.1;
	amber91.options[AmberFF::Option::ELECTROSTATIC_CUTON] = 2.0;
	amber91.options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = 4.0;
	amber91.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = "true";	
	amber91.setup(s);

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	// move atom 1 into the origin and assign charges
	a1.setPosition(Vector3(0.0, 0.0, 0.0));
	a1.setCharge(1.35);
	a2.setCharge(-1.2);

	// calculate the differential quotient of
	// the energy and compare it to the force
	PRECISION(3)
	for (double d = 1.5; d <= 4.5; d += 0.01)
	{
		// move the atom to the new position
		a2.setPosition(Vector3(d, 0.0, 0.0));

		// calculate the force
		amber91.updateForces();
		double force = a2.getForce().x * Constants::NA / 1e13;

		// translate atom 2 by 0.0001 Angstrom to the left
		// and to the right to determine the differential quotient
		double dE = amber91.updateEnergy();
		a2.setPosition(Vector3(d - 0.0001, 0.0, 0.0));
		dE = (amber91.updateEnergy() - dE) / 0.0001;
		TEST_REAL_EQUAL(force, dE)
	}	
RESULT

CHECK(force test 4: VdW switching function [AMBER91])
	HINFile f("data/AmberFF_test_3.hin");	
	System s;
	f >> s;
	f.close();
	s.deselect();
	TEST_EQUAL(s.countAtoms(), 2)

	// set the cutoffs and cutons (VdW is switched off)
	amber91.options[AmberFF::Option::VDW_CUTON] = 4.0;
	amber91.options[AmberFF::Option::VDW_CUTOFF] = 6.0;
	amber91.options[AmberFF::Option::ELECTROSTATIC_CUTON] = 0.1;
	amber91.options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = 0.2;
	amber91.setup(s);

	// create references to the two atoms
	AtomIterator it = s.beginAtom();
	Atom& a1 = *it++;
	Atom& a2 = *it;

	// move atom 1 into the origin and assign charges
	a1.setPosition(Vector3(0.0, 0.0, 0.0));
	a1.setCharge(1.35);
	a2.setCharge(-1.2);

	// calculate the differential quotient of
	// the energy and compare it to the force
	PRECISION(0.01)
	for (double d = 3.5; d <= 6.5; d += 0.01)
	{
		// move the atom to the new position
		a2.setPosition(Vector3(d, 0.0, 0.0));

		// calculate the force
		amber91.updateForces();
		double force = a2.getForce().x * Constants::NA / 1e13;

		// translate atom 2 by 0.0001 Angstrom to the left
		// and to the right to determine the differential quotient
		double dE = amber91.updateEnergy();
		a2.setPosition(Vector3(d - 0.0001, 0.0, 0.0));
		dE = (amber91.updateEnergy() - dE) / 0.0001;
		TEST_REAL_EQUAL(force, dE)
	}	
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

