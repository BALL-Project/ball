// $Id: Function_test.C,v 1.1 2000/12/06 18:59:23 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MATHS/function.h>

///////////////////////////

START_TEST(class_name, "$Id: Function_test.C,v 1.1 2000/12/06 18:59:23 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

ConstantFunction* constant_ptr;

CHECK(ConstantFunction::ConstantFunction())
	constant_ptr = new ConstantFunction;
	TEST_NOT_EQUAL(constant_ptr, 0)
RESULT

CHECK(ConstantFunction::~ConstantFunction())
	delete constant_ptr;
RESULT

CHECK(ConstantFunction::ConstantFunction(DataType constant))
	ConstantFunction cofu1(47.11);
	TEST_EQUAL(cofu1.getConstant(), 47.11)
RESULT

CHECK(ConstantFunction::ConstantFunction(const ConstantFunction& constant))
	ConstantFunction cofu1;
	ConstantFunction cofu2(cofu1);
	TEST_NOT_EQUAL(cofu2.getConstant(), 47.11)
	cofu1.setConstant(47.11);
	ConstantFunction cofu3(cofu1);
	TEST_EQUAL(cofu3.getConstant(), 47.11)
RESULT

CHECK(ConstantFunction::operator = (const ConstantFunction& constant))
	ConstantFunction cofu1;
	ConstantFunction cofu2 = cofu1;
	TEST_NOT_EQUAL(cofu2.getConstant(), 47.11)
	cofu1.setConstant(47.11);
	ConstantFunction cofu2 = cofu1;
	TEST_EQUAL(cofu2.getConstant(), 47.11)
RESULT

CHECK(ConstantFunction::clear())
	ConstantFunction cofu(8.15);
	ConstantFunction cofu2;
	bool test = (cofu == cofu2);
	TEST_EQUAL(test, false)
	cofu.clear();
	test = (cofu == cofu2);
	TEST_EQUAL(test, true)
RESULT

CHECH(ConstantFunction::operator == (const ConstantFunction& constant))
	ConstantFunction cofu(8.15);
	ConstantFunction cofu2;
	bool test = (cofu == cofu2);
	TEST_EQUAL(test, false)
	cofu.clear();
	test = (cofu == cofu2);
	TEST_EQUAL(test, true)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
