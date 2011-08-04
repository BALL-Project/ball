// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/numericalIntegrator.h>

///////////////////////////

START_TEST(NumericalIntegrator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

NumericalIntegrator<MutableConstant<>, float>* ni_ptr = 0;
CHECK(NumericalIntegrator() throw())
	ni_ptr = new NumericalIntegrator<MutableConstant<>, float>;
	TEST_NOT_EQUAL(ni_ptr, 0)
RESULT


CHECK(~NumericalIntegrator() throw())
	delete ni_ptr;
RESULT

CHECK(BALL_CREATE(NumericalIntegrator))
  // ???
RESULT

CHECK(NumericalIntegrator(const NumericalIntegrator& nint) throw())
	float test_constant = 784.83630;
	NumericalIntegrator<MutableConstant<>, float> ni1;
	ni1.getFunction().setConstant(test_constant);
	NumericalIntegrator<MutableConstant<>, float> ni2;
	bool test = (ni1 == ni2);
	TEST_NOT_EQUAL(test, true)

	NumericalIntegrator<MutableConstant<>, float> ni3(ni1);
	test = (ni1 == ni3);
	TEST_EQUAL(test, true)
RESULT


CHECK(NumericalIntegrator& operator = (const NumericalIntegrator& nint) throw())
	NumericalIntegrator<MutableConstant<>, float> ni1;
	float test_constant = 784.83630;
	ni1.getFunction().setConstant(test_constant);

	NumericalIntegrator<MutableConstant<>, float> ni2;
	bool test = (ni1 == ni2);
	TEST_NOT_EQUAL(test, true)

	ni2 = ni1;
	test = (ni1 == ni2);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool operator == (const NumericalIntegrator& nint) const throw())
	NumericalIntegrator<MutableConstant<>, float> ni1;
	float test_constant = 9264.97840;
	ni1.getFunction().setConstant(test_constant);
	NumericalIntegrator<MutableConstant<>, float> ni2;
	bool test = (ni1 == ni2);
	TEST_NOT_EQUAL(test, true)

	ni2.getFunction().setConstant(test_constant);
	test = (ni1 == ni2);
	TEST_EQUAL(test, true)
RESULT


CHECK(void setFunction(const Function& function) throw())
	Product<float, float> test_function;
	float test_constant1 = 3246.2983742;
	float test_constant2 = 8752373.0;
	test_function.getFirst() = test_constant1;
	test_function.getSecond() = test_constant2;
	NumericalIntegrator<Product<float, float>, float> ni1;
	ni1.setFunction(test_function);

	NumericalIntegrator<Product<float, float>, float> ni2;
	bool test = (ni1.getFunction() == ni2.getFunction());
	TEST_NOT_EQUAL(test, true)

	ni2.getFunction().getFirst() = test_constant1;
	ni2.getFunction().getSecond() = test_constant2;
	test = (ni1.getFunction() == ni2.getFunction());
	TEST_EQUAL(test, true)
RESULT


CHECK(const Function& getFunction() const throw())
	Product<float, float> test_function;
	float test_constant1 = 3246.2983742;
	float test_constant2 = 8752373.0;
	test_function.getFirst() = test_constant1;
	test_function.getSecond() = test_constant2;
	NumericalIntegrator<Product<float, float>, float> ni1;
	ni1.setFunction(test_function);

	NumericalIntegrator<Product<float, float>, float> ni2;
	bool test = (ni1.getFunction() == ni2.getFunction());
	TEST_NOT_EQUAL(test, true)

	ni2.getFunction().getFirst() = test_constant1;
	ni2.getFunction().getSecond() = test_constant2;
	test = (ni1.getFunction() == ni2.getFunction());
	TEST_EQUAL(test, true)
RESULT


CHECK(DataType getValue(const DataType& x) const throw())
	Product<MutableConstant<>, MutableConstant<> > test_function;
	float test_constant1 = 8375.38723;
	float test_constant2 = 0.7364823;
	test_function.getFirst() = test_constant1;
	test_function.getSecond() = test_constant2;
	NumericalIntegrator<Product<MutableConstant<>, MutableConstant<> >, float> ni1;
	ni1.setFunction(test_function);

	float bla = 453.39874;
	float result = ni1.getValue(bla);
	PRECISION(0.01)
	TEST_REAL_EQUAL(result, 6168.324451)
RESULT


CHECK(DataType integrate(const DataType& from, const DataType& to) const throw())
	MutableConstant<> test_function;
	float test_constant = 74.816409;
	test_function.setConstant(test_constant);
	NumericalIntegrator<MutableConstant<> > ni;
	ni.setFunction(test_function);
	float result = ni.integrate(0.0, 1.0);
	TEST_REAL_EQUAL(result, test_constant)
	result = ni.integrate(0.0, 2.0);
	TEST_REAL_EQUAL(result, 2.0 * test_constant)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
