// $Id: RDFParameter_test.C,v 1.1 2000/08/31 13:43:08 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/RDFParameter.h>

///////////////////////////

START_TEST(class_name, "$Id: RDFParameter_test.C,v 1.1 2000/08/31 13:43:08 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///


CHECK(RDFParameter::RDFParameter())
  //BAUSTELLE
RESULT


CHECK(RDFParameter::RDFParameter(const RDFParameter& rdf_parameter))
  //BAUSTELLE
RESULT


CHECK(RDFParameter::~RDFParameter())
  //BAUSTELLE
RESULT


CHECK(RDFParameter::destroy())
  //BAUSTELLE
RESULT


CHECK(RDFParameter::clear())
  //BAUSTELLE
RESULT


CHECK(RDFParameter::set(const RDFParameter& rdf_parameter))
  //BAUSTELLE
RESULT


CHECK(RDFParameter::RDFParameter& operator = (const RDFParameter& rdf_parameter))
  //BAUSTELLE
RESULT


CHECK(RDFParameter::getIndex(Atom::Type type_i, Atom::Type type_j) const )
  //BAUSTELLE
RESULT


CHECK(RDFParameter::getRDF(Atom::Type type_i, Atom::Type type_j) const )
  //BAUSTELLE
RESULT


CHECK(RDFParameter::getRDF(Position index) const )
  //BAUSTELLE
RESULT


CHECK(RDFParameter::extractSection(ForceFieldParameters& parameters, const String& section_name))
  //BAUSTELLE
RESULT


CHECK(RDFParameter::hasRDF(Atom::Type solvent_atom_type, Atom::Type solute_atom_type) const )
  //BAUSTELLE
RESULT


CHECK(RDFParameter::hasParameters(Atom::Type solvent_atom_type, Atom::Type solute_atom_type) const )
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
