// $Id: Pair6_12InteractionEnergyProcessor_test.C,v 1.12 2001/12/30 13:28:59 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/SOLVATION/pair6_12InteractionEnergyProcessor.h>

///////////////////////////

START_TEST(Pair6_12InteractionEnergyProcessor, "$Id: Pair6_12InteractionEnergyProcessor_test.C,v 1.12 2001/12/30 13:28:59 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

///  insert tests for each member function here         
///
	
Pair6_12InteractionEnergyProcessor* pointer;

CHECK(Pair6_12InteractionEnergyProcessor::Pair6_12InteractionEnergyProcessor())
	pointer = new Pair6_12InteractionEnergyProcessor;
	TEST_NOT_EQUAL(pointer, 0)
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::~Pair6_12InteractionEnergyProcessor())
	delete pointer;
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::Pair6_12InteractionEnergyProcessor(const Pair6_12InteractionEnergyProcessor& proc))
	Pair6_12InteractionEnergyProcessor proc1;
	Pair6_12InteractionEnergyProcessor proc2(proc1);
	// ?????
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::setSolventDescriptor())
	Pair6_12InteractionEnergyProcessor proc;
	SolventDescriptor solvent;
	proc.setSolventDescriptor(solvent);
	bool test = (proc.getSolventDescriptor() == solvent);
	TEST_EQUAL(test, true)
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::setRDFParameters())
	Pair6_12InteractionEnergyProcessor proc;
	RDFParameter param;
	proc.setRDFParameters(param);
	bool test = (proc.getRDFParameter() == param);
	TEST_EQUAL(test, true)
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::setRDFIntegrator())
	Pair6_12InteractionEnergyProcessor proc;
	Pair6_12RDFIntegrator integ;
	proc.setRDFIntegrator(integ);
	bool test = (proc.getRDFIntegrator() == integ);
	TEST_EQUAL(test, true)
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::clear())
	Pair6_12InteractionEnergyProcessor proc;
	Pair6_12InteractionEnergyProcessor proc2;
	proc2.clear();
	bool test = (proc == proc2);
	TEST_EQUAL(test, true)
	SolventDescriptor solvent;
	proc2.setSolventDescriptor(solvent);
	proc2.clear();
	test = (proc == proc2);
	TEST_EQUAL(test, true)
	RDFParameter param;
	proc2.setRDFParameters(param);
	proc2.clear();
	test = (proc == proc2);
	TEST_EQUAL(test, true)
	Pair6_12RDFIntegrator integ;
	proc2.setRDFIntegrator(integ);
	proc2.clear();
	test = (proc == proc2);
	TEST_EQUAL(test, true)
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::operator = (const Pair6_12InteractionEnergyProcessor& proc))
	Pair6_12InteractionEnergyProcessor proc;
	SolventDescriptor solvent;
	RDFParameter param;
	Pair6_12RDFIntegrator integ;
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::finish())
	PRECISION(0.0001)
	System S;
	HINFile f("data/6_12-test.hin");
	f >> S;
	f.close();
	INIFile ini("data/6_12-test.rul");
	ini.read();
	RadiusRuleProcessor radius_rules;
	radius_rules.initialize(ini, "RadiusRules");
	S.apply(radius_rules);

	Pair6_12InteractionEnergyProcessor proc;
	proc.options.readOptionFile("data/6_12-test.options");

	S.apply(proc);
	double val = proc.getEnergy();
	TEST_REAL_EQUAL(val, -6.027207050)

	proc.options.set(Pair6_12InteractionEnergyProcessor::Option::RDF_FILENAME,
			"data/6_12-test.rdf-fake.ini");
	proc.options.setBool(Pair6_12InteractionEnergyProcessor::Option::USE_RDF,
			true);
	proc.options.setInteger(Pair6_12RDFIntegrator::Option::METHOD,
			Pair6_12RDFIntegrator::METHOD__TRAPEZIUM);
	S.apply(proc);
	val = proc.getEnergy();
	TEST_REAL_EQUAL(val, -6.027207050)

	proc.options.set(Pair6_12InteractionEnergyProcessor::Option::RDF_FILENAME,
			"data/6_12-test.rdf.ini");
	S.apply(proc);
	val = proc.getEnergy();
	TEST_REAL_EQUAL(val, -7.62216)

	// ?????: USE_RDF=true geht trotz nicht gesetzten Dateinamens!!!
	// Was ist mit dem default? Exception?
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::isValid())
	Pair6_12InteractionEnergyProcessor proc;
	TEST_EQUAL(proc.isValid(), true)

	// ?????
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::operator == ())
	Pair6_12InteractionEnergyProcessor proc;
	Pair6_12InteractionEnergyProcessor proc2;
	bool test = (proc == proc2);
	TEST_EQUAL(test, true)
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
