// $Id: SolventParameter_test.C,v 1.9 2001/08/22 09:26:36 aubertin Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SOLVATION/solventParameter.h>
///////////////////////////

START_TEST(SolventParameter, "$Id: SolventParameter_test.C,v 1.9 2001/08/22 09:26:36 aubertin Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SolventParameter* s_ptr;
SolventParameter* s_ptr2;

CHECK(SolventParameter())
	s_ptr = new SolventParameter;	
	TEST_NOT_EQUAL(s_ptr,0)
	bool test = ( "" == s_ptr->getSolventName());
	TEST_EQUAL(test,true)
	test = ( 0.0 == s_ptr->getSolventDensity());
	TEST_EQUAL(test,true)
  ::std::vector<SolventAtomDescriptor> test_sae;
	//test = ( test_sae == s_ptr->getSolventDescription()); 
  //TEST_EQUAL(test, true)
  SolventDescriptor test_sd;
	test = ( test_sd == s_ptr->getSolventDescriptor());
	TEST_EQUAL(test, true)
RESULT

CHECK(SolventParameter(const SolventParameter&))
	s_ptr2 = new SolventParameter(*s_ptr);
	TEST_NOT_EQUAL(s_ptr2,0)
	TEST_EQUAL(s_ptr,s_ptr2)
RESULT

CHECK(~SolventParameter())
	delete s_ptr;
RESULT

CHECK(extractSection(ForceFieldParameters& parameters, const String& section_name))
	//BAUSTELLE , da BAUSTELLE in SolventParameter.C
 	bool result;
	SolventParameter s_ptr;

	ForceFieldParameters param("data/SolventParameter_test1.ini");
	param.init();
	CAPTURE_OUTPUT(2000)	
	result = s_ptr.extractSection(param,"SolventDescription");
	COMPARE_OUTPUT("SolventParameter::extractSection(): Variable missing.\n")
	TEST_EQUAL(result,false)
	
	param = ForceFieldParameters("data/SolventParameter_test2.ini");
	param.init();
	CAPTURE_OUTPUT(2000)	
	result = s_ptr.extractSection(param,"SolventDescription");
	COMPARE_OUTPUT("SolventParameter::extractSection(): no name given.\n")
	TEST_EQUAL(result,true)
	
	param = ForceFieldParameters("data/SolventParameter_test3.ini");
	param.init();
	CAPTURE_OUTPUT(2000)	
	result = s_ptr.extractSection(param,"SolventDescription");
	COMPARE_OUTPUT("SolventParameter::extractSection(): no number density given.\n")
	TEST_EQUAL(result,true)

	param = ForceFieldParameters("data/SolventParameter_test4.ini");
	param.init();
	CAPTURE_OUTPUT(2000)	
	result = s_ptr.extractSection(param,"SolventDescription");
	COMPARE_OUTPUT("SolventParameter::extractSection(): Cannot assign atom type.\n")
	TEST_EQUAL(result,true)
	
	param = ForceFieldParameters("data/SolventParameter_test.ini");
	param.init();
	result = s_ptr.extractSection(param,"SolventDescription");
	TEST_EQUAL(result,true)
RESULT


CHECK(clear())	
	SolventParameter s_ptr;
	ForceFieldParameters param("data/SolventParameter_test.ini");
	param.init();
	s_ptr.extractSection(param,"SolventDescription");
	s_ptr.clear();
	SolventParameter s_ptr2;
	//assumes that operator == is correct
	bool result = (s_ptr==s_ptr2);
	TEST_EQUAL(result,true)
RESULT


CHECK(SolventParameter& operator = (const SolventParameter& param))
	SolventParameter s_ptr;
	ForceFieldParameters param("data/SolventParameter_test.ini");
	param.init();
	s_ptr.extractSection(param,"SolventDescription");
	SolventParameter s_ptr2;
	s_ptr2=s_ptr;
	//assumes that operator == is correct
	bool result = (s_ptr==s_ptr2);
	TEST_EQUAL(result,true)
RESULT


CHECK(SolventParameter(const SolventParameter& param))
	SolventParameter s_ptr;
	ForceFieldParameters param("data/SolventParameter_test.ini");
	param.init();
	s_ptr.extractSection(param,"SolventDescription");
	SolventParameter s_ptr2;
	s_ptr2 = SolventParameter(s_ptr);
	//assumes that operator == is correct
	bool result = (s_ptr==s_ptr2);
	TEST_EQUAL(result,true)
RESULT


CHECK(getSolventDescriptor() const )
	SolventParameter s_ptr;
	ForceFieldParameters param("data/SolventParameter_test.ini");
	param.init();
	s_ptr.extractSection(param,"SolventDescription");
	const SolventDescriptor sd = s_ptr.getSolventDescriptor();
	TEST_EQUAL(sd.getName().getSubstring(0,3),"PCM")
RESULT


CHECK(getSolventDescriptor() )
	SolventParameter s_ptr;
	ForceFieldParameters param("data/SolventParameter_test.ini");
	param.init();
	s_ptr.extractSection(param,"SolventDescription");
	SolventDescriptor sd = s_ptr.getSolventDescriptor();
	TEST_EQUAL(sd.getName().getSubstring(0,3),"PCM")
RESULT


CHECK(operator == (const SolventParameter& param) const)
	//BAUSTELLE , da Baustelle in Quellcode
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
