// $Id: DefaultProcessors_test.C,v 1.2 2000/02/18 13:10:05 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(class_name, "$Id: DefaultProcessors_test.C,v 1.2 2000/02/18 13:10:05 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class ClearChargeProcessor::

CHECK(ClearChargeProcessor::Processor::Result operator()(Atom& atom))
  //BAUSTELLE
RESULT


// tests for class ClearRadiusProcessor::

CHECK(ClearRadiusProcessor::Processor::Result operator()(Atom& atom))
  //BAUSTELLE
RESULT


// tests for class AssignRadiusProcessor::

CHECK(AssignRadiusProcessor::AssignRadiusProcessor())
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::AssignRadiusProcessor(const String& filename))
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::start())
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::finish())
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::Processor::Result operator()(Atom& atom))
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::setFilename(const String& filename))
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::getFilename())
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::getNumberOfAssignments())
  //BAUSTELLE
RESULT


CHECK(AssignRadiusProcessor::getNumberOfErrors())
  //BAUSTELLE
RESULT


// tests for class AssignChargeProcessor::

CHECK(AssignChargeProcessor::AssignChargeProcessor())
  //BAUSTELLE
RESULT


CHECK(AssignChargeProcessor::AssignChargeProcessor(const String& filename))
  //BAUSTELLE
RESULT


CHECK(AssignChargeProcessor::start())
  //BAUSTELLE
RESULT


CHECK(AssignChargeProcessor::Processor::Result operator () (Atom& atom))
  //BAUSTELLE
RESULT


CHECK(AssignChargeProcessor::getTotalCharge())
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
