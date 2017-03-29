// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/RDFParameter.h>

///////////////////////////

START_TEST(RDFParameter)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///


CHECK(RDFParameter::RDFParameter())
  //?????
RESULT


CHECK(RDFParameter::RDFParameter(const RDFParameter& rdf_parameter))
  //?????
RESULT


CHECK(RDFParameter::~RDFParameter())
  //?????
RESULT


CHECK(RDFParameter::destroy())
  //?????
RESULT


CHECK(RDFParameter::clear())
  //?????
RESULT


CHECK(RDFParameter::set(const RDFParameter& rdf_parameter))
  //?????
RESULT


CHECK(RDFParameter::RDFParameter& operator = (const RDFParameter& rdf_parameter))
  //?????
RESULT


CHECK(RDFParameter::getIndex(Atom::Type type_i, Atom::Type type_j) const )
  //?????
RESULT


CHECK(RDFParameter::getRDF(Atom::Type type_i, Atom::Type type_j) const )
  //?????
RESULT


CHECK(RDFParameter::getRDF(Position index) const )
  //?????
RESULT


CHECK(RDFParameter::extractSection(ForceFieldParameters& parameters, const String& section_name))
  //?????
RESULT


CHECK(RDFParameter::hasRDF(Atom::Type solvent_atom_type, Atom::Type solute_atom_type) const )
  //?????
RESULT


CHECK(RDFParameter::hasParameters(Atom::Type solvent_atom_type, Atom::Type solute_atom_type) const )
  //?????
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
