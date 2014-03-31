%{
// $Id: expressionParserLexer.l,v 1.4 2002/01/28 00:10:06 oliver Exp $
#include <BALL/KERNEL/expressionParser.h>
#include <BALL/COMMON/parserDefinitions.h>

using namespace BALL;

#include "expressionParserParser.h"
#include <string.h>

%}

%option nounput
%option noyywrap

%%

NOT|! {
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);						
						ExpressionParserlval.text = yytext;
						return TK_NOT;
}

AND|\&\& {
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);						
						ExpressionParserlval.text = yytext;
						return TK_AND;
}

OR|\|\| {
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);
						ExpressionParserlval.text = yytext;
						return TK_OR;
}

[0-9a-zA-Z_]* {
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);
						ExpressionParserlval.text = yytext;
						return TK_PREDICATE_NAME;
}

[\040\t]* { 
						ExpressionParser::state.current_parser->state.char_count += strlen(yytext);
						return TK_WHITESPACE;
}

"("	{
						ExpressionParser::state.current_parser->state.char_count++;
						ExpressionParserlval.text = yytext;
						return TK_OPEN_BRACKET;
}

")"	{
						ExpressionParser::state.current_parser->state.char_count++;
						ExpressionParserlval.text = yytext;
						return TK_CLOSE_BRACKET;
}

.	{
						ExpressionParser::state.current_parser->state.char_count++;
						ExpressionParserlval.text = yytext;
						return TK_SOMETHING;
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

void ExpressionParser_destroy()
{
#ifdef BALL_HAS_YYLEX_DESTROY
	ExpressionParserlex_destroy();
#endif
}
