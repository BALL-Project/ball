// $Id: MolmecSupport_bench.C,v 1.1.2.4 2002/06/07 10:36:52 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

using namespace BALL;

// args: benchmark name (without '"'), reference time in seconds, CVS ID string
START_BENCHMARK(MolmecSupport, 10.0, "$Id: MolmecSupport_bench.C,v 1.1.2.4 2002/06/07 10:36:52 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


System S;
ForceField ff;

ff.options.setVector(PeriodicBoundary::Option::PERIODIC_BOX_LOWER, Vector3(-20.0));
ff.options.setVector(PeriodicBoundary::Option::PERIODIC_BOX_UPPER, Vector3(20.0));
ff.periodic_boundary.enable();
ff.setup(S);

ForceField::PairVector pair_vector;
pair_vector.reserve(2000000);
START_SECTION(calculateNonBondedAtomPairs(type = HASH_GRID, periodic_boundary = true), 0.4)
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 10.0, true, MolmecSupport::HASH_GRID);
	STOP_TIMER
END_SECTION
std::cout << pair_vector.size() << std::endl;

pair_vector.clear();
START_SECTION(calculateNonBondedAtomPairs(type = BRUTE_FORCE, periodic_boundary = true), 0.4)
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 10.0, true, MolmecSupport::BRUTE_FORCE);
	STOP_TIMER
END_SECTION
std::cout << pair_vector.size() << std::endl;

pair_vector.clear();
START_SECTION(calculateNonBondedAtomPairs(type = HASH_GRID, periodic_boundary = false), 0.1)
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 10.0, false, MolmecSupport::HASH_GRID);
	STOP_TIMER
END_SECTION
std::cout << pair_vector.size() << std::endl;

pair_vector.clear();
START_SECTION(calculateNonBondedAtomPairs(type = BRUTE_FORCE, periodic_boundary = false), 0.1)
	START_TIMER
		MolmecSupport::calculateNonBondedAtomPairs
			(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
			 10.0, false, MolmecSupport::BRUTE_FORCE);
	STOP_TIMER
END_SECTION
std::cout << pair_vector.size() << std::endl;
std::cout << S.countAtoms() << std::endl;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
