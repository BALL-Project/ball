// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94_test.C,v 1.1.2.1 2006/06/12 19:59:08 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/FORMAT/HINFile.h>

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


START_TEST(MMFF94, "$Id: MMFF94_test.C,v 1.1.2.1 2006/06/12 19:59:08 amoll Exp $")

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
ForceFieldComponent* ffc = enableOneComponent("MMFF94 StretchBend", mmff);
MMFF94StretchBend& sb = *((MMFF94StretchBend*) ffc);

CHECK(force test 1 Stretches)
	HINFile f("data/MMFF94_test1.hin");	
	System s;
	f >> s;
	f.close();
	TEST_EQUAL(s.countAtoms(), 2)

	mmff.setup(s);
	sb.updateStretchEnergy();
	sb.updateStretchForces();


	PRECISION(1e-12)
	AtomIterator it = s.beginAtom();
	for (; +it; ++it)
	{
		if (it->getName() == "C1")
		{
			TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(5.65872e-10,7.01203e-13,-3.2631e-10)), 0)
		}
		if (it->getName() == "C2")
		{
			TEST_REAL_EQUAL(it->getForce().getDistance(Vector3(-3.76732e-10,2.77556e-17,2.17502e-10)), 0)
		}
	}
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

	double force = a2.getForce().x * Constants::NA / 1e13;

	// calculate the differential quotient of
	// the energy and compare it to the force
	PRECISION(10)
	for (double d = .0; d <= 1.5; d += 0.01)
	{
		// move the atom to the new position
		a2.getPosition() += Vector3(d, 0.0, 0.0);

		// calculate the force
		sb.updateStretchEnergy();
		sb.updateStretchForces();
		double force = a2.getForce().x * Constants::NA / 1e13;

		// translate atom 2 by 0.0001 Angstrom to the left
		// and to the right to determine the differential quotient
		double dE = sb.getStretchEnergy();
		a2.setPosition(Vector3(d - 0.0001, 0.0, 0.0));
		dE = (sb.getStretchEnergy() - dE) / 0.0001;
		TEST_REAL_EQUAL(force, dE)
	}	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

