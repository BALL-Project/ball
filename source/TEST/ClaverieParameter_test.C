// $Id: ClaverieParameter_test.C,v 1.2 2001/07/15 15:17:13 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/claverieParameter.h>

///////////////////////////

START_TEST(class_name, "$Id: ClaverieParameter_test.C,v 1.2 2001/07/15 15:17:13 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class ClaverieParameter::

ClaverieParameter* cp_ptr = 0;

CHECK(ClaverieParameter::ClaverieParameter())
	cp_ptr = new ClaverieParameter;
	TEST_NOT_EQUAL(cp_ptr, 0)
	::std::vector< ::std::pair<float, float> > test_params;
	bool test = (test_params == cp_ptr->getParameters());
	TEST_EQUAL(test, true)
	HashMap<Atom::Type, Index> test_indices;
	test = (test_indices == cp_ptr->getIndices());
	TEST_EQUAL(test, true)
RESULT


CHECK(ClaverieParameter::(const ClaverieParameter& param)())
	ClaverieParameter cp1;
	ForceFieldParameters fffparam("data/ClaverieParameter_test.ini");
	cp1.extractSection(fffparam, "ClaverieParameters");
	ClaverieParameter cp2;
	bool test = (cp1 == cp2);
	TEST_NOT_EQUAL(test, true)

	ClaverieParameter cp3(cp1);
	test = (cp1 == cp3);
	TEST_EQUAL(test, true)
RESULT


CHECK(ClaverieParameter::clear())
	ClaverieParameter empty;
	ClaverieParameter nonempty;
	ForceFieldParameters fffparam("data/ClaverieParameter_test.ini");
	nonempty.extractSection(fffparam, "ClaverieParameters");
	bool test = (empty == nonempty);
	TEST_NOT_EQUAL(test, true)

	nonempty.clear();
	test = (empty == nonempty);
	TEST_EQUAL(test, true)
RESULT


CHECK(ClaverieParameter::ClaverieParameter& operator = (const ClaverieParameter& param))
	ClaverieParameter cp1;
	ForceFieldParameters fffparam("data/ClaverieParameter_test.ini");
	cp1.extractSection(fffparam, "ClaverieParameters");
	ClaverieParameter cp2;
	bool test = (cp1 == cp2);
	TEST_NOT_EQUAL(test, true)

	cp2 = cp1;
	test = (cp1 == cp2);
	TEST_EQUAL(test, true)
RESULT


CHECK(ClaverieParameter::hasParameters(Atom::Type solvent_type, Atom::Type solute_type) const )
	ClaverieParameter cp;
	ForceFieldParameters fffparam("data/ClaverieParameter_test.ini");
	cp.extractSection(fffparam, "ClaverieParameters");

	Atom::Type type_HW = fffparam.getAtomTypes().getType("HW");
	Atom::Type type_OW = fffparam.getAtomTypes().getType("OW");
	Atom::Type type_CT = fffparam.getAtomTypes().getType("CT");
	Atom::Type type_HC = fffparam.getAtomTypes().getType("HC");

	bool test = cp.hasParameters(type_HW, type_CT);
	TEST_EQUAL(test, true)

	test = cp.hasParameters(type_HC, type_OW);
	TEST_EQUAL(test, true)
RESULT


CHECK(ClaverieParameter::getParameters(Atom::Type solvent_type, Atom::Type solute_type) const )
	ClaverieParameter cp;
	ForceFieldParameters fffparam("data/ClaverieParameter_test.ini");
	cp.extractSection(fffparam, "ClaverieParameters");

	Atom::Type type_HW = fffparam.getAtomTypes().getType("HW");
	Atom::Type type_OW = fffparam.getAtomTypes().getType("OW");
	Atom::Type type_CT = fffparam.getAtomTypes().getType("CT");
	Atom::Type type_HC = fffparam.getAtomTypes().getType("HC");

	::std::pair<float, float> params;
	
	params = cp.getParameters(type_HW, type_CT);
	PRECISION(0.0001)
	TEST_REAL_EQUAL(params.first, 1.0)
	TEST_REAL_EQUAL(params.second, 2.85657)
RESULT


CHECK(ClaverieParameter::extractSection(ForceFieldParameters& parameters, const String& section_name))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
