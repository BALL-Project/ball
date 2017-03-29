// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/shiftedLVMM.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
///////////////////////////

START_TEST(ShiftedLVMMMinimizer)

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

System S;
AmberFF FF(S);

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_1 GEOMETRIC_MEAN)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ethan.hin));
	f >> S;
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-4)
	TEST_REAL_EQUAL(FF.getEnergy(), 18.5605)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(5);
	slvmm.setMaxGradient(0.418);
	slvmm.setEnergyDifferenceBound(0.00000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::GEOMETRIC_MEAN);

	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-2)
	TEST_REAL_EQUAL(energy, 5.906)
			
	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}	

	PRECISION(1E-1)
	Angle torsion;
	torsion = getTorsionAngle(pos[2].x, pos[2].y, pos[2].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[6].x, pos[6].y, pos[6].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[3].x, pos[3].y, pos[3].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[5].x, pos[5].y, pos[5].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[4].x, pos[4].y, pos[4].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[7].x, pos[7].y, pos[7].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	
	PRECISION(1E-2)
	Angle tet;
	tet = (pos[2] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[2] - pos[0]).getAngle(pos[4] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[4] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[7] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[7] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_1 RATIO_OF_SHIFT_PARAMS)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ethan.hin));
	f >> S;
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-4)
	TEST_REAL_EQUAL(FF.getEnergy(), 18.5605)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(5);
	slvmm.setMaxGradient(0.418);
	slvmm.setEnergyDifferenceBound(0.00000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::RATIO_OF_SHIFT_PARAMS);

	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-2)
	TEST_REAL_EQUAL(energy, 5.906)
			
	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}	

	PRECISION(1E-1)
	Angle torsion;
	torsion = getTorsionAngle(pos[2].x, pos[2].y, pos[2].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[6].x, pos[6].y, pos[6].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[3].x, pos[3].y, pos[3].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[5].x, pos[5].y, pos[5].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[4].x, pos[4].y, pos[4].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[7].x, pos[7].y, pos[7].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	
	PRECISION(1E-2)
	Angle tet;
	tet = (pos[2] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[2] - pos[0]).getAngle(pos[4] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[4] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[7] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[7] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_2 UNIT_VALUE)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ethan.hin));
	f >> S;
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-4)
	TEST_REAL_EQUAL(FF.getEnergy(), 18.5605)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(5);
	slvmm.setMaxGradient(0.418);
	slvmm.setEnergyDifferenceBound(0.00000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::UNIT_VALUE);

	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-2)
	TEST_REAL_EQUAL(energy, 5.906)
			
	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}	

	PRECISION(1E-1)
	Angle torsion;
	torsion = getTorsionAngle(pos[2].x, pos[2].y, pos[2].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[6].x, pos[6].y, pos[6].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[3].x, pos[3].y, pos[3].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[5].x, pos[5].y, pos[5].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[4].x, pos[4].y, pos[4].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[7].x, pos[7].y, pos[7].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	
	PRECISION(1E-2)
	Angle tet;
	tet = (pos[2] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[2] - pos[0]).getAngle(pos[4] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[4] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[7] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[7] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_2 BALANCING_VALUE)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ethan.hin));
	f >> S;
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-4)
	TEST_REAL_EQUAL(FF.getEnergy(), 18.5605)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(5);
	slvmm.setMaxGradient(0.418);
	slvmm.setEnergyDifferenceBound(0.00000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::BALANCING_VALUE);

	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-2)
	TEST_REAL_EQUAL(energy, 5.906)
			
	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}	

	PRECISION(1E-1)
	Angle torsion;
	torsion = getTorsionAngle(pos[2].x, pos[2].y, pos[2].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[6].x, pos[6].y, pos[6].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[3].x, pos[3].y, pos[3].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[5].x, pos[5].y, pos[5].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[4].x, pos[4].y, pos[4].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[7].x, pos[7].y, pos[7].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	
	PRECISION(1E-2)
	Angle tet;
	tet = (pos[2] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[2] - pos[0]).getAngle(pos[4] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[4] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[7] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[7] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_2 SQUARE_ROOT)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ethan.hin));
	f >> S;
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-4)
	TEST_REAL_EQUAL(FF.getEnergy(), 18.5605)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(5);
	slvmm.setMaxGradient(0.418);
	slvmm.setEnergyDifferenceBound(0.00000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::SQUARE_ROOT);

	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-2)
	TEST_REAL_EQUAL(energy, 5.906)
			
	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}	

	PRECISION(1E-1)
	Angle torsion;
	torsion = getTorsionAngle(pos[2].x, pos[2].y, pos[2].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[6].x, pos[6].y, pos[6].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[3].x, pos[3].y, pos[3].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[5].x, pos[5].y, pos[5].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[4].x, pos[4].y, pos[4].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[7].x, pos[7].y, pos[7].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	
	PRECISION(1E-2)
	Angle tet;
	tet = (pos[2] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[2] - pos[0]).getAngle(pos[4] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[4] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[7] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[7] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_2 GEOMETRIC_MEAN)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ethan.hin));
	f >> S;
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-4)
	TEST_REAL_EQUAL(FF.getEnergy(), 18.5605)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(5);
	slvmm.setMaxGradient(0.418);
	slvmm.setEnergyDifferenceBound(0.00000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::GEOMETRIC_MEAN);

	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-2)
	TEST_REAL_EQUAL(energy, 5.906)
			
	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}	

	PRECISION(1E-1)
	Angle torsion;
	torsion = getTorsionAngle(pos[2].x, pos[2].y, pos[2].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[6].x, pos[6].y, pos[6].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[3].x, pos[3].y, pos[3].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[5].x, pos[5].y, pos[5].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[4].x, pos[4].y, pos[4].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[7].x, pos[7].y, pos[7].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	
	PRECISION(1E-2)
	Angle tet;
	tet = (pos[2] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[2] - pos[0]).getAngle(pos[4] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[4] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[7] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[7] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_2 RATIO_OF_SHIFT_PARAMS)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ethan.hin));
	f >> S;
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-4)
	TEST_REAL_EQUAL(FF.getEnergy(), 18.5605)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(5);
	slvmm.setMaxGradient(0.418);
	slvmm.setEnergyDifferenceBound(0.00000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::RATIO_OF_SHIFT_PARAMS);

	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-2)
	TEST_REAL_EQUAL(energy, 5.906)
			
	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}	

	PRECISION(1E-1)
	Angle torsion;
	torsion = getTorsionAngle(pos[2].x, pos[2].y, pos[2].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[6].x, pos[6].y, pos[6].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[3].x, pos[3].y, pos[3].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[5].x, pos[5].y, pos[5].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	torsion = getTorsionAngle(pos[4].x, pos[4].y, pos[4].z, pos[0].x, pos[0].y, pos[0].z, pos[1].x, pos[1].y, pos[1].z, pos[7].x, pos[7].y, pos[7].z);
	TEST_REAL_EQUAL(torsion.toRadian(), 1.047)
	
	PRECISION(1E-2)
	Angle tet;
	tet = (pos[2] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[2] - pos[0]).getAngle(pos[4] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[4] - pos[0]).getAngle(pos[3] - pos[0]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[5] - pos[1]).getAngle(pos[7] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
	tet = (pos[7] - pos[1]).getAngle(pos[6] - pos[1]);
	TEST_REAL_EQUAL(tet.toRadian(), 1.902)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
