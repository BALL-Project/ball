// $Id: PairExpInteractionEnergyProcessor_test.C,v 1.3 2001/07/15 17:32:40 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/pairExpInteractionEnergyProcessor.h>

///////////////////////////

START_TEST(PairExpInteractionEnergyProcessor, "$Id: PairExpInteractionEnergyProcessor_test.C,v 1.3 2001/07/15 17:32:40 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

///  insert tests for each member function here         
///
	
// GROSSBAUSTELLE

PairExpInteractionEnergyProcessor* pointer;

CHECK(PairExpInteractionEnergyProcessor::PairExpInteractionEnergyProcessor())
	pointer = new PairExpInteractionEnergyProcessor;
	TEST_NOT_EQUAL(pointer, 0)
RESULT

CHECK(PairExpInteractionEnergyProcessor::~PairExpInteractionEnergyProcessor())
	delete pointer;
RESULT

CHECK(PairExpInteractionEnergyProcessor::PairExpInteractionEnergyProcessor(const PairExpInteractionEnergyProcessor& proc))
	PairExpInteractionEnergyProcessor proc1;
	PairExpInteractionEnergyProcessor proc2(proc1);
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::destroy())
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::clear())
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::set(const PairExpInteractionEnergyProcessor& proc))
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::operator = (const PairExpInteractionEnergyProcessor& proc))
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::finish())
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::setRDF(const RadialDistributionFunction& RDF))
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::computeClaverieParameters_(Atom::Type solvent_type, Atom::Type solute_type, std::pair<float, float>& parameters))
	// BAUSTELLE
RESULT

CHECK(PairExpInteractionEnergyProcessor::getExternalSurface_(vector< pair<Vector3, Surface> >& surface_map, const char* surface_file))
	// BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
