// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CharmmFF_bench.C,v 1.5 2002/12/22 13:19:19 oliver Exp $

#include <BALL/CONCEPT/benchmark.h>

#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(CharmmFF, 1.0, "$Id: CharmmFF_bench.C,v 1.5 2002/12/22 13:19:19 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PDBFile pdb("data/AmberFF_bench.pdb");
System S;
pdb >> S;

CharmmFF charmm;
START_SECTION(20x setup, 0.05)
	START_TIMER
		for (int i = 0; i < 20; i++)
		{
			charmm.setup(S);
		}
	STOP_TIMER
END_SECTION

START_SECTION(20x update w/o selection, 0.05)
	START_TIMER
		for (int i = 0; i < 20; i++)
		{
			charmm.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(50x energy calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			charmm.updateEnergy();	
		}
	STOP_TIMER
END_SECTION

START_SECTION(50x force calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			charmm.updateForces();
		}
	STOP_TIMER
END_SECTION

ForceFieldComponent* component;
START_SECTION(100x nonbonded energy calculation w/o selection, 0.1)
	component = charmm.getComponent("Charmm NonBonded");
	STATUS("Component pointer: " << (void*)component)
	if (component != 0)
	{
		START_TIMER
			for (Size i = 0; i < 100; i++)
			{
				component->updateEnergy();
			}
		STOP_TIMER
	}
END_SECTION

START_SECTION(5000x stretch energy calculation w/o selection, 0.1)
	component = charmm.getComponent("Charmm Stretch");
	START_TIMER
		for (Size i = 0; i < 5000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(5000x bend energy calculation w/o selection, 0.1)
	component = charmm.getComponent("Charmm Bend");
	START_TIMER
		for (Size i = 0; i < 5000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(5000x torsion energy calculation w/o selection, 0.1)
	component = charmm.getComponent("Charmm Torsion");
	START_TIMER
		for (Size i = 0; i < 5000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

S.beginResidue()->select();
START_SECTION(50x update w/ selection, 0.05)
	START_TIMER
		for (int i = 0; i < 50; i++)
		{
			charmm.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(200x energy calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 200; i++)
		{
			charmm.updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(200x force calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 200; i++)
		{
			charmm.updateForces();
		}
	STOP_TIMER
END_SECTION

START_SECTION(1000x nonbonded energy calculation w/ selection, 0.1)
	component = charmm.getComponent("Charmm NonBonded");
	START_TIMER
		for (Size i = 0; i < 1000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(10000x stretch energy calculation w/ selection, 0.1)
	component = charmm.getComponent("Charmm Stretch");
	START_TIMER
		for (Size i = 0; i < 10000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(10000x bend energy calculation w/ selection, 0.1)
	component = charmm.getComponent("Charmm Bend");
	START_TIMER
		for (Size i = 0; i < 10000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(10000x torsion energy calculation w/ selection, 0.1)
	component = charmm.getComponent("Charmm Torsion");
	START_TIMER
		for (Size i = 0; i < 10000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
