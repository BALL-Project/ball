// $Id: Parameters_test.C,v 1.5 2001/08/27 09:24:50 aubertin Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/parameters.h>

///////////////////////////

START_TEST(Parameters, "$Id: Parameters_test.C,v 1.5 2001/08/27 09:24:50 aubertin Exp $")

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
  String filename("data/Parameters_test.ini");
  Parameters para(filename);
  INIFile* inif = &para.getParameterFile();
  para.clear();
  TEST_EQUAL(para.isValid(),false)
  TEST_EQUAL( "", para.getFilename())
  TEST_EQUAL(inif->getDuplicateKeyCheck(),false)
  TEST_EQUAL(inif->isValid(),false)
RESULT


CHECK(Parameters::Parameters& operator = (const Parameters& parameters))
  String filename("data/Parameters_test.ini");
  Parameters para(filename);
  Parameters para2;
  para2 = para;
  TEST_EQUAL(para2.getFilename(),"data/Parameters_test.ini")
  TEST_EQUAL(para2 == para, true)
RESULT


CHECK(Parameters::setFilename(const String& filename) + Parameters::getFilename() const + Parameters::getParameterFile())
  Parameters para;
  para.setFilename("data/Parameters_test.ini");
  TEST_EQUAL(para.getFilename(),"data/Parameters_test.ini")
  INIFile* inif = &para.getParameterFile();
  TEST_NOT_EQUAL(inif,0)
RESULT


CHECK(Parameters::init())
  Parameters para;
  para.setFilename("data/Parameters_test.ini");
  bool test = para.init();
  TEST_EQUAL(test,true)
RESULT


CHECK(Parameters::isValid() const )
  String filename("data/Parameters_test.ini");
  Parameters para(filename);
  TEST_EQUAL(para.isValid(),true)
RESULT


CHECK(bool Parameters::operator == (const Parameters& parameters))
  String filename("data/Parameters_test.ini");
  Parameters para(filename);
  Parameters para2(para);  
  bool test = (para == para2);
  TEST_EQUAL(test,true)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
