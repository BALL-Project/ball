// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/parameters.h>

///////////////////////////

START_TEST(Parameters)

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
  String filename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  Parameters para(filename);
  TEST_EQUAL(para.getFilename(), filename)
RESULT


CHECK(Parameters::Parameters(const Parameters& parameter))
  String filename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  Parameters para(filename);
  TEST_EQUAL(para.getFilename(), filename)
  Parameters para2(para);
  TEST_EQUAL(para.getFilename(), filename)
RESULT


CHECK(Parameters::clear())
  String filename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  Parameters para(filename);
  INIFile* inif = &para.getParameterFile();
  para.clear();
  TEST_EQUAL(para.isValid(), false)
  TEST_EQUAL(inif->isValid(), false)
RESULT


CHECK(Parameters::Parameters& operator = (const Parameters& parameters))
  String filename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  Parameters para(filename);
  Parameters para2;
  para2 = para;
  TEST_EQUAL(para2.getFilename(),BALL_TEST_DATA_PATH(Parameters_test.ini))
  TEST_EQUAL(para2 == para, true)
RESULT


CHECK(Parameters::setFilename(const String& filename) + Parameters::getFilename() const + Parameters::getParameterFile())
  Parameters para;
  para.setFilename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  TEST_EQUAL(para.getFilename(),BALL_TEST_DATA_PATH(Parameters_test.ini))
  INIFile* inif = &para.getParameterFile();
  TEST_NOT_EQUAL(inif,0)
RESULT


CHECK(Parameters::init())
  Parameters para;
  para.setFilename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  bool test = para.init();
  TEST_EQUAL(test,true)
RESULT


CHECK(Parameters::isValid() const )
  String filename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  Parameters para(filename);
  TEST_EQUAL(para.isValid(),true)
RESULT


CHECK(bool Parameters::operator == (const Parameters& parameters))
  String filename(BALL_TEST_DATA_PATH(Parameters_test.ini));
  Parameters para(filename);
  Parameters para2(para);  
  bool test = (para == para2);
  TEST_EQUAL(test,true)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
