// $Id: ParameterSection_test.C,v 1.2 2000/09/19 15:53:02 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/parameterSection.h>

///////////////////////////

START_TEST(Parameters, "$Id: ParameterSection_test.C,v 1.2 2000/09/19 15:53:02 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(ParameterSection::ParameterSection())
  //BAUSTELLE
RESULT


CHECK(ParameterSection::~ParameterSection())
  //BAUSTELLE
RESULT


CHECK(ParameterSection::destroy())
  //BAUSTELLE
RESULT


CHECK(ParameterSection::extractSection(Parameters& parameters, const String& section_name))
  //BAUSTELLE
RESULT


CHECK(ParameterSection::getValue(const String& key, const String& variable) const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::has(const String& key, const String& variable) const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::has(const String& key) const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::hasVariable(const String& variable) const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::getColumnIndex(const String& variable) const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::getNumberOfVariables() const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::getNumberOfKeys() const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::getValue(Position key_index, Position variable_index) const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::getKey(Position key_index) const )
  //BAUSTELLE
RESULT


CHECK(ParameterSection::isValid() const )
  //BAUSTELLE
RESULT


											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
