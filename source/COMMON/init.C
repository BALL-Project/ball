// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/COMMON/init.h>

#include <locale.h>
#include <stdlib.h>

namespace BALL
{
	GlobalInitializer* GlobalInitializer::init()
	{
		setlocale(LC_NUMERIC, "C");
		unsetenv("LC_ALL");
		setenv("LC_NUMERIC", "C", 1);

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
