// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: expression.h,v 1.26 2003/03/26 13:56:29 anhi Exp $

#ifndef BALL_KERNEL_EXPRESSION_H
#define BALL_KERNEL_EXPRESSION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_EXPRESSIONTREE_H
#	include <BALL/KERNEL/expressionTree.h>
#endif

#ifndef BALL_KERNEL_EXPRESSIONPARSER_H
#	include <BALL/KERNEL/expressionParser.h>
#endif

namespace BALL 
{
	/** Expression class. 
			This class provides a frontend to ExpressionTree.	 \par
			Expressions may be built from the following modules:  \par
			AND & a conjunction  \par
			OR & a disjunction  \par
			predicate(argument) & a predicate class that is derived from
			\Ref{ExpressionPredicate) and provides <tt>operator () (const Atom& atom) const</tt>.  \par
			 \par
			Additionally brackets can be used for grouping. At least one bracket
			pair must exist which encloses the argument of a predicate. Empty arguments are allowed.
			
			@see ExpressionTree

    	\ingroup  Predicates
	*/
	class Expression
	{
		public:
		
		BALL_CREATE(Expression)

		/**	@name	Type Definitions
		*/
		//@{

		/**	A creation method for predicates.
		*/
		typedef void * (*CreationMethod) ();

		//@}
 		/**	@name	Constructors and Destructor
		*/
		//@{

		/** Default Constructor.
		*/
		Expression() throw();

		/** Copy Constructor.
		*/
		Expression(const Expression& expression) 
			throw();

		/** Construct an Expression with a string
		*/
		Expression(const String& expression_string) 
			throw();

		/** Destructor.
		*/
		virtual ~Expression() 
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		bool hasPredicate(const String& name) const 
			throw();

		/** Equality operator 
		 */
		bool operator == (const Expression& expression) const 
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Evaluate the expression of <tt>atom</tt>
				@param atom
		*/
		virtual bool operator () (const Atom& atom) const 
			throw();

		/**	Create a new predicate according to the name.
				If the predicate is not known, return 0.
				@param name the name of the predicate
				@param args the optional argument of the predicate
		*/
		ExpressionPredicate* getPredicate(const String& name, 
				const String& args = "") const 
			throw();

		/**	Register a new predicate class.
		*/
		void registerPredicate(const String& name, CreationMethod creation_method)
			throw();

		/** Set the expression and build a tree for it.
		*/
		void setExpression(const String& expression) 
			throw(Exception::ParseError);

		/** Get the expression string.
		*/
		const String& getExpressionString() const 
			throw();

		/** Get the expression tree.
		*/
		const ExpressionTree* getExpressionTree() const
			throw();

		/** Get the creation methods.
		*/
		const StringHashMap<CreationMethod>& getCreationMethods() const
			throw();

		//@}
		/** @name Assignment 
		 */
		//@{ 

		/** Assignment operator 
		 */
		const Expression& operator = (const Expression& expression) 
			throw();

		/** Clear method 
		 */
		virtual void clear() 
			throw();

		//@}

		protected:

		/*_ @name Protected methods
		*/
		//@{

		/*_ Construct the expression tree from the SyntaxTree
		*/
		ExpressionTree*	constructExpressionTree_(const ExpressionParser::SyntaxTree& tree)
			
			throw();

		/*_ Register the predicates defined by default.
				See also: \URL{BALL/KERNEL/standardPredicates.h}
		*/
		void registerStandardPredicates_() 
			throw();

		//@}
		/*_ @name Protected attributes
		*/
		//@{

		/*_ The methods to create the ExpressionPredicate instances
		*/
		StringHashMap<CreationMethod> create_methods_;

		/*_	The ExpressionTree constructed from the string.
				This tree contains the instances of the predicates.
		*/
		ExpressionTree*								expression_tree_;

		/*_ The string describing the expression.
		*/
		String												expression_string_;

		//@}
	};
}	

#endif // BALL_KERNEL_EXPRESSION_H
