// $Id: expressionPredicate.h,v 1.1 2001/07/16 19:38:42 anker Exp $

#ifndef BALL_KERNEL_EXPRESSIONPREDICATE_H
#define BALL_KERNEL_EXPRESSIONPREDICATE_H

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{

	/** ExpressionPredicate. 
			A class that provides a predicate for expressions that can be used on
			atoms. 
			\\
			There are standard predicates that are predefined for
			convenient use (\Ref{BAUSTELLE}) when building an expression.
			\\
			{\bf Definition} \URL{BALL/KERNEL/expression.h}
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
