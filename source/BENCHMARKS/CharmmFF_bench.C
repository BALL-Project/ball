// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CharmmFF_bench.C,v 1.7 2002/12/22 15:04:49 anker Exp $
#include <BALLBenchmarkConfig.h>
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(CharmmFF, 1.0, "$Id: CharmmFF_bench.C,v 1.7 2002/12/22 15:04:49 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

HINFile hin(BALL_BENCHMARK_DATA_PATH(CharmmFF_bench.hin));
System S;
hin >> S;

CharmmFF charmm;
charmm.options[CharmmFF::Option::ASSIGN_CHARGES] = "true";
charmm.options[CharmmFF::Option::ASSIGN_TYPENAMES] = "true";
charmm.options[CharmmFF::Option::ASSIGN_TYPES] = "true";
charmm.options[CharmmFF::Option::FILENAME] = "CHARMM/EEF1/param19_eef1.ini";

START_SECTION(20x setup, 0.05)
	START_TIMER
		for (int i = 0; i < 60; i++)
		{
			charmm.setup(S);
		}
	STOP_TIMER
END_SECTION

START_SECTION(20x update w/o selection, 0.05)
	START_TIMER
		for (int i = 0; i < 60; i++)
		{
			charmm.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(50x energy calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 150; i++)
		{
			charmm.updateEnergy();	
		}
	STOP_TIMER
END_SECTION

START_SECTION(50x force calculation w/o selection, 0.2)
	START_TIMER
		for (int i = 0; i < 150; i++)
		{
			charmm.updateForces();
		}
	STOP_TIMER
END_SECTION

ForceFieldComponent* component;
START_SECTION(100x nonbonded energy calculation w/o selection, 0.1)
	component = charmm.getComponent("CHARMM NonBonded");
	STATUS("Component pointer: " << (void*)component)
	if (component != 0)
	{
		START_TIMER
			for (Size i = 0; i < 300; i++)
			{
				component->updateEnergy();
			}
		STOP_TIMER
	}
END_SECTION

START_SECTION(5000x stretch energy calculation w/o selection, 0.1)
	component = charmm.getComponent("CHARMM Stretch");
	START_TIMER
		for (Size i = 0; i < 15000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(5000x bend energy calculation w/o selection, 0.1)
	component = charmm.getComponent("CHARMM Bend");
	START_TIMER
		for (Size i = 0; i < 15000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(5000x torsion energy calculation w/o selection, 0.1)
	component = charmm.getComponent("CHARMM Torsion");
	START_TIMER
		for (Size i = 0; i < 15000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

S.beginResidue()->select();
START_SECTION(50x update w/ selection, 0.05)
	START_TIMER
		for (int i = 0; i < 150; i++)
		{
			charmm.update();
		}
	STOP_TIMER
END_SECTION

START_SECTION(200x energy calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 600; i++)
		{
			charmm.updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(200x force calculation w/ selection, 0.2)
	START_TIMER
		for (int i = 0; i < 600; i++)
		{
			charmm.updateForces();
		}
	STOP_TIMER
END_SECTION

START_SECTION(1000x nonbonded energy calculation w/ selection, 0.1)
	component = charmm.getComponent("CHARMM NonBonded");
	START_TIMER
		for (Size i = 0; i < 3000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(10000x stretch energy calculation w/ selection, 0.1)
	component = charmm.getComponent("CHARMM Stretch");
	START_TIMER
		for (Size i = 0; i < 30000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(10000x bend energy calculation w/ selection, 0.1)
	component = charmm.getComponent("CHARMM Bend");
	START_TIMER
		for (Size i = 0; i < 30000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

START_SECTION(10000x torsion energy calculation w/ selection, 0.1)
	component = charmm.getComponent("CHARMM Torsion");
	START_TIMER
		for (Size i = 0; i < 30000; i++)
		{
			component->updateEnergy();
		}
	STOP_TIMER
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
