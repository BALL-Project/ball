// $Id: callback.h,v 1.3 2000/12/19 12:50:48 amoll Exp $

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
