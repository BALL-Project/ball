// $Id: NumericalIntegrator_test.C,v 1.4 2001/07/15 17:32:40 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MATHS/numericalIntegrator.h>

///////////////////////////

START_TEST(NumericalIntegrator, "$Id: NumericalIntegrator_test.C,v 1.4 2001/07/15 17:32:40 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

// tests for class NumericalIntegrator::


NumericalIntegrator<MutableConstant<>, float>* ni_ptr = 0;

CHECK(NumericalIntegrator::NumericalIntegrator() throw())
	ni_ptr = new NumericalIntegrator<MutableConstant<>, float>;
	TEST_NOT_EQUAL(ni_ptr, 0)
RESULT


CHECK(NumericalIntegrator::~NumericalIntegrator() throw())
	delete ni_ptr;
RESULT


CHECK(NumericalIntegrator::NumericalIntegrator(const NumericalIntegrator& nint) throw())
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


CHECK(NumericalIntegrator::NumericalIntegrator& operator = (const NumericalIntegrator& nint) throw())
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


CHECK(NumericalIntegrator::clear() throw())
	// BAUSTELLE:
	// see <BALL/MATHS/numericalIntegrator.h>
RESULT


CHECK(NumericalIntegrator::bool operator == (const NumericalIntegrator& nint) const  throw())
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


CHECK(NumericalIntegrator::setFunction(const Function& function) throw())
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


CHECK(NumericalIntegrator::getFunction() const  throw())
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


CHECK(NumericalIntegrator::getValue(DataType x))
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


CHECK(NumericalIntegrator::integrate(from, to))
	MutableConstant<> test_function;
	float test_constant = 74.816409;
	test_function.setConstant(test_constant);
	NumericalIntegrator<MutableConstant<> > ni;
	ni.setFunction(test_function);
	float result = ni.integrate(0.0, 1.0);
	TEST_REAL_EQUAL(result, test_constant)
	result = ni.integrate(0.0, 2.0);
	TEST_REAL_EQUAL(result, 2.0 * test_constant)
	// BAUSTELLE:
	// a more complicated function
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
