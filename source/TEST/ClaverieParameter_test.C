// $Id: ClaverieParameter_test.C,v 1.1 2000/08/31 17:56:16 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/claverieParameter.h>

///////////////////////////

START_TEST(class_name, "$Id: ClaverieParameter_test.C,v 1.1 2000/08/31 17:56:16 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class ClaverieParameter::


// tests for class ClaverieParameter::

CHECK(ClaverieParameter::ClaverieParameter())
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::ClaverieParameter(Parameters& parameters))
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::(const ClaverieParameter& param)())
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::destroy())
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::clear())
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::set(const ClaverieParameter& param))
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::ClaverieParameter& operator = (const ClaverieParameter& param))
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::hasParameters(Atom::Type solvent_type, Atom::Type solute_type) const )
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::getParameters(Atom::Type solvent_type, Atom::Type solute_type) const )
  //BAUSTELLE
RESULT


CHECK(ClaverieParameter::extractSection(ForceFieldParameters& parameters, const String& section_name))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
