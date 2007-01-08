// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MolmecSupport_bench.C,v 1.5 2002/12/21 16:46:18 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

using namespace BALL;

// args: benchmark name (without '"'), reference time in seconds, CVS ID string
START_BENCHMARK(MolmecSupport, 4.0, "$Id: MolmecSupport_bench.C,v 1.5 2002/12/21 16:46:18 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// Create a system filled with solvent (water) only
System S;
ForceField ff;
ff.options.setVector(PeriodicBoundary::Option::PERIODIC_BOX_LOWER, Vector3(-17.0));
ff.options.setVector(PeriodicBoundary::Option::PERIODIC_BOX_UPPER, Vector3(17.0));
ff.periodic_boundary.enable();
ff.setup(S);

ForceField::PairVector pair_vector;
pair_vector.reserve(2000000);
STATUS("number of atoms: " << S.countAtoms())
START_SECTION(calculateNonBondedAtomPairs(type = HASH_GRID, periodic_boundary = true), 0.25)
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 8.0, true, MolmecSupport::HASH_GRID);
	STOP_TIMER
END_SECTION
STATUS(pair_vector.size())

START_SECTION(calculateNonBondedAtomPairs(type = BRUTE_FORCE, periodic_boundary = true), 0.25)
	pair_vector.clear();
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 8.0, true, MolmecSupport::BRUTE_FORCE);
	STOP_TIMER
END_SECTION
STATUS(pair_vector.size())

START_SECTION(calculateNonBondedAtomPairs(type = HASH_GRID, periodic_boundary = false), 0.25)
	pair_vector.clear();
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 8.0, false, MolmecSupport::HASH_GRID);
	STOP_TIMER
END_SECTION
STATUS(pair_vector.size())

START_SECTION(calculateNonBondedAtomPairs(type = BRUTE_FORCE, periodic_boundary = false), 0.25)
	pair_vector.clear();
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 8.0, false, MolmecSupport::BRUTE_FORCE);
	STOP_TIMER
END_SECTION
STATUS(pair_vector.size())

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
