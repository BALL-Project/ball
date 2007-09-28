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

%}

%s			in_data_heading
%s      in_save_heading 
%s			expecting_value
%x			in_loop
%x			in_save_frame

%option noyywrap
%option array

EOL									\n
AnyPrintChar				({OrdinaryChar}|[\"\'#$_\t[:blank:];\[\]])
AnyPrintChar1				({OrdinaryChar}|[\'#$_\t[:blank:];\[\]])
AnyPrintChar2				({OrdinaryChar}|[\"#$_\t[:blank:];\[\]])
/*AnyPrintChar				[^\n]*/

NonSQPrintChar			[^\n']
NonDQPrintChar			[^\n"]
COMMENT     				(#({AnyPrintChar})*{EOL})+
TOKENIZED_COMMENT		[[:blank:]\n]*{COMMENT}
WHITESPACE				  ([[:blank:]\n]|{TOKENIZED_COMMENT})+
DATA_								[dD][aA][tT][aA]_
SAVE_               [sS][aA][vV][eE]_
NBC									[^[:blank:]\n]
LOOP_								[lL][oO][oO][pP]_
STOP_								[sS][tT][oO][pP]_
TAG                 [[:blank:]]*_{NBC}+

OrdinaryChar        [0-9a-zA-Z!%&()*+,\-./:<=>?@\\^`{|}~]

/* NOTE: in the formal definition, UnquotedStrings are allowed to begin with a semicolon if they are not at the beginning of a line! We do not support this case yet! */
UnquotedString      {OrdinaryChar}{NBC}* 
SingleQuotedString  \'{AnyPrintChar2}*\'
DoubleQuotedString 	\"{AnyPrintChar1}*\"

TEXTLEADCHAR        {OrdinaryChar}|\"|#|$|\'|_|[[:blank:]]|\[|\] 
CHARSTRING				 	({UnquotedString}|{SingleQuotedString}|{DoubleQuotedString})

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

<in_save_heading>{WHITESPACE}					{
	/*printf("L: closing save frame\n"); */
	BEGIN(INITIAL);
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_WHITESPACE;
}

<*>{WHITESPACE}												{
	/*printf("L: found whitespace\n");*/ 
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_WHITESPACE;
}

{DATA_}																{
	/*printf("L: found data heading\n");*/ 
	BEGIN(in_data_heading);
}

<in_data_heading>{NBC}+								{
	/*printf("L: found data heading title %s\n", yytext);*/ 
	BEGIN(INITIAL); 
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_DATA_HEADING;
}

{SAVE_}																{
	/*printf("L: found a save frame\n");*/ 
	BEGIN(in_save_heading); 
	return TK_SAVE;
}

<in_save_heading>{NBC}+								{
	/*printf("L: now in save frame %s\n", yytext);*/ 
	BEGIN(INITIAL); 
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	return TK_SAVE_HEADING;
}

{TAG} 																{
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	/*printf("L: found a tag %s\n", yytext);*/ 
	BEGIN(expecting_value); 
	return TK_TAG;
}

<expecting_value>{VALUE}							|
<expecting_value>^{TEXTFIELD}					{ 
	strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
	/*printf("L: found a value %s\n", yytext); */
	BEGIN(INITIAL); 
	return TK_VALUE;
}

{LOOP_}																{
	/*printf("L: found a loop\n");*/ 
	BEGIN(in_loop); 
	return TK_LOOP;
}

<in_loop>{
	{TAG}								  							{
		strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
		/*printf("L: found a looped tag %s\n", yytext);*/ 
		return TK_TAG;
	}

	{STOP_}															{
		/*printf("L: loop stop\n");*/ 
		BEGIN(INITIAL); 
		return TK_STOP;
	}

	{VALUE}							  							|
	^{TEXTFIELD}												{
		strncpy(CIFParserlval.text, yytext, CIFPARSER_LINE_LENGTH); 
		/*printf("L: found a looped value %s\n", yytext);*/ 
		return TK_VALUE;
	}
}

%%




