// $Id: PoissonBoltzmann_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/SOLVATION/poissonBoltzmann.h>

#undef PRECISION
#define PRECISION 0.005

START_TEST(FDPB, "$Id: PoissonBoltzmann_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
			
using namespace BALL;

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

Options	options;
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
options.setReal(FDPB::Option::SPACING, 0.3);
options.setReal(FDPB::Option::BORDER, 10.0);
fdpb = new FDPB(*system, options);
TEST_NOT_EQUAL(fdpb, 0)
RESULT

CHECK(solve)
fdpb->solve();
TEST_EQUAL(fdpb->results["converged"], "true")
RESULT

CHECK(getEnergy and numerical accuracy)
float E_water = fdpb->getEnergy();
fdpb->options.setReal(FDPB::Option::SOLVENT_DC, 1.0);
fdpb->setup(*system);
fdpb->solve();
float E_vacuum = fdpb->getEnergy();
delete fdpb;
TEST_REAL_EQUAL(E_water - E_vacuum, -342.031)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
