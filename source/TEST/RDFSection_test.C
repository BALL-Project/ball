// $Id: RDFSection_test.C,v 1.1 2000/08/31 13:33:59 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include "RDFSection.h"
// #include <BALL/STRUCTURE/RDFSection.h>

///////////////////////////

START_TEST(class_name, "$Id: RDFSection_test.C,v 1.1 2000/08/31 13:33:59 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

RDFSection* pointer;

CHECK(RDFSection::RDFSection())
  pointer = new RDFSection();
	TEST_NOT_EQUAL(pointer, 0)
RESULT


CHECK(RDFSection::~RDFSection())
  delete pointer;
RESULT


CHECK(RDFSection::RDFSection(const RDFSection& rdf_section))
  //BAUSTELLE
RESULT


CHECK(RDFSection::destroy())
  //BAUSTELLE
RESULT


CHECK(RDFSection::clear())
  //BAUSTELLE
RESULT


CHECK(RDFSection::set(const RDFSection& rdf_section))
  //BAUSTELLE
RESULT


CHECK(RDFSection::RDFSection& operator = (const RDFSection& rdf_section))
  //BAUSTELLE
RESULT


CHECK(RDFSection::extractSection(Parameters& parameters, const String& section_name))
  //BAUSTELLE
RESULT


CHECK(RDFSection::getRDF() const )
  //BAUSTELLE
RESULT




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
