// $Id: syntaxTree.h,v 1.1 2001/07/16 19:38:42 anker Exp $

#ifndef BALL_KERNEL_SYNTAXTREE_H
#define BALL_KERNEL_SYNTAXTREE_H

#ifndef BALL_KERNEL_EXPRESSIONTREE_H
#	include <BALL/KERNEL/expressionTree.h>
#endif

namespace BALL
{

	/** SyntaxTree.
			{\bf Definition} \URL{BALL/KERNEL/syntaxTree.h}
	*/
	class SyntaxTree
	{
		public:
		
		BALL_CREATE(SyntaxTree)

		/**	@name	Type Definitions
		*/
		//@{

		/// An iterator for the children of a given node
		typedef	list<SyntaxTree*>::iterator				Iterator;

		/// A const iterator for the children of a given node
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
		/**	@name Debugging
		*/
		//@{
		void dump(std::ostream& is = std::cout, Size depth = 0) const
			throw();
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

}

#endif //  BALL_KERNEL_SYNTAXTREE_H
