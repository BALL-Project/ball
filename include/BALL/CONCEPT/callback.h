// $Id: callback.h,v 1.1 1999/08/26 07:53:16 oliver Exp $

#ifndef BALL_CONCEPT_CALLBACK_H
#define BALL_CONCEPT_CALLBACK_H

namespace BALL {

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
