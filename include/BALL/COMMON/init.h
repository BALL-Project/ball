// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_INIT_H
#define BALL_COMMON_INIT_H

#ifndef BALL_COMMON_GLOBAL_H
#include <BALL/COMMON/global.h>
#endif

namespace BALL
{
	/** General initialization class for the BALL library.
			The sole purpose of this class is to provide a safe place
			for intializations that should occur before the first BALL
			functions are called. Please note that this is realized by
			a static singleton, so there is <b>no</b> guarantee of a 
			particular order of the creation of this class with respect
			to all other static variables.
	*/
	class BALL_EXPORT GlobalInitializer
	{
		public:
			~GlobalInitializer();
			static GlobalInitializer* init();

		private:
			GlobalInitializer();
			GlobalInitializer(const GlobalInitializer& gi);
	};
}

#endif // BALL_COMMON_INIT_H
