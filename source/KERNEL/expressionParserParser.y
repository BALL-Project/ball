%{

#include <BALL/KERNEL/expressionParser.h>
#include <BALL/KERNEL/expressionTree.h>

using namespace BALL;
using namespace std;

extern int ExpressionParserlex();
extern void ExpressionParsererror(char* s);

/* ??? */
/* There is some nasty problem with alloca under Intel/icc/Linux */
/* so we use (inefficient) malloc instead. */
#undef alloca
#define alloca malloc

%}
%union {
	char*	text;
	ExpressionParser::SyntaxTree* node;
}

%left 	<text>		TK_OR
%left 	<text>		TK_AND
%left							TK_NOT
%left							TK_WHITESPACE;
%token	<text>		TK_ARGS
%token	<text>		TK_PREDICATE_NAME
%token	<text>		TK_OPEN_BRACKET
%token	<text>		TK_CLOSE_BRACKET

%type		<node>		kernel_expression
%type		<node>		expression
%type		<node>		predicate


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
			$2->negate = true;
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
		TK_PREDICATE_NAME TK_OPEN_BRACKET TK_ARGS TK_CLOSE_BRACKET { 
			*$2 = '\0';
			*($4 - 1) = '\0';
			$$ = new ExpressionParser::SyntaxTree($1, $3 + 1);
		}
	|	TK_PREDICATE_NAME TK_OPEN_BRACKET TK_CLOSE_BRACKET { 
			*$2 = '\0';
			$$ = new ExpressionParser::SyntaxTree($1, 0);
		}
	;

%%

void ExpressionParsererror(char* s)
{
	throw Exception::ParseError(__FILE__, 0, 
															ExpressionParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(ExpressionParser::state.char_count) + String(")"));
}
