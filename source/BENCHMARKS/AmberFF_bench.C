// $Id: AmberFF_bench.C,v 1.3.4.6 2002/06/06 21:56:04 oliver Exp $
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(AmberFF, 1.0, "$Id: AmberFF_bench.C,v 1.3.4.6 2002/06/06 21:56:04 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PDBFile pdb("data/AmberFF_bench.pdb");
System S;
pdb >> S;

AmberFF amber;
START_SECTION(20x setup, 0.05)
	START_TIMER
		for (int i = 0; i < 20; i++)
		{
			amber.setup(S);
		}
	STOP_TIMER
END_SECTION

START_SECTION(20x update w/o selection, 0.05)
	START_TIMER
		for (int i = 0; i < 20; i++)
		{
			amber.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(50x energy calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			amber.updateEnergy();	
		}
	STOP_TIMER
END_SECTION

START_SECTION(50x force calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			amber.updateForces();
		}
	STOP_TIMER
END_SECTION

S.beginResidue()->select();
START_SECTION(50x update w/ selection, 0.05)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			amber.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(200x energy calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 200; i++)
		{
			amber.updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(200x force calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 200; i++)
		{
			amber.updateForces();
		}
	STOP_TIMER
END_SECTION

ForceFieldComponent* component;
START_SECTION(200x nonbonded energy calculation w/ selection, 0.1)
	component = amber.getComponent("Amber NonBonded");
	START_TIMER
		for (Size i = 0; i < 400; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(1000x stretch energy calculation w/ selection, 0.1)
	component = amber.getComponent("Amber Stretch");
	START_TIMER
		for (Size i = 0; i < 1000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(1000x bend energy calculation w/ selection, 0.1)
	component = amber.getComponent("Amber Bend");
	START_TIMER
		for (Size i = 0; i < 1000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(1000x torsion energy calculation w/ selection, 0.1)
	component = amber.getComponent("Amber Torsion");
	START_TIMER
		for (Size i = 0; i < 1000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
