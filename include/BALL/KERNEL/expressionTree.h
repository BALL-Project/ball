// $Id: expressionTree.h,v 1.1 2001/07/16 19:38:42 anker Exp $

#ifndef BALL_KERNEL_EXPRESSIONTREE_H
#define BALL_KERNEL_EXPRESSIONTREE_H

#ifndef BALL_KERNEL_EXPRESSIONPREDICATE_H
#	include <BALL/KERNEL/expressionPredicate.h>
#endif

namespace BALL
{

	/** Expression tree class.
			Represents the logical tree of an Expression. This is the backend of
			Expression.
			\\
			{\bf Definition:} \URL{BALL/KERNEL/expressionTree.h}
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
		/** @name Assignment 
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
		/**	@name Debugging
		*/
		//@{
		void dump(std::ostream& is = std::cout, Size depth = 0) const
			throw();
		//@}

		
		protected:
		
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

}

#endif // BALL_KERNEL_EXPRESSIONTREE_H
