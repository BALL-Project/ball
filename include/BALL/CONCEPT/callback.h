// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: callback.h,v 1.6 2003/02/25 10:47:45 anhi Exp $

#ifndef BALL_CONCEPT_CALLBACK_H
#define BALL_CONCEPT_CALLBACK_H

#ifndef BALL_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

namespace BALL 
{

	/**	Callback class.
			<b>Definition:</b> BALL/CONCEPT/callback.h
			\ingroup Concepts
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
