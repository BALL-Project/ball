// $Id: RDFSection_test.C,v 1.3 2001/07/15 17:32:41 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/RDFSection.h>

///////////////////////////

START_TEST(RDFSection, "$Id: RDFSection_test.C,v 1.3 2001/07/15 17:32:41 amoll Exp $")

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
