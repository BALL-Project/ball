%{
#include <BALL/FORMAT/CIFFile.h>

using namespace BALL;

#include "CIFParserParser.h"
#include <string.h>

/*
 * BALL Phase 05.1-06: copy yytext into the parser's fixed-width
 * CIFParserlval.text[CIFPARSER_LINE_LENGTH] buffer with explicit
 * nul-termination, replacing the historical
 *   strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH);
 * idiom that triggers -Wstringop-truncation under gcc (no terminating
 * nul when the source is longer than the destination). Tokens longer
 * than CIFPARSER_LINE_LENGTH-1 are truncated deterministically.
 */
static inline void CIF_copy_text(char* dst, const char* src)
{
        const size_t srclen = strlen(src);
        const size_t max    = (size_t)(CIFPARSER_LINE_LENGTH - 1);
        const size_t n      = srclen < max ? srclen : max;
        memcpy(dst, src, n);
        dst[n] = '\0';
}

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

void CIFParserLexer_reset()
{
	BEGIN(0);
	YY_FLUSH_BUFFER;
}
%}

%s			in_data_heading
%s      in_save_heading 
%x      in_single_quote
%x      in_double_quote
%x			in_loop
%x			in_save_frame
%x			in_textfield

%option noyywrap
%option array

EOL									[\n\r]
AnyPrintChar				({OrdinaryChar}|[\"\'#$_\t[:blank:];\[\]])

SingleQuote					\'
DoubleQuote					\"
COMMENT     				(#({AnyPrintChar})*{EOL})+
TOKENIZED_COMMENT		[[:blank:]\n\r]*{COMMENT}
WHITESPACE				  [[:blank:]\n\r]+
WS_OR_COMMENT				([[:blank:]\n\r]|{TOKENIZED_COMMENT})+
DATA_								[dD][aA][tT][aA]_
SAVE_               [sS][aA][vV][eE]_
NBC									[^[:blank:]\n\r]
LOOP_								[lL][oO][oO][pP]_
STOP_								[sS][tT][oO][pP]_
TAG                 [[:blank:]]*_{NBC}+

OrdinaryChar        [0-9a-zA-Z!%&$()*+,\-./:<=>?@\\^`{|}\[\]~]

/* NOTE: in the formal definition, UnquotedStrings are allowed to begin with a semicolon if they are not at the beginning of a line! We do not support this case yet! */
UnquotedString      {OrdinaryChar}({OrdinaryChar}|\;)* 
UnderScore					_

TEXTLEADCHAR        {OrdinaryChar}|{SingleQuote}|{DoubleQuote}|#|$|_|[[:blank:]]|\[|\] 
CHARSTRING				 	{UnquotedString}

DIGIT               [0-9]
UnsignedInteger     {DIGIT}+
EXPONENT         	 	[eE]([+\ -]?){UnsignedInteger}
INT									[+\-]?[0-9]+
FLOAT								[+\-]?(([0-9]+"."[0-9]+)|{EXPONENT})
NUMBER							({INT}|{FLOAT})
NUMERIC							({NUMBER}|{NUMBER}({UnsignedInteger}))
VALUE								[.?]|({NUMERIC}|{CHARSTRING})

%%

<in_textfield>{
		^;/{WS_OR_COMMENT}* {
			CIF_DEBUG("TextField stop")
			BEGIN(textfield_state_buffer);
			return TK_TEXTFIELD;
		}

		{WS_OR_COMMENT}* {
       CIF_DEBUG("TextField Whitespace")
       CIF_copy_text(CIFParserlval.text, yytext);
       return TK_TEXTFIELD_LINE;	
		}
		
		{TEXTLEADCHAR}{AnyPrintChar}*{EOL} {
			CIF_DEBUG("TextField line")
			CIF_copy_text(CIFParserlval.text, yytext);
			return TK_TEXTFIELD_LINE;	
	  }
}

<*>^;/{WS_OR_COMMENT}* {
	CIF_DEBUG("TextField start")
	textfield_state_buffer = YY_START;
	BEGIN(in_textfield);
	return TK_TEXTFIELD;
}

<in_single_quote>{
	{SingleQuote}/{WS_OR_COMMENT}+ {
		CIF_DEBUG("SingleQuote")
		BEGIN(state_buffer);
		return TK_CLOSE_SINGLE_QUOTE;
  }

	# {
		CIF_DEBUG("Quoting # to value")
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_VALUE;
	}
	
	; {
		CIF_DEBUG("Quoting ; to value")
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_VALUE;
	}

	{VALUE}	{
		CIF_copy_text(CIFParserlval.text, yytext);
		CIF_DEBUG("found value " << yytext)
		return TK_VALUE;
	}

	{WHITESPACE} {
		CIF_DEBUG("WhiteSpace")
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_WHITESPACE;
	}
}

<in_data_heading,in_save_heading,in_loop,in_save_frame,INITIAL>{WHITESPACE}+{SingleQuote} {
	CIF_DEBUG("OpenSingleQuote")
	state_buffer = YY_START;
	BEGIN(in_single_quote);
	return TK_OPEN_SINGLE_QUOTE;
}

<*>{SingleQuote} {
	CIF_DEBUG("SingleQuote")
	return TK_SINGLE_QUOTE;
}

<in_double_quote>{
	{DoubleQuote}/{WS_OR_COMMENT}+ {
		CIF_DEBUG("DoubleQuote")
		BEGIN(state_buffer);
		return TK_CLOSE_DOUBLE_QUOTE;
  }

	# {
		CIF_DEBUG("Quoting # to value")
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_VALUE;
	}
	
	; {
		CIF_DEBUG("Quoting ; to value")
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_VALUE;
	}

	{VALUE}	{
		CIF_copy_text(CIFParserlval.text, yytext);
		CIF_DEBUG("found value " << yytext)
		return TK_VALUE;
	}

	{WHITESPACE} {
		CIF_DEBUG("WhiteSpace")
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_WHITESPACE;
	}
}

<in_data_heading,in_save_heading,in_loop,in_save_frame,INITIAL>{WHITESPACE}+{DoubleQuote} {
	CIF_DEBUG("OpenDoubleQuote")
	state_buffer = YY_START;
	BEGIN(in_double_quote);
	return TK_OPEN_DOUBLE_QUOTE;
}

<*>{DoubleQuote} {
	CIF_DEBUG("DoubleQuote")
	return TK_DOUBLE_QUOTE;
}

<in_save_heading>{
	{NBC}+ {
		CIF_DEBUG("L: now in save frame " << yytext)
		BEGIN(INITIAL); 
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_SAVE_HEADING;
	}
	
	{WS_OR_COMMENT} {
		CIF_DEBUG("L: closing save frame")
		BEGIN(INITIAL);
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_WHITESPACE;
	}
}

{SAVE_}																{
	CIF_DEBUG("L: found save frame")
	BEGIN(in_save_heading); 
	return TK_SAVE;
}


<in_data_heading>{
	{NBC}+ {
		CIF_DEBUG("L: found data heading title " << yytext)
		BEGIN(INITIAL); 
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_DATA_HEADING;
  }
}

^{DATA_}						   |
{WS_OR_COMMENT}{DATA_} {
	CIF_DEBUG("L: found data heading")
	BEGIN(in_data_heading);
}

<in_loop>{
	{STOP_}	{
		CIF_DEBUG("found loop stop")
		BEGIN(INITIAL); 
		return TK_STOP;
	}

	{VALUE}	{
		CIF_DEBUG("found looped value " << yytext)
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_VALUE;
	}
	
	{WS_OR_COMMENT} {
		CIF_DEBUG("Found WhiteSpace")
		CIF_copy_text(CIFParserlval.text, yytext);
		return TK_WHITESPACE;
	 }
}

{LOOP_}	{
	CIF_DEBUG("found loop")
	BEGIN(in_loop); 
	return TK_LOOP;
}


{VALUE}	{
 	CIF_copy_text(CIFParserlval.text, yytext);
	CIF_DEBUG("found value " << yytext)
	return TK_VALUE;
}

<*>{UnderScore} {
	CIF_DEBUG("UnderScore")
	return TK_UNDERSCORE;
}

{WS_OR_COMMENT} {
	CIF_DEBUG("Found WhiteSpace")
	CIF_copy_text(CIFParserlval.text, yytext);
	return TK_WHITESPACE;
 }
%%

void CIFParser_destroy()
{
#ifdef BALL_HAS_YYLEX_DESTROY
	CIFParserlex_destroy();
#endif
}
