%{
// $Id: expressionParserLexer.l,v 1.2 2002/01/27 05:08:59 oliver Exp $
#include <BALL/KERNEL/expressionParser.h>

using namespace BALL;

#include "expressionParserParser.h"
#include <string.h>

%}

%option nounput
%option noyywrap

%%

NOT|! {
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);						
						return TK_NOT;
}

AND|\&\& {
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);						
						return TK_AND;
}

OR|\|\| {
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);
						return TK_AND;
}

[0-9a-zA-Z_]* {
						ExpressionParserlval.text = yytext;
						return TK_PREDICATE_NAME;
}

"[^']*" {
						ExpressionParserlval.text = yytext;
						return TK_ARGS;
}

.	{
						ExpressionParser::state.current_parser->state.char_count++;
						return yytext[0];
}
%%

YY_BUFFER_STATE ExpressionParser_buffer;
void ExpressionParser_initBuffer(const char* buf)
{
	ExpressionParser_buffer = ExpressionParser_scan_string(buf);
}

void ExpressionParser_delBuffer()
{
	ExpressionParser_delete_buffer(ExpressionParser_buffer);
}
