// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
///////////////////////////

START_TEST(ConjugateGradientMinimizer)

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

System S;
AmberFF FF(S);

ConjugateGradientMinimizer*	em;
CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer())
	em = new ConjugateGradientMinimizer();
	TEST_NOT_EQUAL(em, 0)
RESULT	

CHECK(ConjugateGradientMinimizer::~ConjugateGradientMinimizer())
	delete em;
RESULT

CHECK(ConjugateGradientMinimizer::getForceField())
	ConjugateGradientMinimizer em;
	TEST_EQUAL(em.getForceField(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField&))
	TEST_EQUAL(FF.isValid(), true)
	ConjugateGradientMinimizer em(FF);
	TEST_EQUAL(em.getForceField(), &FF)
RESULT

CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField&, const Options&))
	Options options;
	options.set("ABC", "DEF");
	em = new ConjugateGradientMinimizer(FF, options);
	TEST_EQUAL(em->getForceField(), &FF)
	TEST_EQUAL(em->options["ABC"], "DEF")
	delete em;
RESULT

CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer(const ConjugateGradientMinimizer&, bool))
	ConjugateGradientMinimizer em1;
	ConjugateGradientMinimizer em2(em1);
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	ConjugateGradientMinimizer em3(em1);
	test = (em1 == em3);
	TEST_EQUAL(test, true)
	em1.setup(FF);
RESULT

CHECK(ConjugateGradientMinimizer::operator = (const ConjugateGradientMinimizer&))
	ConjugateGradientMinimizer em1;
	ConjugateGradientMinimizer em2;
	em2 = em1;
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	ConjugateGradientMinimizer em3;
	em3 = em1;
	test = (em1 == em3);
	TEST_EQUAL(test, true)
RESULT

CHECK(ConjugateGradientMinimizer::isValid() const)
	em = new ConjugateGradientMinimizer;
	TEST_EQUAL(em->isValid(), false)
	delete em;
	em = new ConjugateGradientMinimizer(FF);
	TEST_EQUAL(em->isValid(), true)
	delete em;
RESULT

CHECK(ConjugateGradientMinimizer::setup(ForceField&))
	ConjugateGradientMinimizer e_min(FF);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
RESULT

CHECK(ConjugateGradientMinimizer::setup(ForceField&, const Options&))
	Options options;
	options.setInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, 3456);
	ConjugateGradientMinimizer e_min(FF, options);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 3456)
RESULT

CHECK(ConjugateGradientMinimizer::specificSetup())
	ConjugateGradientMinimizer em(FF);
	TEST_EQUAL(em.specificSetup(), true)
	// specificSetup() shouldn't do anything except returning true
RESULT

CHECK(ConjugateGradientMinimizer::getNumberOfIterations() const)
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getNumberOfIterations(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::setNumberOfIteration(Size))
	ConjugateGradientMinimizer e_min;
	e_min.setNumberOfIterations(4);
	TEST_EQUAL(e_min.getNumberOfIterations(), 4)
RESULT

CHECK(ConjugateGradientMinimizer::getMaxNumberOfIterations())
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::setMaxNumberOfIterations(Size))
	ConjugateGradientMinimizer e_min;
	e_min.setMaxNumberOfIterations(2000);
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 2000)
RESULT

CHECK(ConjugateGradientMinimizer::getEnergyOutputFrequency() const)
	ConjugateGradientMinimizer e_min2;
	TEST_EQUAL(e_min2.getEnergyOutputFrequency(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::setEnergyOutputFrequency(Size))
	ConjugateGradientMinimizer e_min;
	e_min.setEnergyOutputFrequency(8);
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 8)
RESULT

CHECK(ConjugateGradientMinimizer::getEnergyDifferenceBound() const)
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 0.0)
RESULT

CHECK(ConjugateGradientMinimizer::setEnergyDifferenceBound(float))
	ConjugateGradientMinimizer e_min;
	e_min.setEnergyDifferenceBound(9.0);
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 9.0)
RESULT

CHECK(ConjugateGradientMinimizer::getMaximumDisplacement() const)
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getMaximumDisplacement(), 0.0)
RESULT

CHECK(ConjugateGradientMinimizer::setMaximumDisplacement(float))
	ConjugateGradientMinimizer e_min;
	e_min.setMaximumDisplacement(56.0);
	TEST_EQUAL(e_min.getMaximumDisplacement(), 56.0)
RESULT

CHECK(ConjugateGradientMinimizer::minimize(Size, bool))
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
	ConjugateGradientMinimizer cgm(FF);
	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(0.0005);
	cgm.setEnergyDifferenceBound(0.00000001);
	TEST_EQUAL(cgm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = cgm.minimize(20);
	TEST_EQUAL(result, true)
	FF.updateEnergy();
	FF.updateForces();

	TEST_REAL_EQUAL(FF.getEnergy(), -0.359813)	
	TEST_EQUAL(FF.getRMSGradient() <= cgm.getMaxGradient(), true)	
	PRECISION(5e-3)
	TEST_REAL_EQUAL(a1->getPosition().getDistance(a2->getPosition()), 3.81244)
RESULT

CHECK(ConjugateGradientMinimizer::minimize(Size, bool, FLETCHER_REEVES) AlaAla)
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

	ConjugateGradientMinimizer cgm(FF);

	cgm.setEnergyOutputFrequency(5);
	cgm.setMaxGradient(0.0001);
	cgm.setEnergyDifferenceBound(0.000001);
	cgm.setUpdateMethod(ConjugateGradientMinimizer::FLETCHER_REEVES);
	ConjugateGradientMinimizer::UpdateMethod um;
	um = cgm.getUpdateMethod();
	TEST_EQUAL(um, ConjugateGradientMinimizer::FLETCHER_REEVES);
	
	TEST_EQUAL(cgm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	cgm.setMaxNumberOfIterations(10000);
	bool result = cgm.minimize(5000);

	STATUS("FR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << cgm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1.)
	TEST_REAL_EQUAL(energy, -415.)
RESULT

CHECK(ConjugateGradientMinimizer::minimize(Size, bool, POLAK_RIBIERE) AlaAla)
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

	ConjugateGradientMinimizer cgm(FF);

	cgm.setEnergyOutputFrequency(5);
	cgm.setMaxGradient(0.1);
	cgm.setEnergyDifferenceBound(0.00000001);
	cgm.setUpdateMethod(ConjugateGradientMinimizer::POLAK_RIBIERE);
	ConjugateGradientMinimizer::UpdateMethod um;
	um = cgm.getUpdateMethod();
	TEST_EQUAL(um, ConjugateGradientMinimizer::POLAK_RIBIERE);
	
	TEST_EQUAL(cgm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	cgm.setMaxNumberOfIterations(10000);
	bool result = cgm.minimize(5000);

	TEST_EQUAL(result, true)
	STATUS("PR/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << cgm.getNumberOfIterations() << " steps")

	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1.)
	TEST_REAL_EQUAL(energy, -415.)
RESULT

CHECK(ConjugateGradientMinimizer::minimize(Size, bool, SHANNO) AlaAla)
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

	ConjugateGradientMinimizer cgm(FF);

	cgm.setEnergyOutputFrequency(5);
	cgm.setMaxGradient(0.01);
	cgm.setEnergyDifferenceBound(0.00000001);
	cgm.setUpdateMethod(ConjugateGradientMinimizer::SHANNO);
	ConjugateGradientMinimizer::UpdateMethod um;
	um = cgm.getUpdateMethod();
	TEST_EQUAL(um, ConjugateGradientMinimizer::SHANNO);
	
	TEST_EQUAL(cgm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	cgm.setMaxNumberOfIterations(10000);
	bool result = cgm.minimize(5000);

	STATUS("SH/AlaAla -- " << (result ? "" : "not ") << "converged after " 
				 << cgm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(1.)
	TEST_REAL_EQUAL(energy, -415.)
RESULT

CHECK(ConjugateGradientMinimizer::minimize(Size, bool, FLETCHER_REEVES))
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

	ConjugateGradientMinimizer cgm(FF);

	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(0.1);
	cgm.setEnergyDifferenceBound(0.00000001);
	cgm.setUpdateMethod(ConjugateGradientMinimizer::FLETCHER_REEVES);
	ConjugateGradientMinimizer::UpdateMethod um;
	um = cgm.getUpdateMethod();
	TEST_EQUAL(um, ConjugateGradientMinimizer::FLETCHER_REEVES);
	

	TEST_EQUAL(cgm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = cgm.minimize(500);

	STATUS("FR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << cgm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(3E-1)
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

CHECK(ConjugateGradientMinimizer::minimize(Size, bool, POLAK_RIBIERE))
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

	ConjugateGradientMinimizer cgm(FF);

	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(0.1);
	cgm.setEnergyDifferenceBound(0.00000001);
	cgm.setUpdateMethod(ConjugateGradientMinimizer::POLAK_RIBIERE);
	ConjugateGradientMinimizer::UpdateMethod um;
	um = cgm.getUpdateMethod();
	TEST_EQUAL(um, ConjugateGradientMinimizer::POLAK_RIBIERE);
	

	TEST_EQUAL(cgm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = cgm.minimize(500);

	STATUS("PR/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << cgm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(2E-2)
	TEST_REAL_EQUAL(energy, 5.906)

	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}

	PRECISION(3E-2)
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

CHECK(ConjugateGradientMinimizer::minimize(Size, bool, SHANNO) ethan)
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

	ConjugateGradientMinimizer cgm(FF);

	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(0.1);
	cgm.setEnergyDifferenceBound(0.00000001);
	cgm.setUpdateMethod(ConjugateGradientMinimizer::SHANNO);
	ConjugateGradientMinimizer::UpdateMethod um;
	um = cgm.getUpdateMethod();
	TEST_EQUAL(um, ConjugateGradientMinimizer::SHANNO);
	
	TEST_EQUAL(cgm.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	bool result = cgm.minimize(500);

	STATUS("SH/C2H6 -- " << (result ? "" : "not ") << "converged after " 
				 << cgm.getNumberOfIterations() << " steps")

	TEST_EQUAL(result, true)
	float energy = FF.updateEnergy();
	FF.updateForces();
	
	PRECISION(2E-2)
	TEST_REAL_EQUAL(energy, 5.906)

	AtomIterator atit;
	Vector3 pos[8];
	int i=0;

	for (atit = S.beginAtom(); +atit; ++atit)
	{
		pos[i] = atit->getPosition();
		++i;
	}

	PRECISION(3E-2)
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
