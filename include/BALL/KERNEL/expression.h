// $Id: expression.h,v 1.10 2001/06/27 01:33:51 oliver Exp $

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

namespace BALL 
{

	/**	@name	Expression Trees.
			Evaluate simple expressions on Atoms.
			\\
			{\bf Definition:}\URL{BALL/KERNEL/expression.h}
	*/
	//@{


	/** ExpressionPredicate. 
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
		ExpressionPredicate() throw();
		
		/** Copy constructor 
		 */
		ExpressionPredicate(const ExpressionPredicate& predicate) throw();

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
		virtual void clear() throw();

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


	/** Expression tree class.
			Represents the logical tree of an Expression. This is the backend of
			Expression.
			\\
			{\bf Definition:} \URL{BALL/KERNEL/expression.h}
			@see Expression
	*/
	class ExpressionTree
	{
		public:

		BALL_CREATE(ExpressionTree)

		/**	@name	Type Definitions
		*/
		//@{

		/**
		*/
		enum Type
		{	
			///
			INVALID = 0,
			///
			LEAF,
			///
			OR,
			///
			AND
		};
		//@}

		/**	@name	Constructors and Destructor
		*/
		//@{
			
		/**	Default constructor.
				Create an empty expression node. The node's type is set to INVALID,
				{\tt negate_} is set to {\bf false}, the internal predicate is set
				to 0, and the list of children is empty.
		*/
		ExpressionTree() 
			throw();
			
		/** Copy constructor 
		 */
		ExpressionTree(const ExpressionTree& tree) 
			throw();

		/**	Detailed constructor.
				Create an expression node representing a leaf, i.e., a 
				predicate.
				@param	predicate the node's predicate
				@param	negate set to {\bf true} if the node's predicate should be negated
		*/
		ExpressionTree(ExpressionPredicate* predicate, bool negate = false)
			throw();

		/**
		*/
		ExpressionTree(Type type, list<ExpressionTree*> children, bool negate = false) 
			throw();

		/**	Destructor
		*/
		virtual ~ExpressionTree() 
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{
		
		/** Evaluate the (sub)expression.
		*/
		virtual bool operator () (const Atom& atom) const 
			throw();

		/** Equality operator 
		 */
		bool operator == (const ExpressionTree& tree) const 
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Set the expression node's type.
		*/
		void setType(Type type) 
			throw();

		/** Get the expression node's type 
		 */
		Type getType() const 
			throw();
		
		/**	Set the expression node's negation mode.
		*/
		void setNegate(bool negate) 
			throw();

		/** Get the expression node's negation mode. 
		 */
		bool getNegate() const 
			throw();

		/**	Set the predicate.
		*/
		void setPredicate(ExpressionPredicate* predicate) 
			throw();

		/**	Get the predicate. 
		 */
		ExpressionPredicate* getPredicate() const 
			throw();

		/**	Append a child to the tree.
		*/
		void appendChild(ExpressionTree* child) 
			throw();

		//@}
		/** @name Asignment 
		 */
		//@{

		/** Asignment operator 
		 */
		const ExpressionTree& operator = (const ExpressionTree& tree) 
			throw();

		/** Clear method 
		 */
		virtual void clear() 
			throw();

		//@}
		
		protected:
		
		Type									type_;
		bool									negate_;
		ExpressionPredicate*	predicate_;
		list<ExpressionTree*>	children_;
	};


	/** SyntaxTree.
	 *  {\bf Definition} \URL{BALL/KERNEL/expression.h}
	*/
	class SyntaxTree
	{
		public:
		
		BALL_CREATE(SyntaxTree)

		/**	@name	Type Definitions
		*/
		//@{

		///
		typedef	list<SyntaxTree*>::iterator				Iterator;

		///
		typedef	list<SyntaxTree*>::const_iterator	ConstIterator;
		
		//@}
		/**	@name	Constructors and Destructors	
		*/
		//@{

		/** Default constructor
		*/
		SyntaxTree();

		/** Detailed constructor
		*/
		SyntaxTree(const String& expression);

		/**
		*/
		virtual ~SyntaxTree();
		
		//@}
		/**	@name	Accessors
		*/
		//@{
		//
		///
		Iterator begin();

		///
		Iterator end();

		///
		ConstIterator begin() const;

		///
		ConstIterator end() const;	

		///
		void mergeLeft(SyntaxTree* tree);

		///
		void mergeRight(SyntaxTree* tree);

		///
		void parse();
		
		//@}

		String								expression;
		String								argument;
		bool									evaluated;
		bool									negate;
		ExpressionTree::Type	type;
		list<SyntaxTree*>			children;
			
		protected:
		
		void expandBrackets_();
		void collapseANDs_();
		void collapseORs_();

	};

	/** Expression class. 
			This class provides a frontend to ExpressionTree.	\\
			@see ExpressionTree
			{\bf Definition:} \URL{BALL/KERNEL/expression.h}
	*/
	class Expression
	{
		public:
		
		// BAUSTELLE
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
		Expression(const Expression& expression) throw();

		/** Construct an Expression with a string
		*/
		Expression(const String& expression_string) throw();

		/** Destructor.
		*/
		virtual ~Expression() throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		bool hasPredicate(const String& name) const throw();

		/** Equality operator 
		 */
		bool operator == (const Expression& expression) const throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Evaluate the expression of {\tt atom}
				@param atom
		*/
		virtual bool operator () (const Atom& atom) const throw();

		/**	Create a new predicate according to the name.
				If the predicate is not known, return 0.
		*/
		ExpressionPredicate* getPredicate(const String& name, 
				const String& args = "") const throw();

		/**	Register a new predicate class.
		*/
		void registerPredicate(const String& name, CreationMethod creation_method)
			throw();

		/** set the expression string
		*/
		void setExpression(const String& expression) throw();

		/** get the expression string
		*/
		const String& getExpression() const throw();

		//@}
		/** @name Assignment 
		 */
		//@{ 

		/** Assignment operator 
		 */
		const Expression& operator = (const Expression& expression) throw();

		/** Clear method 
		 */
		virtual void clear() throw();

		//@}

		protected:

		ExpressionTree*	constructExpressionTree_(const SyntaxTree& tree)
			throw();
		void registerStandardPredicates_() throw();

		StringHashMap<CreationMethod> create_methods_;
		ExpressionTree*								expression_tree_;
		String												expression_string_;
	};
	//@}
}	

#endif // BALL_KERNEL_EXPRESSION_H
