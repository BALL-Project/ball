// $Id: AmberFF_bench.C,v 1.1 2001/04/10 16:16:55 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(AmberFF, "$Id: AmberFF_bench.C,v 1.1 2001/04/10 16:16:55 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PDBFile pdb("data/AmberFF_bench.pdb");
System S;
pdb >> S;

AmberFF amber;
START_SECTION(Setup)
	START_TIMER
		amber.setup(S);
	STOP_TIMER
END_SECTION

START_SECTION(Update w/o selection)
	START_TIMER
		amber.update();
	STOP_TIMER
END_SECTION

START_SECTION(Energy calculation w/o selection)
	START_TIMER
		amber.updateEnergy();
	STOP_TIMER
END_SECTION

START_SECTION(Second energy calculation w/o selection)
	START_TIMER
		amber.updateEnergy();
	STOP_TIMER
END_SECTION

S.beginResidue()->select();
START_SECTION(Update w/ selection)
	START_TIMER
		amber.update();
	STOP_TIMER
END_SECTION

START_SECTION(Energy calculation w/ selection)
	START_TIMER
		amber.updateEnergy();
	STOP_TIMER
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
