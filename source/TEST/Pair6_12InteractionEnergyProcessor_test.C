// $Id: Pair6_12InteractionEnergyProcessor_test.C,v 1.2 2000/10/06 11:51:56 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/SOLVATION/pair6_12InteractionEnergyProcessor.h>

///////////////////////////

START_TEST(class_name, "$Id: Pair6_12InteractionEnergyProcessor_test.C,v 1.2 2000/10/06 11:51:56 anker Exp $")

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
	System S;
	HINFile f("6_12-test.hin");
	f >> S;
	f.close();
	INIFile ini("6_12-test.rul");
	ini.read();
	RadiusRuleProcessor radius_rules;
	radius_rules.initialize(ini, "RadiusRules");
	S.apply(radius_rules);

	Pair6_12InteractionEnergyProcessor proc;
	proc.options.readOptionFile("6_12-test.options");
	Log.info() << "use_rdf = " 
		<< proc.options[Pair6_12InteractionEnergyProcessor::Option::USE_RDF] 
		<< endl;

	S.apply(proc);
	double val = proc.getEnergy();
	TEST_REAL_EQUAL(val, 0.01)
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
