// $Id: processor.h,v 1.2 1999/12/04 18:34:11 oliver Exp $

#ifndef BALL_CONCEPT_PROCESSOR_H
#define BALL_CONCEPT_PROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BASECONCEPT_H
#	include <BALL/CONCEPT/baseFunctor.h>
#endif

namespace BALL {

	/**	@name Processor Concept
			{\bf Definition:} \URL{BALL/CONCEPT/processor.h}
	*/
	//@{

	/**	Global Types and Enums for Processors
	*/
  struct Processor
  {
		/**	Resutl type
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
	template <class T>
	class UnaryProcessor
		: public UnaryFunctor<T, Processor::Result>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	
		*/
		UnaryProcessor()
		{
		}

		/**	
		*/
		UnaryProcessor(const UnaryProcessor& , bool /* deep */ = true)
		{
		}

		/**	
		*/
		virtual ~UnaryProcessor()
		{
		}
		//@}

		virtual bool start()
		{
			return true;
		}

		virtual bool finish()
		{ 
			return true;
		}

		virtual Processor::Result operator ()(T &);
	};

	
	template <class T>
	BALL_INLINE Processor::Result
	UnaryProcessor<T>::operator ()(T &)
	{
		return Processor::ABORT;
	}


	/**	Generic Binary Processor Class
	*/
	template <class T1, class T2>
	class BinaryProcessor
		: public BinaryFunctor<T1, T2, Processor::Result>
	{
		public:

		/**	@name	Constructors and Destructors	
		*/	
		//@{

		/**
		*/
		BinaryProcessor()
		{
		}

		/**
		*/
		BinaryProcessor(const BinaryProcessor &, bool /* deep */ = true)
		{
		}

		/**
		*/
		virtual ~BinaryProcessor()
		{
		}
		//@}

		virtual Processor::Result operator ()(T1&, T2&);

		virtual bool start()
		{
			return true;
		}

		virtual bool finish()
		{ 
			return true;
		}
	};

	
	template <class T1, class T2>
	BALL_INLINE 
	Processor::Result BinaryProcessor<T1, T2>::operator () (T1&, T2&)
	{
		return Processor::ABORT;
	}

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_PROCESSOR_H
