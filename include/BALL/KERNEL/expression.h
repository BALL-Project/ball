// $Id: expression.h,v 1.3 2000/05/20 13:34:44 oliver Exp $

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
			{\bf Definition:}\URL{BALL/KERNEL/expression.h}
	*/
	//@{

	/**
	*/
	class ExpressionPredicate
		:	public UnaryPredicate<Atom>
	{
		public:

		BALL_CREATE_NODEEP(ExpressionPredicate)

		/**	@name	Constructors and Destructors.
		*/
		//@{

		/** Default Constructor
		*/
		ExpressionPredicate();
		
		/** Detailed Constructor.
				Construct an ExpressionPredicate with a string.
				@param argument the argument to use for this predicate
		*/
		ExpressionPredicate(const String& argument);

		/** Destructor
		*/
		virtual ~ExpressionPredicate();
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
		//@}
		
		protected:
		
		String argument_;
	};

	/**
	*/
	class ExpressionTree
	{
		public:

		// BAUSTELLE
		// BALL_CREATE_NODEEP(ExpressionTree)


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

		/**	@name	Constructors and Destructors
		*/
		//@{
			
		/**	Default constructor.
				Create an empty expression node. The node's type is set to INVALID,
				{\tt negate_} is set to {\bf false}, the internal predicate is set
				to 0, and the list of children is empty.
		*/
		ExpressionTree();
			
		/**	Detailled construtor.
				Create an expression node representing a leaf, i.e., a 
				predicate.
				@param	predicate the node's predicate
				@param	negate set to {\bf true} if the node's predicate should be negated
		*/
		ExpressionTree(ExpressionPredicate* predicate, bool negate = false);

		/**
		*/
		ExpressionTree(Type type, list<ExpressionTree*> children, bool negate = false);

		/**	Destructor
		*/
		virtual ~ExpressionTree();
		//@}

		/**	@name	Predicates
		*/
		//@{
		
		/** Evaluate the (sub)expression.
		*/
		virtual bool operator () (const Atom& atom) const;
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Set the expression node's type.
		*/
		void setType(Type type);
		
		/**	Set the expression node's negation mode.
		*/
		void setNegate(bool negate);
		
		/**	Set the predicate.
		*/
		void setPredicate(ExpressionPredicate* predicate);

		/**	Append a child to the tree.
		*/
		void appendChild(ExpressionTree* child);
		//@}

		
		protected:
		
		Type									type_;
		bool									negate_;
		ExpressionPredicate*	predicate_;
		list<ExpressionTree*>	children_;
	};

	/**
	*/
	class SyntaxTree
	{
		public:
		
		// BAUSTELLE
		// BALL_CREATE_NODEEP(SyntaxTree)

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
	/**
	*/

	class Expression
	{
		public:
		
		// BAUSTELLE
		// BALL_CREATE_NODEEP(Expression)

		/**	@name	Type Definitions
		*/
		//@{

		/**	A creation method for predicates.
		*/
		typedef void * (*CreationMethod) ();
		//@}
 
		/**	@name	Constructors and Destructors
		*/
		//@{
		/** Default Constructor.
		*/
		Expression();

		/** Copy Constructor.
		*/
		Expression(const Expression& expression);

		/** Construct an Expression with a string
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
		bool hasPredicate(const String& name);
		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Evaluate the expression of {\tt atom}
				@param atom
		*/
		virtual bool operator () (const Atom& atom) const;

		/**	Create a new predicate according to the name.
				If the predicate is not known, return 0.
		*/
		ExpressionPredicate* getPredicate(const String& name, const String& args = "") const;

		/**	Register a new predicate class.
		*/
		void registerPredicate(const String& name, CreationMethod creation_method);

		/**
		*/
		void setExpression(const String& expression);

		//@}

		protected:

		ExpressionTree*	constructExpressionTree_(const SyntaxTree& tree);
		void registerStandardPredicates_();

		StringHashMap<CreationMethod> create_methods_;
		ExpressionTree*								expression_tree_;

	};
	//@}
}	

#endif // BALL_KERNEL_EXPRESSION_H
