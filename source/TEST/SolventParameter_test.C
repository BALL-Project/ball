// $Id: SolventParameter_test.C,v 1.4 2001/07/16 21:15:02 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SOLVATION/solventParameter.h>
///////////////////////////

START_TEST(SolventParameter, "$Id: SolventParameter_test.C,v 1.4 2001/07/16 21:15:02 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(SolventParameter())
  //BAUSTELLE
RESULT


CHECK(SolventParameter(Parameters& parameters))
  //BAUSTELLE
RESULT


CHECK(SolventParameter(const SolventParameter& param))
  //BAUSTELLE
RESULT


CHECK(~SolventParameter())
  //BAUSTELLE
RESULT


CHECK(destroy())
  //BAUSTELLE
RESULT


CHECK(clear())
  //BAUSTELLE
RESULT


CHECK(set(const SolventParameter& param))
  //BAUSTELLE
RESULT


CHECK(SolventParameter& operator = (const SolventParameter& param))
  //BAUSTELLE
RESULT


CHECK(getSolventDescriptor() const )
  //BAUSTELLE
RESULT


CHECK(getSolventDescriptor() )
  //BAUSTELLE
RESULT


CHECK(extractSection(ForceFieldParameters& parameters, const String& section_name))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
