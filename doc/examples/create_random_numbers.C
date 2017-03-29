// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A small example application for creating random numbers.

#include <BALL/DATATYPE/string.h>
#include <BALL/CONCEPT/timeStamp.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>

using namespace BALL;

int main(int argc, char** argv)
{
	Size numbers;

	if (argc != 2)
	{
		numbers = 20;
		std::cerr << "Creating 20 random numbers. Call me with the number of random numbers you want." 
			<< std::endl;
	}
	else
	{
		String argument(argv[1]);
		numbers = argument.toInt();
	}
	
	std::cout << "******************************" << std::endl;

	double r, rmin=1e32, rmax=-1e32;

	boost::mt19937 rng(PreciseTime::now().getMicroSeconds());
	boost::uniform_01<double> unif;

	for (Size i=0; i < numbers; i++)
	{
		r = unif(rng);
		if (r < rmin) rmin = r;
		if (r > rmax) rmax = r;
		std::cout << r << std::endl;
	}
	std::cout << "******************************" << std::endl;
	std::cout << "min: " << rmin << "  max: " << rmax << std::endl; 

}
