// $Id: callback.h,v 1.2 1999/12/04 14:12:24 oliver Exp $

#ifndef BALL_CONCEPT_CALLBACK_H
#define BALL_CONCEPT_CALLBACK_H

#ifndef BALL_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

namespace BALL 
{

	/**	Callback class.
			{\bf Definition:} \URL{BALL/CONCEPT/callback.h}
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
		{
		}

		/**	Destructor
		*/
		virtual ~Callback()
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
