// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_PROCESSOR_H
#define BALL_CONCEPT_PROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BASEFUNCTOR_H
#	include <BALL/CONCEPT/baseFunctor.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL 
{

	/**	@name Processor Concept
			
	 	 \ingroup ConceptsMiscellaneous
	*/
	//@{

	/**	Global Types and Enums for Processors
	*/
	class BALL_EXPORT Processor
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
		BALL_CREATE(UnaryProcessor)
		/**	Default constructor
		*/
		UnaryProcessor()
		{
		}

		/**	Copy constructor
		*/
		UnaryProcessor
			(const UnaryProcessor& /* processor */)
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

	/**
	 * This is the baseclass for UnaryProcessors that do not change the objects they
	 * are applied to. If your processor should work on const objects this is the
	 * right class to derive from.
	 */
	template<typename T>
	class ConstUnaryProcessor : public UnaryFunctor<T, Processor::Result>
	{
		public:
			BALL_CREATE(ConstUnaryProcessor)

			virtual ~ConstUnaryProcessor()
			{
			}

			virtual bool start()
			{
				return true;
			}

			virtual bool finish()
			{
				return true;
			}

			virtual Processor::Result operator()(const T&)
			{
				return Processor::CONTINUE;
			}
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
		BALL_CREATE(BinaryProcessor)

		/**	Default constructor
		*/
		BinaryProcessor()	
		{
		}

		/**	Copy constructor
		*/
		BinaryProcessor	
			(const BinaryProcessor& /* processor */)
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

	template <typename T1, typename T2>
	class ConstBinaryProcessor
		: public BinaryFunctor<T1, T2, Processor::Result>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		BALL_CREATE(ConstBinaryProcessor)

		/**	Destructor
		*/
		virtual ~ConstBinaryProcessor()
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
		virtual Processor::Result operator () (const T1&, const T2&)
		{
			return Processor::CONTINUE;
		}
		//@}
	};

	//@}
// required for visual studio
#ifdef BALL_COMPILER_MSVC
class Atom;
template class BALL_EXPORT UnaryProcessor<Atom>;
#endif

} // namespace BALL

#endif // BALL_CONCEPT_PROCESSOR_H
