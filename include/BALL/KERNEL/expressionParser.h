// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_EXPRESSIONPARSER_H
#define BALL_KERNEL_EXPRESSIONPARSER_H

#ifndef BALL_KERNEL_EXPRESSION_TREE_H
#	include <BALL/KERNEL/expressionTree.h>
#endif

namespace BALL 
{

	/** Expression Parser.
			A simple parser for BALL kernel expressions. It reads a string and constructs
			a syntax tree from the boolean expressions contained therein.
			 \par
			
			\ingroup KernelMiscellaneous 
	*/
	class BALL_EXPORT ExpressionParser
	{
		public:

		/** SyntaxTree.
				This internal class should be used in the implementation
				of  \link ExpressionParser ExpressionParser \endlink  only.
				 \par
		*/
		class BALL_EXPORT SyntaxTree
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
			SyntaxTree();

			/** Detailed constructor.
			*/
			SyntaxTree(const char* predicate_name, const char* args);
				
			/**	Detailed constructor
			*/
			SyntaxTree(SyntaxTree* left, SyntaxTree* right, ExpressionTree::Type type);

			/** Destructor.
			*/
			virtual ~SyntaxTree();
			
			//@}
			/** @name Assignment
			*/
			//@{

			/** Clear method. This method brings this instance to the state after
					default construction. <b>Note</b> that the list of children will be
					cleared but the childrem themselves will \emph{not} be deleted.
			*/
			virtual void clear();

			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Return a mutable iterator pointing to the first child.
			*/
			Iterator begin();

			/** Return a mutable iterator pointing to the last child.
			*/
			Iterator end();

			/** Return a constant iterator pointing to the first child.
			*/
			ConstIterator begin() const;

			/** Return a constant iterator pointing to the last child.
			*/
			ConstIterator end() const;

			//@}

			/**	@name Debugging
			*/
			//@{
			void dump(std::ostream& is = std::cout, Size depth = 0) const;
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
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		void parse(const String& s);

		/**	Return the parsed system
		 *  @throw Exception::NullPointer if the syntax tree is NULL
		 */
		const SyntaxTree& getSyntaxTree() const;

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
