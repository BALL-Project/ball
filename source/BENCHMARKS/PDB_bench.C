// $Id: PDB_bench.C,v 1.2.4.1 2002/02/27 00:32:50 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(PDBFile, 1.0, "$Id: PDB_bench.C,v 1.2.4.1 2002/02/27 00:32:50 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

START_SECTION(Reading, 1.0)
	std::ios_base::sync_with_stdio (false);


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
