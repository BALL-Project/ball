// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: expressionPredicate.h,v 1.9 2003/03/26 13:56:30 anhi Exp $

#ifndef BALL_KERNEL_EXPRESSIONPREDICATE_H
#define BALL_KERNEL_EXPRESSIONPREDICATE_H

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{
	/** ExpressionPredicate. 
			The base class for all kernel predicates (defined in BALL/KERNEL/standardPredicates.h
			and used by  \link Expression Expression \endlink . You have to derive your own predicate classes from 
			this class if you want to use (and register) them with  \link Expression Expression \endlink .
			 \par
			@see Expression
    	
			\ingroup  Predicates
	*/
	class ExpressionPredicate
		:	public UnaryPredicate<Atom>
	{
		public:

		BALL_CREATE(ExpressionPredicate)

		/**	@name	Constructors and Destructor.
		*/
		//@{

		/** Default Constructor
		*/
		ExpressionPredicate() 
			throw();
			
		/** Copy constructor 
		 */
		ExpressionPredicate(const ExpressionPredicate& predicate) 
			throw();

		/** Detailed Constructor.
				Construct an ExpressionPredicate with a string.
				@param argument the argument to use for this predicate
		*/
		ExpressionPredicate(const String& argument) 
			throw();

		/** Destructor
		*/
		virtual ~ExpressionPredicate() 
			throw();

		//@}
		/** @name Assignment 
		 */
		//@{

		/** Assignment operator 
		 */
		const ExpressionPredicate& operator = (const ExpressionPredicate& predicate) 
			throw();

		/** Clear method 
		 */
		virtual void clear() 
			throw();

		//@}
		/** @name Predicates 
		 */
		//@{

		/** Equality operator 
		 */
		bool operator == (const ExpressionPredicate& predicate) const 
			throw();

		//@}
		/** @name Accessors.
		*/
		//@{

		/** evaluate this ExpressionPredicate by comparing its argument with
				the selected predicate of atom.
				@param atom the atom to compare with
		*/
		virtual bool operator () (const Atom& atom) const 
			throw();

		/** set the Argument of this ExpressionPredicate
				@param argument the string to use for this predicate
		*/
		virtual void setArgument(const String& argument) 
			throw();

		/** Get the argument of an ExpressionPredicate
				@return the argument of the expression
		 */
		const String& getArgument() const 
			throw();

		//@}
		
		protected:
		
		/*_ The argument of the predicate 
		 */
		String argument_;
	};
}

#endif // BALL_KERNEL_EXPRESSIONPREDICATE_H
