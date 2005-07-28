// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: baseFunctor.h,v 1.10.6.1 2005/07/28 13:26:16 amoll Exp $
//

#ifndef BALL_CONCEPT_BASEFUNCTOR_H
#define BALL_CONCEPT_BASEFUNCTOR_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL 
{

	/**	@name Generic Functors.
			
	* 	 \ingroup  ConceptsMiscellaneous
	*/
	//@{
		
	/**	Generic Unary Functor Class.
			
	*/
	template <class ArgumentType, class ResultType>
	BALL_EXPORT class UnaryFunctor
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
			
	*/
	template <class FirstArgumentType, class SecondArgumentType, class ResultType>
	BALL_EXPORT class BinaryFunctor
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
