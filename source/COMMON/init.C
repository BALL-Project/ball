// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/COMMON/init.h>

#include <stdlib.h>

namespace BALL
{
	GlobalInitializer* GlobalInitializer::init()
	{
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
