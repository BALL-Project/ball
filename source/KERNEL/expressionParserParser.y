%{

#include <BALL/KERNEL/expressionParser.h>
#include <BALL/KERNEL/expressionTree.h>

using namespace BALL;
using namespace std;

extern int ExpressionParserlex();
extern void ExpressionParsererror(char* s);

%}
%union {
	char*	text;
	ExpressionParser::SyntaxTree* node;
}

%token	<text>		TK_AND
%token	<text>		TK_OR
%token	<text>		TK_ARGS
%token	<text>		TK_PREDICATE_NAME

%type		<node>		kernel_expression
%type		<node>		expression
%type		<node>		predicate


%start kernel_expression

%%

kernel_expression:	expression 
		{ 
		}
	;

expression:
		predicate { 
			$$ = $1;
		}
	|	'!' expression {
			$$ = $2;
			$2->negate = true;
		}
	| '(' expression ')' { 
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
		TK_PREDICATE_NAME '(' TK_ARGS ')' { 
			$$ = new ExpressionParser::SyntaxTree($1, $3);
		}
	|	TK_PREDICATE_NAME '(' ')' { 
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
