// $Id: ParameterSection_test.C,v 1.5 2000/10/07 13:21:42 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/parameterSection.h>
#include <BALL/FORMAT/parameters.h>

///////////////////////////

START_TEST(Parameters, "$Id: ParameterSection_test.C,v 1.5 2000/10/07 13:21:42 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Parameters param("data/ParameterSection_test.ini");
param.init();

ParameterSection* ps = 0;
CHECK(ParameterSection::ParameterSection())
	ps = new ParameterSection;
	TEST_NOT_EQUAL(ps, 0)
RESULT


CHECK(ParameterSection::~ParameterSection())
	delete ps;
RESULT


CHECK(ParameterSection::clear())
  //BAUSTELLE
	ParameterSection ps;
	ps.extractSection(param, "Section2");
	ps.clear();
	ParameterSection ps_new;
	// assumes that operator == () is correct.
	bool result = (ps == ps_new);
	TEST_EQUAL(result, true);
RESULT

CHECK(ParameterSection::getSectionName() const)
	ParameterSection ps;
	TEST_EQUAL(ps.getSectionName(), "")
RESULT

CHECK(ParameterSection::extractSection(Parameters& parameters, const String& section_name))
	ParameterSection ps;
	bool result = ps.extractSection(param, "Section1");
	TEST_EQUAL(result, false)
	TEST_EQUAL(ps.getSectionName(), "Section1")
	result = ps.extractSection(param, "Section2");
	TEST_EQUAL(result, true)
	TEST_EQUAL(ps.getSectionName(), "Section2")
	result = ps.extractSection(param, "Section3");
	TEST_EQUAL(result, true)
	TEST_EQUAL(ps.getSectionName(), "Section3")
	result = ps.extractSection(param, "Section4");
	TEST_EQUAL(result, true)
	TEST_EQUAL(ps.getSectionName(), "Section4")
	result = ps.extractSection(param, "Section5");
	TEST_EQUAL(result, true)
	TEST_EQUAL(ps.getSectionName(), "Section5")
	result = ps.extractSection(param, "Section6");
	TEST_EQUAL(result, false)
	TEST_EQUAL(ps.getSectionName(), "Section6")
	result = ps.extractSection(param, "");
	TEST_EQUAL(result, false)
	TEST_EQUAL(ps.getSectionName(), "")
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


CHECK(const ParameterSection& ParameterSection::operator = (const ParameterSection& parameter_section))
  //BAUSTELLE
RESULT


CHECK(bool ParameterSection::operator == (const ParameterSection& parameter_section))
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
