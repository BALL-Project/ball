// $Id: PoissonBoltzmann_bench.C,v 1.3 2001/05/07 14:24:19 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

#include <BALL/SOLVATION/poissonBoltzmann.h>

START_BENCHMARK(FDPB, 1.0, "$Id: PoissonBoltzmann_bench.C,v 1.3 2001/05/07 14:24:19 oliver Exp $")

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

Options options;
FDPB*		fdpb;

START_SECTION(setup, 0.5)
	options.setReal(FDPB::Option::SOLVENT_DC, 78.0);
	options.setReal(FDPB::Option::SOLUTE_DC, 1.0);
	options.setReal(FDPB::Option::SPACING, 0.20);
	options.setReal(FDPB::Option::BORDER, 10.001);
	options.set(FDPB::Option::CHARGE_DISTRIBUTION, FDPB::ChargeDistribution::UNIFORM);
	options.set(FDPB::Option::DIELECTRIC_SMOOTHING, FDPB::DielectricSmoothing::NONE);
	options[FDPB::Option::IONIC_STRENGTH] = 0.0;
	options[FDPB::Option::VERBOSITY] = 0;
	fdpb = new FDPB(*system, options);
	END_SECTION

START_SECTION(solve, 0.5)
	START_TIMER
	fdpb->solve();
	STOP_TIMER
END_SECTION

delete fdpb;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
