// $Id: factory.h,v 1.1.2.1 2002/11/30 15:57:58 oliver Exp $

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
	class Factory
	{
		public:
		
		/// Return a pointer to a new instance of T
		static T* create() { return new T; }
		/// Return a void pointer to a new instance of T
		static void* createVoid() { return (void*)new T; }
		/// Return a reference to a (pre-instantiated) default object
		static const T& getDefault() { return default_; }
		protected:
		///
		static T default_;
	};


}

#endif // BALL_CONCEPT_FACTORY_H
