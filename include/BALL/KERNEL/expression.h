// $Id: expression.h,v 1.14 2001/07/13 14:24:53 anker Exp $

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

		/** The type of an expression node in the tree.
				The type determines how a node is to be interpreted.
		*/
		enum Type
		{	
			/// The node is invalid.
			INVALID = 0,
			/// The node is a leaf.
			LEAF,
			/// The node is a logical OR conjunction.
			OR,
			/// The node is a logical AND conjunction.
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
				@param	negate set to {\bf true} if the node's predicate should be
								negated
		*/
		ExpressionTree(ExpressionPredicate* predicate, bool negate = false)
			throw();

		/**
		*/
		ExpressionTree(Type type, list<const ExpressionTree*> children, bool negate = false) 
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

		/** Inequality operator 
		 */
		bool operator != (const ExpressionTree& tree) const 
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
		void appendChild(const ExpressionTree* child) 
			throw();

		/** Get the list of children.
		*/
		const list<const ExpressionTree*>& getChildren() const
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
		
		/** @name Protected methods
		*/
		//@{

		/*_ A helper function for operator == () that compares the children of
				a node.
		*/
		bool compareChildren_(const ExpressionTree& tree) const
			throw();

		/*_ The type of this node.
		*/
		Type												type_;
		
		/*_ Negation flag. If set, the value of this node will be negated.
		*/
		bool												negate_;
		
		/*_ A pointer to the predicate that this node represents.
		*/
		ExpressionPredicate*				predicate_;
		
		/*_ A list containing pointers to the children of this node. 
		*/
		list<const ExpressionTree*>	children_;

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

		/// BAUSTELLE
		typedef	list<SyntaxTree*>::iterator				Iterator;

		/// BAUSTELLE
		typedef	list<SyntaxTree*>::const_iterator	ConstIterator;
		
		//@}

		/**	@name	Constructors and Destructors	
		*/
		//@{

		/** Default constructor.
		*/
		SyntaxTree()
			throw();

		/** Detailed constructor.
		*/
		SyntaxTree(const String& expression)
			throw();

		/** Destructor.
		*/
		virtual ~SyntaxTree()
			throw();
		
		//@}
		/** @name Assignment
		*/
		//@{

		/** Clear method. This method brings this instance to the state after
				default construction. {\bf Note} that the list of children will be
				cleared but the childrem themselves will \emph{not} be deleted.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Return a mutable iterator pointing to the first child.
		*/
		Iterator begin()
			throw();

		/** Return a mutable iterator pointing to the last child.
		*/
		Iterator end()
			throw();

		/** Return a constant iterator pointing to the first child.
		*/
		ConstIterator begin() const
			throw();

		/** Return a constant iterator pointing to the last child.
		*/
		ConstIterator end() const
			throw();	

		/** Merge this node with a tree (from the left side). A non-empty tree
				will be deleted by this function.
		*/
		void mergeLeft(SyntaxTree* tree)
			throw();

		/** Merge this node with a tree (from the right side). A non-empty tree
				will be deleted by this function.
		*/
		void mergeRight(SyntaxTree* tree)
			throw();

		/** Parse the tree. This method expands brackets and collapses all
				conjunctions and disjunctions. The result is a tree that consists
				of BAUSTELLE.
		*/
		void parse()
			throw(Exception::ParseError);
		
		//@}
		/** @name Public attributes
		*/
		//@{

		/** BAUSTELLE
		*/
		String								expression;

		/** BAUSTELLE
		*/
		String								argument;

		/** BAUSTELLE
		*/
		bool									evaluated;

		/** BAUSTELLE
		*/
		bool									negate;

		/** BAUSTELLE
		*/
		ExpressionTree::Type	type;

		/** BAUSTELLE
		*/
		list<SyntaxTree*>			children;

		//@}
			
		protected:
		
		/*_ BAUSTELLE
		*/
		void expandBrackets_()
			throw(Exception::ParseError);
		
		/*_ BAUSTELLE
		*/
		void collapseANDs_()
			throw(Exception::ParseError);
		
		/*_ BAUSTELLE
		*/
		void collapseORs_()
			throw(Exception::ParseError);

	};

	/** Expression class. 
			This class provides a frontend to ExpressionTree.	\\
			@see ExpressionTree
			{\bf Definition:} \URL{BALL/KERNEL/expression.h}
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

		/** Evaluate the expression of {\tt atom}
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

		/*_ BAUSTELLE
		*/
		ExpressionTree*	constructExpressionTree_(const SyntaxTree& tree)
			
			throw();

		/*_ BAUSTELLE
		*/
		void registerStandardPredicates_() 
			throw();

		//@}
		/*_ @name Protected attributes
		*/
		//@{

		/*_ BAUSTELLE
		*/
		StringHashMap<CreationMethod> create_methods_;

		/*_ BAUSTELLE
		*/
		ExpressionTree*								expression_tree_;

		/*_ BAUSTELLE
		*/
		String												expression_string_;

		//@}
	};
	//@}
}	

#endif // BALL_KERNEL_EXPRESSION_H
