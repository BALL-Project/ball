// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: callback.h,v 1.8 2003/03/14 11:48:46 sturm Exp $

#ifndef BALL_CONCEPT_CALLBACK_H
#define BALL_CONCEPT_CALLBACK_H

#ifndef BALL_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

namespace BALL 
{

	/**	Callback class.
			
			 \addtogroup  Concepts
	*/
	class Callback
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		Callback()
			throw()
		{
		}

		/**	Destructor
		*/
		virtual ~Callback()
		  throw()
		{
		}
		//@}

		/**
		*/
		virtual bool operator ()()

		{
			return true;
		}
	};

} // namespace BALL

#endif // BALL_CONCEPT_CALLBACK_H 
