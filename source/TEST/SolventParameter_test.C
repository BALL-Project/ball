// $Id: SolventParameter_test.C,v 1.6 2001/08/14 20:28:14 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SOLVATION/solventParameter.h>
///////////////////////////

START_TEST(SolventParameter, "$Id: SolventParameter_test.C,v 1.6 2001/08/14 20:28:14 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(SolventParameter())
  SolventParameter* s_ptr;
	s_ptr = new SolventParameter;
	TEST_NOT_EQUAL(s_ptr,0)
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
