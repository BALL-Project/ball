// $Id: expressionParser.C,v 1.1 2002/01/26 22:08:08 oliver Exp $

#include <BALL/KERNEL/expressionParser.h>
#include <BALL/KERNEL/PTE.h>

#include <algorithm>

// defined in the lexer (expressionParserLexer.l)
extern void ExpressionParser_initBuffer(const char* buf);
extern void ExpressionParser_delBuffer();
extern int ExpressionParserparse();

namespace BALL
{
	ExpressionParser::SyntaxTree::SyntaxTree()
		throw()
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
		throw()
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
			throw()
		:	expression(""),
			predicate(predicate_name),
			argument(args),
			evaluated(false),
			negate(false),
			type(),
			children()
	{
	}

	ExpressionParser::SyntaxTree::~SyntaxTree()
		throw()
	{
		for (Iterator it = begin(); it != end(); ++it)
		{
			delete *it;
		}
	}

	void ExpressionParser::SyntaxTree::clear()
		throw()
	{
		expression = "";
		argument = "";
		evaluated = false;
		negate = false;
		type = ExpressionTree::INVALID;
		children.clear();
	}

	ExpressionParser::SyntaxTree::Iterator ExpressionParser::SyntaxTree::begin()
		throw()
	{
		return children.begin();
	}
	
	ExpressionParser::SyntaxTree::ConstIterator ExpressionParser::SyntaxTree::begin() const
		throw()
	{
		return children.begin();
	}
	
	ExpressionParser::SyntaxTree::Iterator ExpressionParser::SyntaxTree::end()
		throw()
	{
		return children.end();
	}
	
	ExpressionParser::SyntaxTree::ConstIterator ExpressionParser::SyntaxTree::end() const
		throw()
	{
		return children.end();
	}

	void ExpressionParser::SyntaxTree::dump(std::ostream& os, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(os);
		BALL_DUMP_HEADER(os, this, this);
		BALL_DUMP_DEPTH(os, depth);
		os << "[expression = " << expression 
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
	{
	}

	ExpressionParser::~ExpressionParser()
	{
	}

	const ExpressionParser::SyntaxTree& ExpressionParser::getSyntaxTree() const
	{
		return syntax_tree_;
	}

	void ExpressionParser::parse(const String& s)
		throw(Exception::ParseError)
	{
		// make the internals of this parser available for all
		state.current_parser = this;
		state.buffer = s.c_str();	
		state.char_count = 0;
    
		try
		{
			ExpressionParser_initBuffer(state.buffer);
			ExpressionParserparse();
			ExpressionParser_delBuffer();	
		}
		catch (Exception::ParseError& e)
		{
			ExpressionParser_delBuffer();
			throw e;
		}		
	}
	
	struct ExpressionParser::State ExpressionParser::state;
	
} // namespace BALL
