// $Id: processor.h,v 1.5 2000/02/17 09:47:12 oliver Exp $

#ifndef BALL_CONCEPT_PROCESSOR_H
#define BALL_CONCEPT_PROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BASECONCEPT_H
#	include <BALL/CONCEPT/baseFunctor.h>
#endif

namespace BALL 
{

	/**	@name Processor Concept
			{\bf Definition:} \URL{BALL/CONCEPT/processor.h}
	*/
	//@{

	/**	Global Types and Enums for Processors
	*/
  class Processor
  {
		public:
		/**	Result type
		*/
    typedef int Result;

		/**	Result codes
		*/
    enum
    {
			/**	Abort 
			*/
      ABORT    = 0,
			/**	Break 
			*/
      BREAK    = 1,
			/**	Continue 
			*/
      CONTINUE = 2
		};
	};
 

	/**	Generic Unary Processor Class
	*/
	template <typename T>
	class UnaryProcessor
		: public UnaryFunctor<T, Processor::Result>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		/**	Default constructor
		*/
		UnaryProcessor()
		{
		}

		/**	Copy constructor
		*/
		UnaryProcessor
			(const UnaryProcessor& /* processor */, bool /* deep */ = true)
		{
		}

		/**	Destructor
		*/
		virtual ~UnaryProcessor()
		{
		}
		//@}

		/**	@name Processor specific methods
		*/
		//@{
		/** start method
		*/
		virtual bool start()
		{
			return true;
		}

		/** finish method
		*/
		virtual bool finish()
		{
			return true;
		}

		/**	operator ()
		*/
		virtual Processor::Result operator () (T &)
		{
			return Processor::CONTINUE;
		}
		//@}
	};

	/**	Generic Binary Processor Class
	*/
	template <typename T1, typename T2>
	class BinaryProcessor
		: public BinaryFunctor<T1, T2, Processor::Result>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		/**	Default constructor
		*/
		BinaryProcessor()	
		{
		}

		/**	Copy constructor
		*/
		BinaryProcessor	
			(const BinaryProcessor& /* processor */, bool /* deep */ = true)
		{
		}

		/**	Destructor
		*/
		virtual ~BinaryProcessor()
		{
		}
		//@}

		/**	@name	Processor-specific methods
		*/
		//@{
		
		/**	start method
		*/
		virtual bool start()
		{
			return true;
		}

		/**	finish method
		*/
		virtual bool finish()
		{
			return true;
		}

		/**	operator ()
		*/
		virtual Processor::Result operator () (T1&, T2&)
		{
			return Processor::CONTINUE;
		}
		//@}
	};

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_PROCESSOR_H
