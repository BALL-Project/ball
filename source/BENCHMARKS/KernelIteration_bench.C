// $Id: KernelIteration_bench.C,v 1.1 2001/06/24 16:25:59 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(KernelIteration, 10.5, "$Id: KernelIteration_bench.C,v 1.1 2001/06/24 16:25:59 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PDBFile infile("data/AmberFF_bench.pdb");
System S;
infile >> S;
infile.close();

START_SECTION(ResidueIteration, 0.3)

	for (int count = 0; count < 10000; count++)
	{
		START_TIMER
			ResidueIterator res_it = S.beginResidue();
			for (; +res_it; ++res_it);
		STOP_TIMER
	}

END_SECTION

START_SECTION(AtomIteration, 0.7)

	for (int count = 0; count < 10000; count++)
	{
		START_TIMER
			AtomIterator res_it = S.beginAtom();
			for (; +res_it; ++res_it);
		STOP_TIMER
	}

END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
