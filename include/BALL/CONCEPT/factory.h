// $Id: factory.h,v 1.1.2.2 2002/11/30 16:27:21 oliver Exp $

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
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/factory.h}
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
			static T def; 
			return def; 
		}
	};


}

#endif // BALL_CONCEPT_FACTORY_H
