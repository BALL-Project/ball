// $Id: PoissonBoltzmann_test.C,v 1.7 2000/05/23 12:24:26 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/SOLVATION/poissonBoltzmann.h>

START_TEST(FDPB, "$Id: PoissonBoltzmann_test.C,v 1.7 2000/05/23 12:24:26 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
			
using namespace BALL;

PRECISION(0.005)

System*		system = new System;
Atom*			atom = new Atom;
Molecule*	molecule = new Molecule;
molecule->insert(*atom);
system->insert(*molecule);
atom->setRadius( 2.0);
atom->setCharge(+1.0);

FDPB*		fdpb;

CHECK(default constructor)
	fdpb = new FDPB;
	TEST_NOT_EQUAL(fdpb, 0)
RESULT

CHECK(destructor)
	delete fdpb;
RESULT

CHECK(constructor/1)
	fdpb = new FDPB(*system);
	TEST_NOT_EQUAL(fdpb, 0)
RESULT

Options options;
CHECK(constructor/2)
	options = fdpb->options;
	delete fdpb;
	fdpb = new FDPB(options);
	TEST_NOT_EQUAL(fdpb, 0)
RESULT

CHECK(constructor/3)
	delete fdpb;
	options.destroy();
	options.setReal(FDPB::Option::SOLVENT_DC, 78.0);
	options.setReal(FDPB::Option::SOLUTE_DC, 1.0);
	options.setReal(FDPB::Option::SPACING, 0.4);
	options.setReal(FDPB::Option::BORDER, 10.0);
	options.set(FDPB::Option::CHARGE_DISTRIBUTION, FDPB::ChargeDistribution::UNIFORM);
	options.set(FDPB::Option::DIELECTRIC_SMOOTHING, FDPB::DielectricSmoothing::NONE);
	options[FDPB::Option::IONIC_STRENGTH] = 0.0;
	options[FDPB::Option::VERBOSITY] = 0;
	fdpb = new FDPB(*system, options);
	TEST_NOT_EQUAL(fdpb, 0)
RESULT

CHECK(solve)
	fdpb->solve();
	TEST_EQUAL(fdpb->results["converged"], "true")
RESULT

CHECK(energy - reaction field energy - and numerical accuracy)
	float E_water = fdpb->getEnergy();
	float E_RF_water = fdpb->getReactionFieldEnergy();
	
	fdpb->options.setReal(FDPB::Option::SOLVENT_DC, 1.0);
	fdpb->setup(*system);
	fdpb->solve();
	float E_vacuum = fdpb->getEnergy();
	float E_RF_vacuum = fdpb->getReactionFieldEnergy();

	PRECISION(1.0)
	TEST_REAL_EQUAL(E_water - E_vacuum, -357.68)

	PRECISION(0.005)
	TEST_REAL_EQUAL(E_RF_water, -342.897)
	TEST_REAL_EQUAL(E_RF_vacuum, 0.0)
	delete fdpb;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
