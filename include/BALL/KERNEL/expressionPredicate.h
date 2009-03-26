// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: expressionPredicate.h,v 1.12 2005/10/23 12:02:18 oliver Exp $
//

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
	class BALL_EXPORT ExpressionPredicate
		:	public UnaryPredicate<Atom>
	{
		public:

		BALL_CREATE(ExpressionPredicate)

		/**	@name	Constructors and Destructor.
		*/
		//@{

		/** Default Constructor
		*/
		ExpressionPredicate();
			
		/** Copy constructor 
		 */
		ExpressionPredicate(const ExpressionPredicate& predicate);

		/** Detailed Constructor.
				Construct an ExpressionPredicate with a string.
				@param argument the argument to use for this predicate
		*/
		ExpressionPredicate(const String& argument);

		/** Destructor
		*/
		virtual ~ExpressionPredicate();

		//@}
		/** @name Assignment 
		 */
		//@{

		/** Assignment operator 
		 */
		ExpressionPredicate& operator = (const ExpressionPredicate& predicate);

		/** Clear method 
		 */
		virtual void clear();

		//@}
		/** @name Predicates 
		 */
		//@{

		/** Equality operator 
		 */
		bool operator == (const ExpressionPredicate& predicate) const;

		//@}
		/** @name Accessors.
		*/
		//@{

		/** evaluate this ExpressionPredicate by comparing its argument with
				the selected predicate of atom.
				@param atom the atom to compare with
		*/
		virtual bool operator () (const Atom& atom) const;

		/** set the Argument of this ExpressionPredicate
				@param argument the string to use for this predicate
		*/
		virtual void setArgument(const String& argument);

		/** Get the argument of an ExpressionPredicate
				@return the argument of the expression
		 */
		const String& getArgument() const;

		//@}
		
		protected:
		
		/*_ The argument of the predicate 
		 */
		String argument_;
	};
}

#endif // BALL_KERNEL_EXPRESSIONPREDICATE_H
