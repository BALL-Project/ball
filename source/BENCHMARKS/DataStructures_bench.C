// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DataStructures_bench.C,v 1.2 2004/12/13 13:14:57 oliver Exp $
//

#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/DATATYPE/hashMap.h>
#include <BALL/DATATYPE/hashSet.h>
#include <utility>
#include <stdlib.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(DataStructures, 11.0, "$Id: DataStructures_bench.C,v 1.2 2004/12/13 13:14:57 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

HashSet<Size> hashed_numbers;
Size MAX_NUMBERS = 500000;
while (hashed_numbers.size() < MAX_NUMBERS)
{
	hashed_numbers.insert(Size(random()));
}
std::vector<Size> numbers(MAX_NUMBERS);
std::copy(hashed_numbers.begin(), hashed_numbers.end(), numbers.begin());

START_SECTION(HashMap::insert, 1.0)
	HashMap<Size, Size> hm;
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			hm.insert(std::make_pair(numbers[i], i));
		STOP_TIMER
	}
END_SECTION

START_SECTION(HashMap::lookup, 1.0)
	Size s;
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			s = hm[numbers[i]];
		STOP_TIMER
	}
END_SECTION

/*
START_SECTION(map::insert, 0.0)
	__gnu_cxx::hash_map<Size, Size> m;
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			m.insert(std::make_pair(numbers[i], i));
		STOP_TIMER
	}
END_SECTION

START_SECTION(map::lookup, 0.0)
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			s = m[numbers[i]];
		STOP_TIMER
	}
END_SECTION
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
