%{
#include <BALL/FORMAT/CIFFile.h>

using namespace BALL;

#include "CIFParserParser.h"
#include <string.h>

#define YY_INPUT(buf,result,max_size) \
{ \
	((std::istream*)CIFFile::state.current_parser)->read(buf, max_size);\
	result = CIFFile::state.current_parser->gcount();\
}

#undef BALL_CIFPARSERLEXER_DEBUG
//#define BALL_CIFPARSERLEXER_DEBUG

#ifdef BALL_CIFPARSERLEXER_DEBUG
#define CIF_DEBUG(a) Log.info() << "CIFParserLexer: " << a << std::endl;
#else
#define CIF_DEBUG(a)
#endif

int state_buffer = 0;
int textfield_state_buffer = 0;
%}

%s			in_data_heading
%s      in_save_heading 
%s      in_single_quote
%s      in_double_quote
%s			expecting_value
%x			in_loop
%x			in_save_frame
%x			in_textfield

%option noyywrap
%option array

EOL									[\n\r]
AnyPrintChar				({OrdinaryChar}|[\"\'#$_\t[:blank:];\[\]])
/*AnyPrintChar				[^\n]*/

SingleQuote					\'
DoubleQuote					\"
SQWithChar					\'{OrdinaryChar}
SQWithSpace					\'[[:blank:]\n\r]
NonSQPrintChar			[^\n']
NonDQPrintChar			[^\n"]
COMMENT     				(#({AnyPrintChar})*{EOL})+
TOKENIZED_COMMENT		[[:blank:]\n\r]*{COMMENT}
WHITESPACE				  ([[:blank:]\n\r]|{TOKENIZED_COMMENT})+
DATA_								[dD][aA][tT][aA]_
SAVE_               [sS][aA][vV][eE]_
NBC									[^[:blank:]\n\r]
LOOP_								[lL][oO][oO][pP]_
STOP_								[sS][tT][oO][pP]_
TAG                 [[:blank:]]*_{NBC}+

OrdinaryChar        [0-9a-zA-Z!%&$()*+,\-./:<=>?@\\^`{|}\[\]~]

/* NOTE: in the formal definition, UnquotedStrings are allowed to begin with a semicolon if they are not at the beginning of a line! We do not support this case yet! */
UnquotedString      {OrdinaryChar}({OrdinaryChar}|\;)* 
SingleQuotedString  \'{NonSQPrintChar}*{SQWithSpace}
DoubleQuotedString 	\"{NonDQPrintChar}*\"
UnderScore					_

TEXTLEADCHAR        {OrdinaryChar}|{SingleQuote}|{DoubleQuote}|#|$|_|[[:blank:]]|\[|\] 
/*CHARSTRING				 	({UnquotedString}|{SingleQuotedString}|{DoubleQuotedString}) */
CHARSTRING				 	{UnquotedString}

/* Attention: TEXTFIELD is only valid at the beginning of a line! But flex does not allow to use the ^ - operator correctly in definitions, so it has to be manually preceded when used*/
TEXTFIELD           ;({AnyPrintChar}*{EOL}(({TEXTLEADCHAR}({AnyPrintChar}*))?{EOL})*);

DIGIT               [0-9]
UnsignedInteger     {DIGIT}+
EXPONENT         	 	[eE]([+\ -]?){UnsignedInteger}
INT									[+\-]?[0-9]+
FLOAT								[+\-]?(([0-9]+"."[0-9]+)|{EXPONENT})
NUMBER							({INT}|{FLOAT})
NUMERIC							({NUMBER}|{NUMBER}({UnsignedInteger}))
/* Attention a TEXTFIELD can also be a value. Since we cannot use the ^-operator correctly in the definitions, we added TEXTFIELD manually in the rules. */
VALUE								[.?]|({NUMERIC}|{CHARSTRING})

%%

<in_textfield>^;{WHITESPACE}* {
	CIF_DEBUG("TextField stop")
	BEGIN(textfield_state_buffer);
	return TK_TEXTFIELD;
}

<*>^;{WHITESPACE}* {
	CIF_DEBUG("TextField start")
	textfield_state_buffer = YY_START;
	BEGIN(in_textfield);
	return TK_TEXTFIELD;
}

<in_textfield>{
		{WHITESPACE}* {
			             CIF_DEBUG("TextField Whitespace")
				           strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
			             return TK_TEXTFIELD_LINE;	
		}
		
		{TEXTLEADCHAR}{AnyPrintChar}*{EOL} {
			CIF_DEBUG("TextField line")
			strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
			return TK_TEXTFIELD_LINE;	
	  }
}

<in_single_quote>{SingleQuote}/{WHITESPACE}+ {
	CIF_DEBUG("SingleQuote")
	BEGIN(state_buffer);
	return TK_CLOSE_SINGLE_QUOTE;
}

<*>{WHITESPACE}+{SingleQuote} {
	CIF_DEBUG("OpenSingleQuote")
	state_buffer = YY_START;
	BEGIN(in_single_quote);
	return TK_OPEN_SINGLE_QUOTE;
}

<*>{SingleQuote} {
	CIF_DEBUG("SingleQuote")
	return TK_SINGLE_QUOTE;
}


<in_double_quote>{DoubleQuote}/{WHITESPACE}+ {
	CIF_DEBUG("DoubleQuote")
	BEGIN(state_buffer);
	return TK_CLOSE_DOUBLE_QUOTE;
}

<*>{DoubleQuote}/[^[:blank]]+ {
	CIF_DEBUG("DoubleQuote")
	return TK_DOUBLE_QUOTE;
}

<*>{WHITESPACE}+{DoubleQuote} {
	CIF_DEBUG("OpenDoubleQuote")
	state_buffer = YY_START;
	BEGIN(in_double_quote);
	return TK_OPEN_DOUBLE_QUOTE;
}

<in_save_heading>{WHITESPACE}					{
	CIF_DEBUG("L: closing save frame")
	BEGIN(INITIAL);
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_WHITESPACE;
}

<*>{WHITESPACE}												{
	CIF_DEBUG("L: found whitespace")
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_WHITESPACE;
}

^{DATA_}						|
{WHITESPACE}{DATA_} {
	CIF_DEBUG("L: found data heading")
	BEGIN(in_data_heading);
}

<in_data_heading>{NBC}+								{
	CIF_DEBUG("L: found data heading title " << yytext)
	BEGIN(INITIAL); 
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_DATA_HEADING;
}

{SAVE_}																{
	CIF_DEBUG("L: found save frame")
	BEGIN(in_save_heading); 
	return TK_SAVE;
}

<in_save_heading>{NBC}+								{
	CIF_DEBUG("L: now in save frame " << yytext)
	BEGIN(INITIAL); 
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_SAVE_HEADING;
}

{LOOP_}																{
	CIF_DEBUG("found loop")
	BEGIN(in_loop); 
	return TK_LOOP;
}

<in_loop>{
	{STOP_}															{
		CIF_DEBUG("found loop stop")
		BEGIN(INITIAL); 
		return TK_STOP;
	}

	{VALUE}	{
		CIF_DEBUG("found looped value " << yytext)
		strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
		return TK_VALUE;
	}
}

{VALUE}	{
 	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	CIF_DEBUG("found value " << yytext)
	return TK_VALUE;
}

<*>{UnderScore} {
	CIF_DEBUG("UnderScore")
	return TK_UNDERSCORE;
}

%%


