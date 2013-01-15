// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/expressionParser.h>

// defined in the lexer (expressionParserLexer.l)
extern void ExpressionParser_initBuffer(const char* buf);
extern void ExpressionParser_delBuffer();
extern void ExpressionParser_destroy();
extern int ExpressionParserparse();

namespace BALL
{
	ExpressionParser::SyntaxTree::SyntaxTree()
		:	expression(""),
			argument(""),
			evaluated(false),
			negate(false),
			type(ExpressionTree::INVALID),
			children()
	{
	}

	ExpressionParser::SyntaxTree::SyntaxTree
		(ExpressionParser::SyntaxTree* left, ExpressionParser::SyntaxTree* right, ExpressionTree::Type my_type)
		:	expression(""),
			argument(""),
			evaluated(false),
			negate(false),
			type(my_type),
			children()
	{
		children.push_back(left);
		children.push_back(right);
	}

	ExpressionParser::SyntaxTree::SyntaxTree
		(const char* predicate_name, const char* args)
		:	expression(""),
			predicate(predicate_name),
			argument((args == 0) ? "" : args),
			evaluated(false),
			negate(false),
			type(ExpressionTree::LEAF),
			children()
	{
	}

	ExpressionParser::SyntaxTree::~SyntaxTree()
	{
		for (Iterator it = begin(); it != end(); ++it)
		{
			delete *it;
		}
	}

	void ExpressionParser::SyntaxTree::clear()
	{
		expression = "";
		argument = "";
		evaluated = false;
		negate = false;
		type = ExpressionTree::INVALID;
		children.clear();
	}

	ExpressionParser::SyntaxTree::Iterator ExpressionParser::SyntaxTree::begin()
	{
		return children.begin();
	}
	
	ExpressionParser::SyntaxTree::ConstIterator ExpressionParser::SyntaxTree::begin() const
	{
		return children.begin();
	}
	
	ExpressionParser::SyntaxTree::Iterator ExpressionParser::SyntaxTree::end()
	{
		return children.end();
	}
	
	ExpressionParser::SyntaxTree::ConstIterator ExpressionParser::SyntaxTree::end() const
	{
		return children.end();
	}

	void ExpressionParser::SyntaxTree::dump(std::ostream& os, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(os);
		BALL_DUMP_HEADER(os, this, this);
		BALL_DUMP_DEPTH(os, depth);
		os << "[expression = " << expression 
			<< "  predicate = " << predicate 
			<< "  arg = " << argument
			<< "  evaluated = " << evaluated 
			<< "  negate = " << negate 
			<< "  type = " << type
			<< "]" << ::std::endl;
		list<ExpressionParser::SyntaxTree*>::const_iterator it = children.begin();
		for (; it != children.end(); ++it)
		{
			(*it)->dump(os, depth + 2);
		}
		BALL_DUMP_STREAM_SUFFIX(os);
	}

	ExpressionParser::ExpressionParser()
		:	syntax_tree_(0)
	{
	}

	ExpressionParser::ExpressionParser(const ExpressionParser& parser)
		:	syntax_tree_(0)
	{	
		if (parser.syntax_tree_ != 0) syntax_tree_ = new SyntaxTree(*parser.syntax_tree_);
	}


	ExpressionParser::~ExpressionParser()
	{
		if (syntax_tree_ != 0)
		{
			delete syntax_tree_;
		}

		ExpressionParser_destroy();
	}

	const ExpressionParser::SyntaxTree& ExpressionParser::getSyntaxTree() const
	{
		if (syntax_tree_ == 0) throw(Exception::NullPointer(__FILE__, __LINE__));
		return *syntax_tree_;
	}

	void ExpressionParser::parse(const String& s)
	{
		// clear former contents
		if (syntax_tree_ != 0)
		{
			delete syntax_tree_;
			syntax_tree_ = 0;
		}
		

		// make the internals of this parser available for all
		state.current_parser = this;
		state.buffer = s.c_str();	
		state.char_count = 0;
		state.tree = 0;
    
		try
		{
			ExpressionParser_initBuffer(state.buffer);
			ExpressionParserparse();
			ExpressionParser_delBuffer();		

			// the tree's mine now...
			syntax_tree_ = state.tree;
			state.tree = 0;
		}
		catch (Exception::ParseError& e)
		{
			ExpressionParser_delBuffer();
			throw e;
		}		
	}
	
	struct ExpressionParser::State ExpressionParser::state;
	
} // namespace BALL
