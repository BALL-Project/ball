// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_FACTORY_H
#define BALL_CONCEPT_FACTORY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif


namespace BALL
{

	/** Factory class.
			The factory class is used to create objects of the type it 
			was parameterized with.
			 \par
			
	 	 \ingroup ConceptsMiscellaneous
	*/
	template <typename T>
	class Factory
	{
		public:
		
		/// Return a pointer to a new instance of T
		static T* create() { return new T; }

		/// Return a void pointer to a new instance of T
		static void* createVoid() { return (void*)new T; }

		/// Return a reference to a (pre-instantiated) default object
		static const T& getDefault() 
		{ 
			static T default_instance;
			return default_instance;
		}
	};
}

#endif // BALL_CONCEPT_FACTORY_H
