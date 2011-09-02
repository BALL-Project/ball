%{
#include <BALL/FORMAT/ClustalFile.h>


#define YYERROR_VERBOSE


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
	char text[CLUSTALPARSER_LINE_LENGTH];
	unsigned int  integer;
	}


%token <text>     TK_WHITESPACE
%token <text>     TK_CONSERV_LINE
%token <text>     TK_IDENT
%token <text>     TK_SEQUENCE
%token <integer>  TK_NUM
%token <text>     TK_CLUSTAL
%token            TK_EMPTYLINE

%%
/*        grammar rules           */

clustal: header content {}
 ;

header: TK_CLUSTAL /*TK_WHITESPACE TK_COMMENT*/ {}
	;

content: data_content optional_emptylines

data_content: data_content emptylines block{}
									|emptylines block {}
		;

optional_emptylines: /*empty*/  {}
				| emptylines  {}
;

emptylines: TK_EMPTYLINE {}
							|emptylines TK_EMPTYLINE {}
;

block: sequence_block TK_CONSERV_LINE{
															//save line htat gives the degree of conservation in the alignment
															current_block.conserv_line= $2;

															//add the block to the current parser
															ClustalFile::state.current_parser->addBlock(current_block);
/*TODO was wenn conservLine mehr Zeichen als die sequences enthält*/

				//reset the internal block
				current_block.reset();

											}

										|sequence_block {
												
												//add the block to the current parser
												ClustalFile::state.current_parser->addBlock(current_block);
//reset the internal block
												current_block.reset();

											}
 ;

sequence_block: sequence_line { 			 //TODO besser hier adden oder schon früher
																						//current_block.addSequenceLine(current_seq_line);
			
																			}

											|sequence_block sequence_line {
														
																										}
			;

optional_ws: /*empty*/
						|TK_WHITESPACE {}
;

sequence_line:optional_ws TK_IDENT TK_WHITESPACE TK_SEQUENCE TK_WHITESPACE TK_NUM	optional_ws	{

										//reset the internal sequenceLine
											current_seq_line.reset();
					
											//save the ident
											current_seq_line.ident=$2;
//cout<<	"current_seq_line.ident=$1" <<current_seq_line.ident<<"whereas in $6:"<<$6<<endl;


											//save the sequence
											current_seq_line.sequence=$4;
											//save the length of the sequence
//TODO convert string to int!!!!!!!!!!!
											current_seq_line.length=$6;							
											current_block.addSequenceLine(current_seq_line);

//cout<<"added the sequenceline. now the size is: "<< current_block.seqs.size()<<endl;

//current_block.dump();

																											}

					/*	| TK_WHITESPACE TK_IDENT TK_WHITESPACE TK_SEQUENCE TK_WHITESPACE TK_NUM {

										//reset the internal sequenceLine
										current_seq_line.reset();
					
										//save the ident
										current_seq_line.ident=$1;
cout<<	"current_seq_line.ident=$1" <<current_seq_line.ident<<"whereas in $1:"<<$1<<endl;

cout<<" whereas in $2:"<<$2<<" whereas in $4:"<<$3<<" whereas in $6:"<<$6<<endl;														

										//save the sequence
										current_seq_line.sequence=$4;
										//save the length of the sequence
										current_seq_line.length=$6;

	//add the line to the block
												current_block.addSequenceLine(current_seq_line);	

			cout<<"added the sequenceline. now the size is: "<< current_block.seqs.size()<<endl;
																															}*/
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
