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
	

ShiftedLVMMMinimizer*	em;
CHECK(ShiftedLVMMMinimizer::ShiftedLVMMMinimizer())
	em = new ShiftedLVMMMinimizer();
	TEST_NOT_EQUAL(em, 0)
RESULT	

CHECK(ShiftedLVMMMinimizer::~ShiftedLVMMMinimizer())
	delete em;
RESULT

CHECK(ShiftedLVMMMinimizer::getForceField())
	ShiftedLVMMMinimizer em;
	TEST_EQUAL(em.getForceField(), 0)
RESULT

CHECK(ShiftedLVMMMinimizer::ShiftedLVMMMinimizer(ForceField&))
	TEST_EQUAL(FF.isValid(), true)
	ShiftedLVMMMinimizer em(FF);
	TEST_EQUAL(em.getForceField(), &FF)
RESULT

CHECK(ShiftedLVMMMinimizer::ShiftedLVMMMinimizer(ForceField&, const Options&))
	Options options;
	options.set("ABC", "DEF");
	em = new ShiftedLVMMMinimizer(FF, options);
	TEST_EQUAL(em->getForceField(), &FF)
	TEST_EQUAL(em->options["ABC"], "DEF")
	delete em;
RESULT

CHECK(ShiftedLVMMMinimizer::ShiftedLVMMMinimizer(const ShiftedLVMMMinimizer&, bool))
	ShiftedLVMMMinimizer em1;
	ShiftedLVMMMinimizer em2(em1);
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	ShiftedLVMMMinimizer em3(em1);
	test = (em1 == em3);
	TEST_EQUAL(test, true)
	em1.setup(FF);
RESULT

CHECK(ShiftedLVMMMinimizer::operator = (const ShiftedLVMMMinimizer&))
	ShiftedLVMMMinimizer em1;
	ShiftedLVMMMinimizer em2;
	em2 = em1;
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	ShiftedLVMMMinimizer em3;
	em3 = em1;
	test = (em1 == em3);
	TEST_EQUAL(test, true)
RESULT

CHECK(ShiftedLVMMMinimizer::isValid() const)
	em = new ShiftedLVMMMinimizer;
	TEST_EQUAL(em->isValid(), false)
	delete em;
	em = new ShiftedLVMMMinimizer(FF);
	TEST_EQUAL(em->isValid(), true)
	delete em;
RESULT

CHECK(ShiftedLVMMMinimizer::setup(ForceField&))
	ShiftedLVMMMinimizer e_min(FF);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
RESULT

CHECK(ShiftedLVMMMinimizer::setup(ForceField&, const Options&))
	Options options;
	options.setInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, 3456);
	ShiftedLVMMMinimizer e_min(FF, options);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 3456)
RESULT

CHECK(ShiftedLVMMMinimizer::specificSetup())
	ShiftedLVMMMinimizer em(FF);
	TEST_EQUAL(em.specificSetup(), true)
	// specificSetup() shouldn't do anything except returning true
RESULT

CHECK(ShiftedLVMMMinimizer::getNumberOfIterations() const)
	ShiftedLVMMMinimizer e_min;
	TEST_EQUAL(e_min.getNumberOfIterations(), 0)
RESULT

CHECK(ShiftedLVMMMinimizer::setNumberOfIteration(Size))
	ShiftedLVMMMinimizer e_min;
	e_min.setNumberOfIterations(4);
	TEST_EQUAL(e_min.getNumberOfIterations(), 4)
RESULT

CHECK(ShiftedLVMMMinimizer::getMaxNumberOfIterations())
	ShiftedLVMMMinimizer e_min;
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 0)
RESULT

CHECK(ShiftedLVMMMinimizer::setMaxNumberOfIterations(Size))
	ShiftedLVMMMinimizer e_min;
	e_min.setMaxNumberOfIterations(2000);
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 2000)
RESULT

CHECK(ShiftedLVMMMinimizer::getEnergyOutputFrequency() const)
	ShiftedLVMMMinimizer e_min2;
	TEST_EQUAL(e_min2.getEnergyOutputFrequency(), 0)
RESULT

CHECK(ShiftedLVMMMinimizer::setEnergyOutputFrequency(Size))
	ShiftedLVMMMinimizer e_min;
	e_min.setEnergyOutputFrequency(8);
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 8)
RESULT

CHECK(ShiftedLVMMMinimizer::getEnergyDifferenceBound() const)
	ShiftedLVMMMinimizer e_min;
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 0.0)
RESULT

CHECK(ShiftedLVMMMinimizer::setEnergyDifferenceBound(float))
	ShiftedLVMMMinimizer e_min;
	e_min.setEnergyDifferenceBound(9.0);
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 9.0)
RESULT

CHECK(ShiftedLVMMMinimizer::getMaximumDisplacement() const)
	ShiftedLVMMMinimizer e_min;
	TEST_EQUAL(e_min.getMaximumDisplacement(), 0.0)
RESULT

CHECK(ShiftedLVMMMinimizer::setMaximumDisplacement(float))
	ShiftedLVMMMinimizer e_min;
	e_min.setMaximumDisplacement(56.0);
	TEST_EQUAL(e_min.getMaximumDisplacement(), 56.0)
RESULT

CHECK(ShiftedLVMMMinimizer::setMaxNumOfColumns(Size num))	
	ShiftedLVMMMinimizer e_min;
	e_min.setMaxNumOfColumns(11);
	TEST_EQUAL(e_min.getMaxNumOfColumns(), 11)
RESULT

CHECK(ShiftedLVMMMinimizer::getMaxNumOfColumns() const)	
	ShiftedLVMMMinimizer e_min;
	TEST_EQUAL(e_min.getMaxNumOfColumns(), 5)
RESULT

CHECK(ShiftedLVMMMinimizer::getUpdateMethod() const)	
	ShiftedLVMMMinimizer e_min;
	TEST_EQUAL(e_min.getUpdateMethod(), ShiftedLVMMMinimizer::RANK_2)
RESULT
			
CHECK(ShiftedLVMMMinimizer::setUpdateMethod(UpdateMethod rank))	
	ShiftedLVMMMinimizer e_min;
	e_min.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	TEST_EQUAL(e_min.getUpdateMethod(), ShiftedLVMMMinimizer::RANK_1)
RESULT

CHECK(ShiftedLVMMMinimizer::getCorrectionParameter() const)	
	ShiftedLVMMMinimizer e_min;
	TEST_EQUAL(e_min.getCorrectionParameter(), ShiftedLVMMMinimizer::RATIO_OF_SHIFT_PARAMS)
RESULT

CHECK(ShiftedLVMMMinimizer::setCorrectionParameter(CorrectionParameter par))	
	ShiftedLVMMMinimizer e_min;
	e_min.setCorrectionParameter(ShiftedLVMMMinimizer::UNIT_VALUE);
	TEST_EQUAL(e_min.getCorrectionParameter(), ShiftedLVMMMinimizer::UNIT_VALUE)
RESULT

CHECK(ShiftedLVMMMinimizer::setCorrectionParameter(CorrectionParameter par))	
	ShiftedLVMMMinimizer e_min;
	e_min.setCorrectionParameter(ShiftedLVMMMinimizer::BALANCING_VALUE);
	TEST_EQUAL(e_min.getCorrectionParameter(), ShiftedLVMMMinimizer::BALANCING_VALUE)
RESULT

CHECK(ShiftedLVMMMinimizer::setCorrectionParameter(CorrectionParameter par))	
	ShiftedLVMMMinimizer e_min;
	e_min.setCorrectionParameter(ShiftedLVMMMinimizer::SQUARE_ROOT);
	TEST_EQUAL(e_min.getCorrectionParameter(), ShiftedLVMMMinimizer::SQUARE_ROOT)
RESULT

CHECK(ShiftedLVMMMinimizer::setCorrectionParameter(CorrectionParameter par))	
	ShiftedLVMMMinimizer e_min;
	e_min.setCorrectionParameter(ShiftedLVMMMinimizer::GEOMETRIC_MEAN);
	TEST_EQUAL(e_min.getCorrectionParameter(), ShiftedLVMMMinimizer::GEOMETRIC_MEAN)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) RANK_1)
	System S;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	S.insert(*m);
	m->insert(*a1);
	m->insert(*a2);
	a1->setPosition(Vector3(-0.5, 0, 0));
	a2->setPosition(Vector3(0.5, 0, 0));
	a1->setElement(PTE[Element::C]);
	a2->setElement(PTE[Element::C]);
	a1->setTypeName("C");
	a2->setTypeName("C");
	a1->setCharge(0.0);
	a2->setCharge(0.0);
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(10)
	TEST_REAL_EQUAL(FF.getEnergy(), 3.42854e6)

	PRECISION(1e-4)
	ShiftedLVMMMinimizer slvmm(FF);
	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.0005);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setEnergyDifferenceBound(0.00000001);
	TEST_EQUAL(slvmm.isValid(), true)
	
	FF.updateEnergy();
	FF.updateForces();
	bool result = slvmm.minimize(20);
	TEST_EQUAL(result, true)
			
	FF.updateEnergy();
	FF.updateForces();
	
	TEST_REAL_EQUAL(FF.getEnergy(), -0.359813)	
	TEST_EQUAL(FF.getRMSGradient() <= slvmm.getMaxGradient(), true)	
	PRECISION(5e-3)
	TEST_REAL_EQUAL(a1->getPosition().getDistance(a2->getPosition()), 3.81244)
RESULT


CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_1 UNIT_VALUE)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::UNIT_VALUE);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_1 BALANCING_VALUE)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::BALANCING_VALUE);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_1 SQUARE_ROOT)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::SQUARE_ROOT);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_1 GEOMETRIC_MEAN)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::GEOMETRIC_MEAN);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_1 RATIO_OF_SHIFT_PARAMS)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_1);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::RATIO_OF_SHIFT_PARAMS);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_2 UNIT_VALUE)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::UNIT_VALUE);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_2 BALANCING_VALUE)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::BALANCING_VALUE);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_2 SQUARE_ROOT)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::SQUARE_ROOT);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_2 GEOMETRIC_MEAN)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::GEOMETRIC_MEAN);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) AlaAla RANK_2 RATIO_OF_SHIFT_PARAMS)	
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AA.hin));
	f >> S;
	S.deselect();
	
	FragmentDB fd("");
	S.apply(fd.normalize_names);
	ResidueChecker checker(fd);
	S.apply(checker);
	
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "true";
	FF.options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
	FF.options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
	FF.setup(S);

	TEST_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(1E-2)
	TEST_REAL_EQUAL(FF.getEnergy(), -135.909)

	ShiftedLVMMMinimizer slvmm(FF);

	slvmm.setEnergyOutputFrequency(1);
	slvmm.setMaxGradient(0.04);
	slvmm.setEnergyDifferenceBound(0.000001);
	slvmm.setUpdateMethod(ShiftedLVMMMinimizer::RANK_2);
	slvmm.setCorrectionParameter(ShiftedLVMMMinimizer::RATIO_OF_SHIFT_PARAMS);
	
	TEST_EQUAL(slvmm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	slvmm.setMaxNumberOfIterations(10000);
	bool result = slvmm.minimize(1000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << slvmm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT


CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_1 UNIT_VALUE)
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

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_1 BALANCING_VALUE)
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

CHECK(ShiftedLVMMMinimizer::minimize(Size, bool) Ethane RANK_1 SQUARE_ROOT)
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
