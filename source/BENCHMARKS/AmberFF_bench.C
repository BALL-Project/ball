// $Id: AmberFF_bench.C,v 1.3.4.4 2002/03/01 02:55:35 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(AmberFF, 1.0, "$Id: AmberFF_bench.C,v 1.3.4.4 2002/03/01 02:55:35 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PDBFile pdb("data/AmberFF_bench.pdb");
System S;
pdb >> S;

AmberFF amber;
START_SECTION(Setup, 0.05)
	START_TIMER
		for (int i = 0; i < 20; i++)
		{
			amber.setup(S);
		}
	STOP_TIMER
END_SECTION

START_SECTION(Update w/o selection, 0.05)
	START_TIMER
		for (int i = 0; i < 20; i++)
		{
			amber.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(Energy calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			amber.updateEnergy();	
		}
	STOP_TIMER
END_SECTION

START_SECTION(Force calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			amber.updateForces();
		}
	STOP_TIMER
END_SECTION

S.beginResidue()->select();
START_SECTION(Update w/ selection, 0.05)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			amber.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(Energy calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 200; i++)
		{
			amber.updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(Force calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 200; i++)
		{
			amber.updateForces();
		}
	STOP_TIMER
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
