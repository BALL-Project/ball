// $Id: SolventParameter_test.C,v 1.3 2001/07/15 17:32:41 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/solventParameter.h>

///////////////////////////

START_TEST(SolventParameter, "$Id: SolventParameter_test.C,v 1.3 2001/07/15 17:32:41 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(SolventAtomDescriptor::SolventParameter::SolventParameter())
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::SolventParameter(Parameters& parameters))
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::SolventParameter(const SolventParameter& param))
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::~SolventParameter())
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::destroy())
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::clear())
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::set(const SolventParameter& param))
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::SolventParameter& operator = (const SolventParameter& param))
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::getSolventDescriptor() const )
  //BAUSTELLE
RESULT


CHECK(SolventAtomDescriptor::SolventParameter::extractSection(ForceFieldParameters& parameters, const String& section_name))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
