// $Id: PoissonBoltzmann_bench.C,v 1.1 2001/04/10 16:26:30 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/SOLVATION/poissonBoltzmann.h>

START_BENCHMARK(FDPB, "$Id: PoissonBoltzmann_bench.C,v 1.1 2001/04/10 16:26:30 oliver Exp $")

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

START_SECTION(setup)
	delete fdpb;
	options.destroy();
	options.setReal(FDPB::Option::SOLVENT_DC, 78.0);
	options.setReal(FDPB::Option::SOLUTE_DC, 1.0);
	options.setReal(FDPB::Option::SPACING, 0.3);
	options.setReal(FDPB::Option::BORDER, 10.001);
	options.set(FDPB::Option::CHARGE_DISTRIBUTION, FDPB::ChargeDistribution::UNIFORM);
	options.set(FDPB::Option::DIELECTRIC_SMOOTHING, FDPB::DielectricSmoothing::NONE);
	options[FDPB::Option::IONIC_STRENGTH] = 0.0;
	options[FDPB::Option::VERBOSITY] = 0;
	fdpb = new FDPB(*system, options);
	TEST_NOT_EQUAL(fdpb, 0)
END_SECTION

START_SECTION(solve)
	START_TIMER
	fdpb->solve();
	STOP_TIMER
END_SECTION

delete fdpb;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
