// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2 sw=2
//
// $Id: randomNumberGenerator.h,v 1.1.18.1 2007/03/25 21:23:46 oliver Exp $
//

#ifndef BALL_MATHS_RANDOMNUMBERGENERATOR_H
#define BALL_MATHS_RANDOMNUMBERGENERATOR_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include<vector>

namespace BALL
{

	/** The following code is taken (and adapted to BALL's C++ interface)
		from the code provided at
		
		 	http://astronomy.swin.edu.au/~pbourke/other/random/

		The following comment stems from the original C source code:

		This Random Number Generator is based on the algorithm in a FORTRAN
		version published by George Marsaglia and Arif Zaman, Florida State
		University; ref.: see original comments below. 
		At the fhw (Fachhochschule Wiesbaden, W.Germany), Dept. of Computer
		Science, we have written sources in further languages (C, Modula-2
		Turbo-Pascal(3.0, 5.0), Basic and Ada) to get exactly the same test
		results compared with the original FORTRAN version. 
		pril 1989
		Karl-L. Noell <NOELL@DWIFH1.BITNET>
    		and  Helmut  Weber <WEBER@DWIFH1.BITNET>

		This random number generator originally appeared in "Toward a Universal
		Random Number Generator" by George Marsaglia and Arif Zaman.
		Florida State University Report: FSU-SCRI-87-50 (1987)
		It was later modified by F. James and published in "A Review of Pseudo-
		random Number Generators"
		THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.
		(However, a newly discovered technique can yield
		a period of 10^600. But that is still in the development stage.)
		It passes ALL of the tests for random number generators and has a period
		of 2^144, is completely portable (gives bit identical results on all
		machines with at least 24-bit mantissas in the floating point
		representation).
		The algorithm is a combination of a Fibonacci sequence (with lags of 97
		and 33, and operation "subtraction plus one, modulo one") and an
		"arithmetic sequence" (using subtraction).
		
		Use IJ = 1802 & KL = 9373 to test the random number generator. The
		subroutine RANMAR should be used to generate 20000 random numbers.
		Then display the next six random numbers generated multiplied by *4096
		If the random number generator is working properly, the random numbers
		should be:
			6533892.0  14220222.0  7275067.0
			6172232.0  8354498.0   10633180.0

	*/

	class BALL_EXPORT BALL_DEPRECATED RandomNumberGenerator
	{

		public:

			/// 
			RandomNumberGenerator();

			///
			RandomNumberGenerator(const RandomNumberGenerator& rng);

			///
			~RandomNumberGenerator();

			void setup(int ij = 1802, int kl = 9373);

			/**
				This is the random number generator proposed by George Marsaglia in
				Florida State University Report: FSU-SCRI-87-50
			*/
			double randomUniform();

			/** Random number generator from
				ALGORITHM 712, COLLECTED ALGORITHMS FROM ACM. THIS WORK PUBLISHED
				IN TRANSACTIONS ON MATHEMATICAL SOFTWARE, VOL. 18, NO. 4, DECEMBER,
				1992, PP. 434-435. The function returns a normally distributed
				pseudo-random number with a given mean and standard deviation.
				Calls are made to a function subprogram which must return
				independent random numbers uniform in the interval (0,1). The
				algorithm uses the ratio of uniforms method of A.J. Kinderman and
				J.F. Monahan augmented with quadratic bounding curves.
			*/
			double randomGaussian(double mean, double stddev);

			/** Generate a random integer in the interval [lower, upper]
			*/
			int randomInteger(int lower, int upper);

			/** Generate a random double in the interval (lower, upper)
			*/
			double randomDouble(double lower, double upper);

			///
			bool isValid() const
				
			{
				return(valid_);
			}

		private:

			//_
			std::vector<double> u_;

			//_
			double c_;

			//_
			double cd_;

			//_
			double cm_;

			//_
			int i97_;

			//_
			int j97_;

			//_ 
			bool valid_;


	};
}

#endif //  BALL_MATHS_RANDOMNUMBERGENERATOR_H

