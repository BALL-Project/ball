// $Id: Pair6_12InteractionEnergyProcessor_test.C,v 1.5 2000/11/14 19:13:06 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/SOLVATION/pair6_12InteractionEnergyProcessor.h>

///////////////////////////

START_TEST(class_name, "$Id: Pair6_12InteractionEnergyProcessor_test.C,v 1.5 2000/11/14 19:13:06 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

///  insert tests for each member function here         
///
	
// GROSSBAUSTELLE

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
	// BAUSTELLE
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::clear())
	// BAUSTELLE
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::operator = (const Pair6_12InteractionEnergyProcessor& proc))
	// BAUSTELLE
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
	S.apply(proc);
	val = proc.getEnergy();
	TEST_REAL_EQUAL(val, -6.027207050)

	proc.options.set(Pair6_12InteractionEnergyProcessor::Option::RDF_FILENAME,
	"data/6_12-test.rdf.ini");
	S.apply(proc);
	val = proc.getEnergy();
	TEST_REAL_EQUAL(val, -6.027207050)

	// BAUSTELLE: USE_RDF=true geht trotz nicht gesetzten Dateinamens!!!
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::isValid())
	Pair6_12InteractionEnergyProcessor proc;
	TEST_EQUAL(proc.isValid(), false)

	// BAUSTELLE
RESULT

CHECK(Pair6_12InteractionEnergyProcessor::operator == ())
  // BAUSTELLE
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
