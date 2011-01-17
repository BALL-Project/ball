// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_EXPRESSION_H
#define BALL_KERNEL_EXPRESSION_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_EXPRESSIONPARSER_H
#	include <BALL/KERNEL/expressionParser.h>
#endif

namespace BALL
{
	class Atom;
	class ExpressionTree;

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
	class BALL_EXPORT Expression
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
		Expression();

		/** Copy Constructor.
		*/
		Expression(const Expression& expression);

		/** Construct an Expression with a string
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		Expression(const String& expression_string);

		/** Destructor.
		 */
		virtual ~Expression();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		bool hasPredicate(const String& name) const;

		/** Equality operator 
		 */
		bool operator == (const Expression& expression) const;

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Evaluate the expression of <tt>atom</tt>
				@param atom
		*/
		virtual bool operator () (const Atom& atom) const;

		/**	Create a new predicate according to the name.
				If the predicate is not known, return 0.
				@param name the name of the predicate
				@param args the optional argument of the predicate
		*/
		ExpressionPredicate* getPredicate(const String& name,
		                                  const String& args = "") const;

		/**	Register a new predicate class.
		*/
		void registerPredicate(const String& name, CreationMethod creation_method);

		/** Set the expression and build a tree for it.
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		void setExpression(const String& expression);

		/** Get the expression string.
		*/
		const String& getExpressionString() const;

		/** Get the expression tree.
		*/
		const ExpressionTree* getExpressionTree() const;

		/** Get the creation methods.
		*/
		const StringHashMap<CreationMethod>& getCreationMethods() const;

		//@}
		/** @name Assignment 
		 */
		//@{ 

		/** Assignment operator 
		 */
		Expression& operator = (const Expression& expression);

		/** Clear method 
		 */
		virtual void clear();

		//@}

		protected:

		/*_ @name Protected methods
		*/
		//@{

		/*_ Construct the expression tree from the SyntaxTree
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		ExpressionTree*	constructExpressionTree_(const ExpressionParser::SyntaxTree& tree);

		/*_ Register the predicates defined by default.
				See also: BALL/KERNEL/standardPredicates.h
		*/
		void registerStandardPredicates_();

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

