%{
#include <BALL/FORMAT/ClustalFile.h>
#include <BALL/COMMON/parserDefinitions.h>

using namespace BALL;

#include <string.h>
#include "ClustalParserParser.h"

#define YY_INPUT(buf,result,max_size) \
{ \
	((std::istream*)ClustalFile::state.current_parser)->read(buf, max_size);\
	result = ClustalFile::state.current_parser->gcount();\
}

#undef BALL_CLUSTALPARSERLEXER_DEBUG
//#define BALL_CLUSTALPARSERLEXER_DEBUG

#ifdef BALL_CLUSTALPARSERLEXER_DEBUG
#define CLUSTAL_DEBUG(a) Log.info() << "ClustalParserLexer: " << a << std::endl;
#else
#define CLUSTAL_DEBUG(a)
#endif

/*
int state_buffer = 0;
int textfield_state_buffer = 0;
*/
void ClustalParserLexer_reset()
{
	BEGIN(0);
	YY_FLUSH_BUFFER;
}

%}


%option nounput
%option noyywrap

EOL					[\n\r]
WHITESPACE				[[:blank:]]+
CLUSTAL             			[cC][lL][uU][sS][tT][aA][lL]([[:blank:]\r]*|{OrdinaryChar}*|{MiscChars}*)*{EOL}
MiscChars  				[%&$()*+,/:<=>?\\^`{|}\[\]~\"\'#_;]
SEQUENCE				[GPAVLIMCFWHKRQNEDSTYgpavlimcfwhkrqnedsty\-]+
NUMBER					[0-9]+
EVAL 					[*:. ]+
EmptyLine                               ^[[:blank:]]*{EOL}
OrdinaryChar        [0-9!a-zA-Z\-.@_]


%%
{WHITESPACE} {
	CLUSTAL_DEBUG("WHITESPACE")
	return TK_WHITESPACE;
}


{CLUSTAL} {

	//copy what is read to lval.text to enable bison to access the values
	strncpy(ClustalParserlval.text, yytext, CLUSTALPARSER_LINE_LENGTH);
	CLUSTAL_DEBUG("CLUSTAL")
	return TK_CLUSTAL;
}

{NUMBER} {
ClustalParserlval.integer = atoi( yytext);
	CLUSTAL_DEBUG("NUMBER")
	return TK_NUM;
}

{SEQUENCE} {
strncpy(ClustalParserlval.text, yytext, CLUSTALPARSER_LINE_LENGTH);
	CLUSTAL_DEBUG ("SEQUENCE")
	return TK_SEQUENCE;
}

{EVAL} {
strncpy(ClustalParserlval.text, yytext, CLUSTALPARSER_LINE_LENGTH);
	CLUSTAL_DEBUG("CONSERV_LINE")
	return TK_CONSERV_LINE;
}

{OrdinaryChar}+ {

		strncpy(ClustalParserlval.text, yytext, CLUSTALPARSER_LINE_LENGTH);
		CLUSTAL_DEBUG("IDENT")
		return TK_IDENT;
}


{EmptyLine} {
	 		CLUSTAL_DEBUG("EMPTY_LINE")
			return TK_EMPTYLINE;
}

%%
