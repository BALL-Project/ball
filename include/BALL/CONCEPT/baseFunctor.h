// $Id: baseFunctor.h,v 1.1 1999/08/26 07:53:17 oliver Exp $

#ifndef BALL_CONCEPT_BASEFUNCTOR_H
#define BALL_CONCEPT_BASEFUNCTOR_H

namespace BALL {

	/**	@name Generic Functors.
			{\bf Definition:} \URL{BALL/CONCEPT/baseFunctor.h}
	*/
	//@{
		
	/**	Generic Unary Functor Class.
			{\bf Definition:} \URL{BALL/CONCEPT/baseFunctor.h}
	*/
	template <class ArgumentType, class ResultType>
	class UnaryFunctor
	{
		public:
		
		/**	@name	Type Definitions
		*/
		//@{

		/**
		*/
		typedef ResultType           result_type;

		/**
		*/
		typedef ArgumentType         argument_type;

		/**
		*/
		typedef ArgumentType&        argument_reference;

		/**
		*/
		typedef const ArgumentType&  const_argument_reference;

		/**
		*/
		typedef ArgumentType*        argument_pointer;

		/**
		*/
		typedef const ArgumentType*  const_argument_pointer;

		//@}
	};

	/**	Generic Binary Functor Class.
			{\bf Definition:} \URL{BALL/CONCEPT/baseFunctor.h}
	*/
	template <class FirstArgumentType, class SecondArgumentType, class ResultType>
	class BinaryFunctor
	{
		public:
		
		/**	@name	Type Definitions
		*/
		//@{

		/**
		*/
		typedef ResultType                 result_type;

		/**
		*/
		typedef FirstArgumentType          first_argument_type;
		/**
		*/
		typedef FirstArgumentType &        first_argument_reference;
		/**
		*/
		typedef const FirstArgumentType &  const_first_argument_reference;
		/**
		*/
		typedef FirstArgumentType *        first_argument_pointer;
		/**
		*/
		typedef const FirstArgumentType *  const_first_argument_pointer;
		
		/**
		*/
		typedef SecondArgumentType         second_argument_type;
		/**
		*/
		typedef SecondArgumentType &       second_argument_reference;
		/**
		*/
		typedef const SecondArgumentType & const_second_argument_reference;
		/**
		*/
		typedef SecondArgumentType *       second_argument_pointer;
		/**
		*/
		typedef const SecondArgumentType * const_second_argument_pointer;

		//@}
	};

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_BASEFUNCTOR_H
