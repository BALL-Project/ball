// $Id: processor.h,v 1.4 2000/02/16 19:13:06 oliver Exp $

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
		UnaryProcessor();

		/**	Copy constructor
		*/
		UnaryProcessor(const UnaryProcessor& processor, bool deep = true);

		/**	Destructor
		*/
		virtual ~UnaryProcessor();
		//@}

		/**	@name Processor specific methods
		*/
		//@{
		/** start method
		*/
		virtual bool start();

		/** finish method
		*/
		virtual bool finish();

		/**	operator ()
		*/
		virtual Processor::Result operator () (T &);
		//@}
	};
			
	template <typename T>
	BALL_INLINE 
	UnaryProcessor<T>::UnaryProcessor()
	{
	}
				
	template <typename T>
	BALL_INLINE 
	UnaryProcessor<T>::UnaryProcessor
		(const UnaryProcessor& /* processor */, bool /* deep */)
	{
	}
				
	template <typename T>
	BALL_INLINE 
	UnaryProcessor<T>::~UnaryProcessor()
	{
	}
				
	template <typename T>
	BALL_INLINE 
	bool UnaryProcessor<T>::start()
	{
		return true;
	}

	template <typename T>
	BALL_INLINE 
	bool UnaryProcessor<T>::finish()
	{
		return false;
	}

	template <typename T>
	BALL_INLINE 
	Processor::Result UnaryProcessor<T>::operator () (T&)
	{
		return Processor::ABORT;
	}


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
		BinaryProcessor();

		/**	Copy constructor
		*/
		BinaryProcessor(const BinaryProcessor& processor, bool deep = true);

		/**	Destructor
		*/
		virtual ~BinaryProcessor();
		//@}

		/**	@name	Processor-specific methods
		*/
		//@{
		
		/**	start method
		*/
		virtual bool start();

		/**	finish method
		*/
		virtual bool finish();

		/**	operator ()
		*/
		virtual Processor::Result operator () (T1&, T2&);
		//@}
	};

	template <typename T1, typename T2>
	BALL_INLINE 
	BinaryProcessor<T1, T2>::BinaryProcessor()
	{
	}
				
	template <typename T1, typename T2>
	BALL_INLINE 
	BinaryProcessor<T1, T2>::BinaryProcessor
		(const BinaryProcessor& /* processor */, bool /* deep */)
	{
	}
				
	template <typename T1, typename T2>
	BALL_INLINE 
	BinaryProcessor<T1, T2>::~BinaryProcessor()
	{
	}
				

	template <typename T1, typename T2>
	BALL_INLINE 
	bool BinaryProcessor<T1, T2>::start()
	{
		return true;
	}

	template <typename T1, typename T2>
	BALL_INLINE 
	bool BinaryProcessor<T1, T2>::finish()
	{
		return false;
	}

	
	template <typename T1, typename T2>
	BALL_INLINE 
	Processor::Result BinaryProcessor<T1, T2>::operator () (T1&, T2&)
	{
		return Processor::ABORT;
	}

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_PROCESSOR_H
