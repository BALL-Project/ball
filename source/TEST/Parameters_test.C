// $Id: Parameters_test.C,v 1.3 2000/10/07 12:01:39 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/parameters.h>

///////////////////////////

START_TEST(Parameters, "$Id: Parameters_test.C,v 1.3 2000/10/07 12:01:39 oliver Exp $")

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
	String filename("data/Parameters_test.ini");
  Parameters para(filename);
	TEST_EQUAL(para.getFilename(), filename)
RESULT


CHECK(Parameters::Parameters(const Parameters& parameter))
	String filename("data/Parameters_test.ini");
  Parameters para(filename);
	TEST_EQUAL(para.getFilename(), filename)
	Parameters para2(para);
	TEST_EQUAL(para.getFilename(), filename)
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
