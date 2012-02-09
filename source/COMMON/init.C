// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/COMMON/init.h>

#include <clocale>
#include <cstdlib>

namespace BALL
{
	GlobalInitializer* GlobalInitializer::init()
	{
		setlocale(LC_NUMERIC, "C");

#ifndef BALL_COMPILER_MSVC
		unsetenv("LC_ALL");
		setenv("LC_NUMERIC", "C", 1);
#endif

		static GlobalInitializer instance;
		return &instance;
	}

	GlobalInitializer::~GlobalInitializer()
	{
	}

	GlobalInitializer::GlobalInitializer()
	{
	}

	GlobalInitializer::GlobalInitializer(const GlobalInitializer& /*gi*/)
	{
	}

	GlobalInitializer* gi = GlobalInitializer::init();
}
