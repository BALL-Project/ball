// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/randomNumberGenerator.h>

///////////////////////////

START_TEST(RandomNumberGenerator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
CHECK(RandomNumberGenerator::RandomNumberGenerator())
	RandomNumberGenerator* rng_ptr = new RandomNumberGenerator();
	TEST_NOT_EQUAL(rng_ptr,0)
	TEST_EQUAL(rng_ptr->isValid(), true)
RESULT

CHECK(RandomNumberGenerator::RandomNumberGenerator(const RandomNumberGenerator& rng))
	RandomNumberGenerator rng1;
	RandomNumberGenerator rng2(rng1);
	// ???
	// This test needs accessors. Later.
	// TEST_EQUAL(rng1 == rng2, true)
RESULT

CHECK(RandomNumberGenerator::~RandomNumberGenerator())
	RandomNumberGenerator* rng_ptr = new RandomNumberGenerator();
	delete rng_ptr;
RESULT

CHECK(void RandomNumberGenerator::setup())
	// ???
	// This test needs accessors. Later.
RESULT

CHECK(double RandomNumberGenerator::randomUniform())
	RandomNumberGenerator rng;
	rng.setup(1802, 9373);

	// Generate 20000 random numbers
	double r;
  for (int i=0; i < 20000; i++)
	{
		r = rng.randomUniform();
	}

	/*
		If the random number generator is working properly, 
		the next six random numbers should be:
		6533892.0  14220222.0  7275067.0
		6172232.0  8354498.0   10633180.0
	*/

	std::vector<double> test_numbers;
	test_numbers.push_back(6533892.0);
	test_numbers.push_back(14220222.0);
	test_numbers.push_back(7275067.0);
	test_numbers.push_back(6172232.0);
	test_numbers.push_back(8354498.0);
	test_numbers.push_back(10633180.0);


	for (int i=0; i<6; i++)
	{
		double number = rng.randomUniform() * 4096.0 * 4096.0;
		TEST_EQUAL(number, test_numbers[i])
	}
RESULT

CHECK(RandomNumberGenerator::randomGaussian())
	RandomNumberGenerator rng;
	rng.setup(1802,9373);

	double r;
	double sum = 0.0;
	double sum2 = 0.0;

	double MEAN = 1.0;
	double STDDEV = 0.5;
	Size numbers = 20000;

	for (Size i=0; i < numbers; i++) 
	{
		r = rng.randomGaussian(1.0, 0.5);
		sum += r;
		sum2 += (r*r);
	}

	double mean = sum / numbers;
	double stddev = sqrt((sum2 - sum * sum / numbers) / numbers);

	PRECISION(0.01)
	TEST_REAL_EQUAL(mean, MEAN)
	TEST_REAL_EQUAL(stddev, STDDEV)
RESULT

CHECK(RandomNumberGenerator::randomInteger())
	// ???
RESULT

CHECK(RandomNumberGenerator::randomDouble())
	// ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
