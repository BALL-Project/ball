// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: expressionParser.h,v 1.10 2003/06/30 13:54:01 amoll Exp $

#ifndef BALL_KERNEL_EXPRESSIONPARSER_H
#define BALL_KERNEL_EXPRESSIONPARSER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_TREE_H
	#include <BALL/KERNEL/expressionTree.h>
#endif

namespace BALL 
{

	/** Expression Parser.
			A simple parser for BALL kernel expressions. It reads a string and constructs
			a syntax tree from the boolean expressions contained therein.
			 \par
			
			\ingroup KernelMiscellaneous 
	*/
	class ExpressionParser
	{
		public:

		/** SyntaxTree.
				This internal class should be used in the implementation
				of  \link ExpressionParser ExpressionParser \endlink  only.
				 \par
		*/
		class SyntaxTree
		{
			public:
			
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
			SyntaxTree(const char* predicate_name, const char* args)
				throw();
				
			/**	Detailed constructor
			*/
			SyntaxTree(SyntaxTree* left, SyntaxTree* right, ExpressionTree::Type type)
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
					default construction. <b>Note</b> that the list of children will be
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
			/**
			*/
			String								expression;

			/** ?????
			*/
			String								predicate;

			/** ?????
			*/
			String								argument;

			/** ?????
			*/
			bool									evaluated;

			/** ?????
			*/
			bool									negate;

			/** ?????
			*/
			ExpressionTree::Type	type;

			/** ?????
			*/
			list<SyntaxTree*>			children;
			//@}			
		};


		/**	@name Constructors and Destructors
		*/
		//@{

		///
		ExpressionParser();
			
		///
		ExpressionParser(const ExpressionParser& parser);

		///
		~ExpressionParser();
		//@}
		
		/**	@name	Parsing
		*/
		//@{
		/**	Parse an expression.
		*/
		void parse(const String& s)
			throw(Exception::ParseError);

		/**	Return the parsed system
		*/
		const SyntaxTree& getSyntaxTree() const
			throw(Exception::NullPointer);
		//@}
		
		struct State
		{
			Size							char_count;
			ExpressionParser*	current_parser;
			const char*				buffer;
			SyntaxTree*				tree;
		};
		
		static State state;

		protected:
		SyntaxTree*								syntax_tree_;
		static ExpressionParser*	current_parser_;
	};
} // namespace BALL

#endif // BALL_KERNEL_EXPRESSIONPARSER_H
