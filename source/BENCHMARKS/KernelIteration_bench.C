// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: KernelIteration_bench.C,v 1.3 2002/02/27 12:20:32 sturm Exp $
#include <BALLBenchmarkConfig.h>
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(KernelIteration, 1.0, "$Id: KernelIteration_bench.C,v 1.3 2002/02/27 12:20:32 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PDBFile infile(BALL_BENCHMARK_DATA_PATH(AmberFF_bench.pdb));
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
