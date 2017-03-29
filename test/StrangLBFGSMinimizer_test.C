// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// NOTE: we are still missing a test that triggers a restart in the minimization after
//			 the line search fails

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/strangLBFGS.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
///////////////////////////

START_TEST(StrangLBFGSMinimizer)

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

System S;
AmberFF FF(S);

StrangLBFGSMinimizer*	em;
CHECK(StrangLBFGSMinimizer::StrangLBFGSMinimizer())
	em = new StrangLBFGSMinimizer();
	TEST_NOT_EQUAL(em, 0)
RESULT	

CHECK(StrangLBFGSMinimizer::~StrangLBFGSMinimizer())
	delete em;
RESULT

CHECK(StrangLBFGSMinimizer::getForceField())
	StrangLBFGSMinimizer em;
	TEST_EQUAL(em.getForceField(), 0)
RESULT

CHECK(StrangLBFGSMinimizer::StrangLBFGSMinimizer(ForceField&))
	TEST_EQUAL(FF.isValid(), true)
	StrangLBFGSMinimizer em(FF);
	TEST_EQUAL(em.getForceField(), &FF)
RESULT

CHECK(StrangLBFGSMinimizer::StrangLBFGSMinimizer(ForceField&, const Options&))
	Options options;
	options.set("ABC", "DEF");
	em = new StrangLBFGSMinimizer(FF, options);
	TEST_EQUAL(em->getForceField(), &FF)
	TEST_EQUAL(em->options["ABC"], "DEF")
	delete em;
RESULT

CHECK(StrangLBFGSMinimizer::StrangLBFGSMinimizer(const StrangLBFGSMinimizer&, bool))
	StrangLBFGSMinimizer em1;
	StrangLBFGSMinimizer em2(em1);
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	StrangLBFGSMinimizer em3(em1);
	test = (em1 == em3);
	TEST_EQUAL(test, true)
	em1.setup(FF);
RESULT

CHECK(StrangLBFGSMinimizer::operator = (const StrangLBFGSMinimizer&))
	StrangLBFGSMinimizer em1;
	StrangLBFGSMinimizer em2;
	em2 = em1;
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	StrangLBFGSMinimizer em3;
	em3 = em1;
	test = (em1 == em3);
	TEST_EQUAL(test, true)
RESULT

CHECK(StrangLBFGSMinimizer::isValid() const)
	em = new StrangLBFGSMinimizer;
	TEST_EQUAL(em->isValid(), false)
	delete em;
	em = new StrangLBFGSMinimizer(FF);
	TEST_EQUAL(em->isValid(), true)
	delete em;
RESULT

CHECK(StrangLBFGSMinimizer::setup(ForceField&))
	StrangLBFGSMinimizer e_min(FF);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
RESULT

CHECK(StrangLBFGSMinimizer::setup(ForceField&, const Options&))
	Options options;
	options.setInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, 3456);
	StrangLBFGSMinimizer e_min(FF, options);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 3456)
RESULT

CHECK(StrangLBFGSMinimizer::specificSetup())
	StrangLBFGSMinimizer em(FF);
	TEST_EQUAL(em.specificSetup(), true)
	// specificSetup() shouldn't do anything except returning true
RESULT

CHECK(StrangLBFGSMinimizer::getNumberOfIterations() const)
	StrangLBFGSMinimizer e_min;
	TEST_EQUAL(e_min.getNumberOfIterations(), 0)
RESULT

CHECK(StrangLBFGSMinimizer::setNumberOfIteration(Size))
	StrangLBFGSMinimizer e_min;
	e_min.setNumberOfIterations(4);
	TEST_EQUAL(e_min.getNumberOfIterations(), 4)
RESULT

CHECK(StrangLBFGSMinimizer::getMaxNumberOfIterations())
	StrangLBFGSMinimizer e_min;
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 0)
RESULT

CHECK(StrangLBFGSMinimizer::setMaxNumberOfIterations(Size))
	StrangLBFGSMinimizer e_min;
	e_min.setMaxNumberOfIterations(2000);
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 2000)
RESULT

CHECK(StrangLBFGSMinimizer::getEnergyOutputFrequency() const)
	StrangLBFGSMinimizer e_min2;
	TEST_EQUAL(e_min2.getEnergyOutputFrequency(), 0)
RESULT

CHECK(StrangLBFGSMinimizer::setEnergyOutputFrequency(Size))
	StrangLBFGSMinimizer e_min;
	e_min.setEnergyOutputFrequency(8);
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 8)
RESULT

CHECK(StrangLBFGSMinimizer::getEnergyDifferenceBound() const)
	StrangLBFGSMinimizer e_min;
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 0.0)
RESULT

CHECK(StrangLBFGSMinimizer::setEnergyDifferenceBound(float))
	StrangLBFGSMinimizer e_min;
	e_min.setEnergyDifferenceBound(9.0);
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 9.0)
RESULT

CHECK(StrangLBFGSMinimizer::getMaximumDisplacement() const)
	StrangLBFGSMinimizer e_min;
	TEST_EQUAL(e_min.getMaximumDisplacement(), 0.0)
RESULT

CHECK(StrangLBFGSMinimizer::setMaximumDisplacement(float))
	StrangLBFGSMinimizer e_min;
	e_min.setMaximumDisplacement(56.0);
	TEST_EQUAL(e_min.getMaximumDisplacement(), 56.0)
RESULT

CHECK(StrangLBFGSMinimizer::setMaxNumOfStoredVectPairs(Size num))	
	StrangLBFGSMinimizer e_min;
	e_min.setMaxNumOfStoredVectPairs(11);
	TEST_EQUAL(e_min.getMaxNumOfStoredVectPairs(), 11)
RESULT

CHECK(StrangLBFGSMinimizer::getMaxNumOfStoredVectPairs())	
	StrangLBFGSMinimizer e_min;
	TEST_EQUAL(e_min.getMaxNumOfStoredVectPairs(), 5)
RESULT

CHECK(StrangLBFGSMinimizer::setImprovedFlag(bool flag))	
	StrangLBFGSMinimizer e_min;
	e_min.setImprovedFlag(false);
	TEST_EQUAL(e_min.getImprovedFlag(), false)
RESULT

CHECK(StrangLBFGSMinimizer::getImprovedFlag())	
	StrangLBFGSMinimizer e_min;
	TEST_EQUAL(e_min.getImprovedFlag(), true)
RESULT

CHECK(StrangLBFGSMinimizer::minimize(Size, bool, improved))
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
	StrangLBFGSMinimizer bfgsm(FF);
	bfgsm.setImprovedFlag(true);
	bfgsm.setEnergyOutputFrequency(1);
	bfgsm.setMaxGradient(0.0005);
	bfgsm.setEnergyDifferenceBound(0.00000001);
	TEST_EQUAL(bfgsm.isValid(), true)
	
	FF.updateEnergy();
	FF.updateForces();
	bool result = bfgsm.minimize(20);
	TEST_EQUAL(result, true)
			
	FF.updateEnergy();
	FF.updateForces();
	
HINFile test("schmadder.hin", std::ios::out);
test << S;
	TEST_REAL_EQUAL(FF.getEnergy(), -0.359813)	
	TEST_EQUAL(FF.getRMSGradient() <= bfgsm.getMaxGradient(), true)	
	PRECISION(5e-3)
	TEST_REAL_EQUAL(a1->getPosition().getDistance(a2->getPosition()), 3.81244)
RESULT

CHECK(StrangLBFGSMinimizer::minimize(Size, bool, not improved))
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
	StrangLBFGSMinimizer bfgsm(FF);
	bfgsm.setImprovedFlag(false);
	bfgsm.setEnergyOutputFrequency(1);
	bfgsm.setMaxGradient(0.0005);
	bfgsm.setEnergyDifferenceBound(0.00000001);
	TEST_EQUAL(bfgsm.isValid(), true)
	
	FF.updateEnergy();
	FF.updateForces();
	bool result = bfgsm.minimize(20);
	TEST_EQUAL(result, true)
			
	FF.updateEnergy();
	FF.updateForces();
	
	TEST_REAL_EQUAL(FF.getEnergy(), -0.359813)	
	TEST_EQUAL(FF.getRMSGradient() <= bfgsm.getMaxGradient(), true)	
	PRECISION(5e-3)
	TEST_REAL_EQUAL(a1->getPosition().getDistance(a2->getPosition()), 3.81244)
RESULT

CHECK(StrangLBFGSMinimizer::minimize(Size, bool, improved) AlaAla)	
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

	StrangLBFGSMinimizer bfgsm(FF);
	bfgsm.setImprovedFlag(true);
	bfgsm.setEnergyOutputFrequency(1);
	bfgsm.setMaxGradient(0.04);
	bfgsm.setEnergyDifferenceBound(0.000001);
	bfgsm.setMaximumDisplacement(20.0);
	
	TEST_EQUAL(bfgsm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bfgsm.setMaxNumberOfIterations(10000);
	bool result = bfgsm.minimize(500);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << bfgsm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT

CHECK(StrangLBFGSMinimizer::minimize(Size, bool, not improved) AlaAla)	
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

	StrangLBFGSMinimizer bfgsm(FF);
	bfgsm.setImprovedFlag(false);
	bfgsm.setEnergyOutputFrequency(1);
	bfgsm.setMaxGradient(0.04);
	bfgsm.setEnergyDifferenceBound(0.000001);
	bfgsm.setMaximumDisplacement(20.0);
	
	TEST_EQUAL(bfgsm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bfgsm.setMaxNumberOfIterations(10000);
	bool result = bfgsm.minimize(500);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << bfgsm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1E-1)
	TEST_REAL_EQUAL(energy, -415.5)
RESULT


CHECK(StrangLBFGSMinimizer::minimize(Size, bool, improved) Ethane)
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

	StrangLBFGSMinimizer bfgsm(FF);
	bfgsm.setImprovedFlag(true);
	bfgsm.setEnergyOutputFrequency(5);
	bfgsm.setMaxGradient(0.418);
	bfgsm.setEnergyDifferenceBound(0.00000001);

	TEST_EQUAL(bfgsm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = bfgsm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << bfgsm.getNumberOfIterations() << " steps")

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

CHECK(StrangLBFGSMinimizer::minimize(Size, bool, not improved) Ethane)
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

	StrangLBFGSMinimizer bfgsm(FF);
	bfgsm.setImprovedFlag(false);
	bfgsm.setEnergyOutputFrequency(5);
	bfgsm.setMaxGradient(0.418);
	bfgsm.setEnergyDifferenceBound(0.00000001);

	TEST_EQUAL(bfgsm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = bfgsm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << bfgsm.getNumberOfIterations() << " steps")

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
