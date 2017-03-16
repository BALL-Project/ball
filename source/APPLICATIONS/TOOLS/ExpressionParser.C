// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/KERNEL/expressionParser.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/PTE.h>
#include "version.h"

#include <iostream>
#include <string>
#include <vector>

#define BOOST_SPIRIT_DEBUG
#include <boost/bind.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>

using namespace BALL;
using namespace std;

namespace ascii = boost::spirit::ascii;
namespace fsn   = boost::fusion;
namespace phx   = boost::phoenix;
namespace qi    = boost::spirit::qi;

/*
struct STree
{
		int type;
		bool negate;
		string predicate;
		string argument;
		list<STree> children;
};


struct ExpressionParserNEW : qi::grammar<std::string::const_iterator, qi::space_type>
{
		ExpressionParserNEW() : ExpressionParserNEW::base_type(r_kernel_expression_)
		{
			s_and_.add("AND", "AND");
			s_and_.add("and", "AND");
			s_and_.add("&&",  "AND");
			s_or_.add("OR",  "OR");
			s_or_.add("or",  "OR");
			s_or_.add("||",  "OR");

			r_not_ = (
						 ( qi::lit("NOT") | qi::lit("not") | '!' ) >> qi::attr(true) | qi::attr(false)
					  ) [ phx::bind(&ExpressionParserNEW::negate, this) = qi::_1 ];

			r_predicate_ = +( ascii::char_ - '(' ) >> '(' >> *( ascii::char_ - ')' ) >> ')';

			r_expression_ = ( r_predicate_ | r_kernel_expression_ )
							>> *( s_and_ >> r_kernel_expression_ )
							>> *( s_or_ >> r_kernel_expression_ );



			r_kernel_expression_ = r_not_  >>
								   ( '(' >> r_expression_ >> ')' | r_expression_ );

			BOOST_SPIRIT_DEBUG_NODE(r_not_);
			BOOST_SPIRIT_DEBUG_NODE(r_predicate_);
			BOOST_SPIRIT_DEBUG_NODE(r_expression_);
			BOOST_SPIRIT_DEBUG_NODE(r_kernel_expression_);
		}

		bool negate;

		qi::symbols<char, std::string> s_and_;
		qi::symbols<char, std::string> s_or_;

		qi::rule<std::string::const_iterator> r_not_;
		qi::rule<std::string::const_iterator, qi::space_type> r_predicate_;
		qi::rule<std::string::const_iterator, qi::space_type> r_expression_;
		qi::rule<std::string::const_iterator, qi::space_type> r_kernel_expression_;
};

*/

#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/numericalSAS.h>

int main(int argc, char* argv[])
{
	CommandlineParser par("Boost Spirit SMILES Parser", "parse a SMILES string", VERSION, String(__DATE__), "Input");
	String man("Initial attempt to replace flex/bison by Boost's parser library Spirit.");
	par.registerOptionalStringParameter("exp", "expression to be parser", "true() AND element(H)");

	par.setToolManual(man);
	par.parse(argc, argv);

	/*
	string exp = par.get("exp").c_str();
	cout << "INPUT: " << exp << endl;

	string::const_iterator begin = exp.begin();
	string::const_iterator end = exp.end();


	ExpressionParserNEW e_parser;
	//bool success = qi::phrase_parse(begin, end, e_parser, qi::space);


	ExpressionParser ep;
	ep.parse(exp);
	const ExpressionParser::SyntaxTree& st = ep.getSyntaxTree();
	st.dump();
	*/

	return 0;
}


/*=============================================================================
	Copyright (c) 2001-2011 Hartmut Kaiser
	Copyright (c) 2001-2011 Joel de Guzman

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================

///////////////////////////////////////////////////////////////////////////////
//
//  Plain calculator example demonstrating the grammar. The parser is a
//  syntax checker only and does not do any semantic evaluation.
//
//  [ JDG May 10, 2002 ]        spirit1
//  [ JDG March 4, 2007 ]       spirit2
//  [ HK November 30, 2010 ]    spirit2/utree
//
///////////////////////////////////////////////////////////////////////////////

// This rather naive example demonstrates that you can pass an instance of a
// utree as the attribute for almost any grammar. As the result the utree will
// be filled with the parse tree as generated during the parsing. This is most
// of the time not what's desired, but is usually a good first step in order to
// prepare your grammar to generate a customized AST. See the calc_utree_ast
// example for a modified version of this grammar filling the attribute with a
// AST (abstract syntax tree) representing the math expression as matched from
// the input.

// #define BOOST_SPIRIT_DEBUG

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>

#include <iostream>
#include <string>

namespace client
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace spirit = boost::spirit;

	///////////////////////////////////////////////////////////////////////////////
	//  Our calculator grammar
	///////////////////////////////////////////////////////////////////////////////
	template <typename Iterator>
	struct calculator : qi::grammar<Iterator, ascii::space_type, spirit::utree()>
	{
		calculator() : calculator::base_type(expression)
		{
			using qi::uint_;
			using qi::char_;

			expression =
				term
				>> *(   (char_('+') >> term)
					|   (char_('-') >> term)
					)
				;

			term =
				factor
				>> *(   (char_('*') >> factor)
					|   (char_('/') >> factor)
					)
				;

			factor =
				uint_
				|   '(' >> expression >> ')'
				|   (char_('-') >> factor)
				|   (char_('+') >> factor)
				;

			BOOST_SPIRIT_DEBUG_NODE(expression);
			BOOST_SPIRIT_DEBUG_NODE(term);
			BOOST_SPIRIT_DEBUG_NODE(factor);
		}

		qi::rule<Iterator, ascii::space_type, spirit::utree()> expression;
		qi::rule<Iterator, ascii::space_type, spirit::utree::list_type()> term;
		qi::rule<Iterator, ascii::space_type, spirit::utree::list_type()> factor;
	};
}

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int main()
{
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "Expression parser...\n\n";
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "Type an expression...or [q or Q] to quit\n\n";

	using boost::spirit::ascii::space;
	using boost::spirit::utree;
	typedef std::string::const_iterator iterator_type;
	typedef client::calculator<iterator_type> calculator;

	calculator calc; // Our grammar

	std::string str;
	while (std::getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			break;

		std::string::const_iterator iter = str.begin();
		std::string::const_iterator end = str.end();
		utree ut;
		bool r = phrase_parse(iter, end, calc, space, ut);

		if (r && iter == end)
		{
			std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded: " << ut << "\n";
			std::cout << "-------------------------\n";
		}
		else
		{
			std::string rest(iter, end);
			std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << "stopped at: \": " << rest << "\"\n";
			std::cout << "-------------------------\n";
		}
	}

	std::cout << "Bye... :-) \n\n";
	return 0;
}

*/









