// $Id: Parameters_test.C,v 1.2 2000/10/05 17:16:57 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/parameters.h>

///////////////////////////

START_TEST(Parameters, "$Id: Parameters_test.C,v 1.2 2000/10/05 17:16:57 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

// tests for class Parameters::

Parameters* pointer;

CHECK(Parameters::Parameters())
  pointer = new Parameters;
	TEST_NOT_EQUAL(pointer, 0)
RESULT


CHECK(Parameters::~Parameters())
  delete pointer;
RESULT


CHECK(Parameters::Parameters(const String& filename))
	String filename("bla.file");
  Parameters para(filename);
	bool result = (para.getFilename() == filename);
	TEST_EQUAL(result, true)
RESULT


CHECK(Parameters::Parameters(const Parameters& parameter))
	Parameters para;

	Parameters PARA(para);
	
  //BAUSTELLE
RESULT


CHECK(Parameters::clear())
  //BAUSTELLE
RESULT


CHECK(Parameters::Parameters& operator = (const Parameters& parameters))
  //BAUSTELLE
RESULT


CHECK(Parameters::setFilename(const String& filename))
  //BAUSTELLE
RESULT


CHECK(Parameters::getFilename() const )
  //BAUSTELLE
RESULT


CHECK(Parameters::getParameterFile())
  //BAUSTELLE
RESULT


CHECK(Parameters::init())
  //BAUSTELLE
RESULT


CHECK(Parameters::isValid() const )
  //BAUSTELLE
RESULT


CHECK(bool Parameters::operator = (const Parameters& parameters))
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
