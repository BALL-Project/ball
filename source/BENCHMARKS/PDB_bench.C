// $Id: PDB_bench.C,v 1.1 2001/06/24 16:25:59 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(PDBFile, 10.0, "$Id: PDB_bench.C,v 1.1 2001/06/24 16:25:59 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION(Reading, 1.0)

	for (int count = 0; count < 100; count++)
	{
		PDBFile infile("data/AmberFF_bench.pdb");
		System S;
		START_TIMER
		infile >> S;
		STOP_TIMER
		infile.close();
	}
	

END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
