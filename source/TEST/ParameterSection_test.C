// $Id: ParameterSection_test.C,v 1.8 2001/04/09 23:02:52 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/parameterSection.h>
#include <BALL/FORMAT/parameters.h>

///////////////////////////

START_TEST(Parameters, "$Id: ParameterSection_test.C,v 1.8 2001/04/09 23:02:52 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Parameters param("data/ParameterSection_test.ini");
param.init();

{
ParameterSection* ps = 0;
CHECK(ParameterSection::ParameterSection())
	ps = new ParameterSection;
	TEST_NOT_EQUAL(ps, 0)
RESULT


CHECK(ParameterSection::~ParameterSection())
	delete ps;
RESULT
}

CHECK(ParameterSection::clear())
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
	TEST_EQUAL(result, false)
	TEST_EQUAL(ps.getSectionName(), "Section4")

	result = ps.extractSection(param, "Section5");
	TEST_EQUAL(result, false)
	TEST_EQUAL(ps.getSectionName(), "Section5")

	result = ps.extractSection(param, "Section6");
	TEST_EQUAL(result, false)
	TEST_EQUAL(ps.getSectionName(), "Section6")

	result = ps.extractSection(param, "");
	TEST_EQUAL(result, false)
	TEST_EQUAL(ps.getSectionName(), "")
RESULT

ParameterSection ps;
ps.extractSection(param, "Section2");


CHECK(ParameterSection::getValue(const String& key, const String& variable) const )

	TEST_EQUAL(ps.getValue("A", "val"), "B")
	TEST_EQUAL(ps.getValue("C", "val"), "C")
	TEST_EQUAL(ps.getValue("D", "val"), "E")
	TEST_EQUAL(ps.getValue("F", "val"), "G")
	TEST_EQUAL(ps.getValue("J", "val"), " K")
	TEST_EQUAL(ps.getValue(" L", "val"), " M")
	TEST_EQUAL(ps.getValue("N", "val"), "O")
	TEST_EQUAL(ps.getValue("X", "val"), ParameterSection::UNDEFINED)
RESULT


CHECK(ParameterSection::has(const String& key, const String& variable) const )
	ParameterSection ps;
	ps.extractSection(param, "Section2");

	TEST_EQUAL(ps.has("A", "val"), true)
	TEST_EQUAL(ps.has(" L", "val"), true)
	TEST_EQUAL(ps.has("N", "val"), true)
	TEST_EQUAL(ps.has("X", "val"), false)
	TEST_EQUAL(ps.has("N", "X"), false)
RESULT


CHECK(ParameterSection::has(const String& key) const )
	TEST_EQUAL(ps.has("A"), true)
	TEST_EQUAL(ps.has(" L"), true)
	TEST_EQUAL(ps.has("X"), false)

RESULT


CHECK(ParameterSection::hasVariable(const String& variable) const )
	TEST_EQUAL(ps.hasVariable("val"), true)
	TEST_EQUAL(ps.hasVariable("x"), false)
RESULT


CHECK(ParameterSection::getColumnIndex(const String& variable) const )
	TEST_EQUAL(ps.getColumnIndex("val"), 0)
	TEST_EQUAL(ps.getColumnIndex("X"), INVALID_POSITION)
RESULT


CHECK(ParameterSection::getNumberOfVariables() const )
	TEST_EQUAL(ps.getNumberOfVariables(), 1)
RESULT


CHECK(ParameterSection::getNumberOfKeys() const )
	TEST_EQUAL(ps.getNumberOfKeys(), 7)
RESULT


CHECK(ParameterSection::getValue(Position key_index, Position variable_index) const )
	TEST_EQUAL(ps.getValue(2, 0), "E")
	TEST_EQUAL(ps.getValue(2, 1), ParameterSection::UNDEFINED)
	TEST_EQUAL(ps.getValue(20, 1), ParameterSection::UNDEFINED)
	TEST_EQUAL(ps.getValue(20, -1), ParameterSection::UNDEFINED)
RESULT


CHECK(ParameterSection::getKey(Position key_index) const )
	TEST_EQUAL(ps.getKey(2), "D")
RESULT


CHECK(ParameterSection::isValid() const )
	TEST_EQUAL(ps.isValid(), true)
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
