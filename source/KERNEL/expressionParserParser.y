%{

#include <BALL/KERNEL/expressionParser.h>
#include <BALL/KERNEL/expressionTree.h>

using namespace BALL;
using namespace std;

extern int ExpressionParserlex();
extern void ExpressionParsererror(char const* s)
	throw(Exception::ParseError);

%}
%union {
	char*	text;
	ExpressionParser::SyntaxTree* node;
}

%left 	<text>		TK_OR
%left 	<text>		TK_AND
%left		<text>		TK_NOT
%left		<text>		TK_WHITESPACE;
%left		<text>		TK_SOMETHING
%token	<text>		TK_PREDICATE_NAME
%token	<text>		TK_OPEN_BRACKET
%token	<text>		TK_CLOSE_BRACKET

%type		<node>		kernel_expression
%type		<node>		expression
%type		<node>		predicate
%type		<text>		balanced_brackets
%type		<text>		outer_brackets
%type		<text>		inside_brackets
%type		<text>		something

%start kernel_expression

%%

kernel_expression:	expression 
		{ 
			ExpressionParser::state.tree = $1;
		}
	;

expression:
		predicate { 
			$$ = $1;
		}
	| TK_WHITESPACE expression {
			$$ = $2;
		}
	| expression TK_WHITESPACE {
			$$ = $1;
		}
	|	TK_NOT expression {
			$$ = $2;
			$2->negate = !$2->negate;
		}
	| TK_OPEN_BRACKET expression TK_CLOSE_BRACKET { 
			$$ = $2; 
		}
	| expression TK_AND expression { 
			$$ = new ExpressionParser::SyntaxTree($1, $3, ExpressionTree::AND); 
		}
	| expression TK_OR expression {
			$$ = new ExpressionParser::SyntaxTree($1, $3, ExpressionTree::OR); 
		}
	;

predicate:
		TK_PREDICATE_NAME outer_brackets { 
			*$2 = '\0';
			$$ = new ExpressionParser::SyntaxTree($1, $2 + 1);
		}
	;

outer_brackets:
		balanced_brackets {
			$$ = $1;
			*($1 + strlen($1) - 1) = '\0';
		}
		;

balanced_brackets: 
		TK_OPEN_BRACKET inside_brackets TK_CLOSE_BRACKET {
			$$ = $1;
		}
		;

inside_brackets:
		something balanced_brackets inside_brackets {
			$$ = $1;
		}
	|	something {
			$$ = $1;
		}
	;

something:	
		{
			$$ = 0;
		}
	|	TK_SOMETHING something {
			$$ = $1;
		}
	|	TK_WHITESPACE something {
			$$ = $1;
		}
	|	TK_PREDICATE_NAME something {
			$$ = $1;
		}
	|	TK_AND something {
			$$ = $1;
		}
	|	TK_OR something {
			$$ = $1;
		}
	|	TK_NOT something {
			$$ = $1;
		}
	;

%%
void ExpressionParsererror(char const* s)
	throw(Exception::ParseError)
{
	throw Exception::ParseError(__FILE__, 0, 
															ExpressionParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(ExpressionParser::state.char_count) + String(")"));
}
