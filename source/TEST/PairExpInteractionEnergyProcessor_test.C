// $Id: PairExpInteractionEnergyProcessor_test.C,v 1.4 2001/12/30 13:28:59 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/pairExpInteractionEnergyProcessor.h>

///////////////////////////

START_TEST(PairExpInteractionEnergyProcessor, "$Id: PairExpInteractionEnergyProcessor_test.C,v 1.4 2001/12/30 13:28:59 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

///  insert tests for each member function here         
///
	
// GROSS?????

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
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::destroy())
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::clear())
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::set(const PairExpInteractionEnergyProcessor& proc))
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::operator = (const PairExpInteractionEnergyProcessor& proc))
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::finish())
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::setRDF(const RadialDistributionFunction& RDF))
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::computeClaverieParameters_(Atom::Type solvent_type, Atom::Type solute_type, std::pair<float, float>& parameters))
	// ?????
RESULT

CHECK(PairExpInteractionEnergyProcessor::getExternalSurface_(vector< pair<Vector3, Surface> >& surface_map, const char* surface_file))
	// ?????
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
