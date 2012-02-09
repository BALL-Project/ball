// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DataStructures_bench.C,v 1.4 2006/06/08 09:26:30 oliver Exp $
//
// Author:
//  Andreas Moll
//

#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/DATATYPE/hashMap.h>
#include <BALL/DATATYPE/hashSet.h>
#include <utility>
#include <algorithm>
#include <map>
// #include <ext/hash_map>

#include <cstdlib> // rand



///////////////////////////

using namespace BALL;

// Random number generator function
Size randomGen()
{
	return (Size)rand();
}

START_BENCHMARK(DataStructures, 11.0, "$Id: DataStructures_bench.C,v 1.4 2006/06/08 09:26:30 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

const Size MAX_NUMBERS = 500000;
std::vector<Size> numbers(MAX_NUMBERS);
std::generate(numbers.begin(), numbers.end(), randomGen);

START_SECTION(HashMap::insert(std::pair<key_type, value_type>), 1.0)
	HashMap<Size, Size> hm;
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			hm.insert(std::make_pair(numbers[i], i));
		STOP_TIMER
	}
END_SECTION

START_SECTION(HashMap::operator [] (key_type), 1.0)
	Size s;
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			s = hm[numbers[i]];
		STOP_TIMER
	}
END_SECTION


START_SECTION(map::insert(std::pair<key_type, value_type>), 0.0)
	std::map<Size, Size> m;
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			m.insert(std::make_pair(numbers[i], i));
		STOP_TIMER
	}
END_SECTION

START_SECTION(map::operator [] (key_type), 0.0)
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			s = m[numbers[i]];
		STOP_TIMER
	}
END_SECTION

/*
START_SECTION(hash_map::insert, 0.0)
	__gnu_cxx::hash_map<Size, Size> ghm;
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			ghm.insert(std::make_pair(numbers[i], i));
		STOP_TIMER
	}
END_SECTION

START_SECTION(hash_map::lookup, 0.0)
	for (Position i = 0; i < numbers.size(); ++i)
	{
		START_TIMER
			s = ghm[numbers[i]];
		STOP_TIMER
	}
END_SECTION
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
