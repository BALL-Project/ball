%{
#include <BALL/FORMAT/ClustalFile.h>

#include <iostream>
#include <string>

using namespace std;
using namespace BALL;

extern int yylex();
extern void yyerror(char const* s);

ClustalFile::Block current_block;
ClustalFile::SequenceLine current_seq_line;

#undef BALL_CLUSTALPARSERPARSER_DEBUG
//#define BALL_CLUSTALPARSERPARSER_DEBUG

#ifdef BALL_CLUSTALPARSERPARSER_DEBUG
#define CLUSTAL_DEBUG(a) Log.info() << a << std::endl;
#define YYDEBUG 1
#else
#define CLUSTAL_DEBUG(a)
#endif






%}

/*         bison declarations      */
%union {
	char  	text[CLUSTALPARSER_LINE_LENGTH];
	int	  	integer;
	}


%token <text>		  	TK_WHITESPACE
%token <text>	  		TK_CONSERV_LINE
%token <text>			TK_IDENT
%token <text>			TK_SEQUENCE
%token <integer> 		TK_NUM
%token <text>			TK_CLUSTAL



%%
/*        grammar rules           */

Fasta: header content {}
 ;

header: TK_CLUSTAL /*TK_WHITESPACE TK_COMMENT*/ {}
	;

content:  content TK_WHITESPACE block {}
		| TK_WHITESPACE block {}
	;

block: sequence_block TK_CONSERV_LINE {
					current_block.reset();
					current_block.conserv_line= $2;
					ClustalFile::state.current_parser->addBlock(current_block);
	}
	;

sequence_block:   sequence_block sequence_line { current_block.addSequenceLine(current_seq_line);}
		| sequence_line { current_block.addSequenceLine(current_seq_line);}
	;

sequence_line: TK_IDENT TK_WHITESPACE TK_SEQUENCE TK_WHITESPACE TK_NUM {
			current_seq_line.reset();
			current_seq_line.ident=$1;
			current_seq_line.sequence=$3;
			current_seq_line.length=$5;
	}
	;



%%
/*        additional C code            */

void yyerror(char const* s)
{
	printf("ClustalParserParser(): Parse Error! %s \n",s);
}

/*
int main(int argc, char **argv)
{
	yydebug = 1;
	return yyparse();
}

*/
