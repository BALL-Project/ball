// $Id: SolventParameter_test.C,v 1.1 2000/08/31 17:33:31 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/solventParameter.h>

///////////////////////////

START_TEST(class_name, "$Id: SolventParameter_test.C,v 1.1 2000/08/31 17:33:31 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

line 35: found class SolventAtomDescriptor::SolventParameter::
// tests for class SolventAtomDescriptor::SolventParameter::

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
