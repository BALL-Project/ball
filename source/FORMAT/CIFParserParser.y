%{
#include <BALL/FORMAT/CIFFile.h>

#include <iostream>
#include <string>

using namespace std;
using namespace BALL;

extern int yylex();
extern void yyerror(char* s);

CIFFile::Datablock current_datablock;
CIFFile::Datacontent current_datacontent;
CIFFile::Item current_data_item;
CIFFile::SaveFrame current_saveframe;

bool first_datablock;

/* There is some nasty problem with alloca under Intel/icc/Linux */
/* so we use (inefficient) malloc instead. */
#undef alloca
#define alloca malloc

%}
/*         bison declarations      */
%union {
	char  	text[CIFPARSER_LINE_LENGTH];
	int	  	integer;
	double 	float_number;
}

%token <text>		      	TK_WHITESPACE
%token <text>		   			TK_DATA_HEADING
%token <text>		   			TK_SAVE_HEADING
%token					 				TK_SAVE
%token <text>    				TK_TAG
%token <text>	  				TK_VALUE
%token 									TK_LOOP
%token 									TK_STOP
%type  <text>		 				optional_whitespace
%%
/*        grammar rules           */

CIF: optional_whitespace optional_datablock {}
 ;

optional_datablock: /* empty */ {}
	| datablock_list optional_whitespace {}
	;

datablock_list: datablock {}
	| datablock_list /*TK_WHITESPACE*/ optional_whitespace datablock {};
	;

datablock: data_heading TK_WHITESPACE { 
			CIFFile::state.current_parser->insertDatablock(current_datablock);
		}
	/* NOTE: the final optinal_whitespace is neccessary to help BISON to finish a datablock instead of being forced to continue a datablock_content when seeing a coming TK_WHITESPACE */
	| data_heading TK_WHITESPACE datablock_content TK_WHITESPACE {
			CIFFile::state.current_parser->insertDatablock(current_datablock);
		}
	;

data_heading: TK_DATA_HEADING {/* cout << "Started data heading" << std::endl;*/
						current_datablock.clear();
						current_datablock.start($1);
	}
	;

datablock_content: data_items {/*cout << "P: content: data items "  << std::endl; */
								current_datablock.insertDatacontent(current_data_item); }
	| saveframe { /*cout << "P: content saveframe " << endl;*/
								current_datablock.insertDatacontent(current_saveframe); }
	| datablock_content TK_WHITESPACE data_items { /*cout << "P: data block content of type data items!" << std::endl;*/
								current_datablock.insertDatacontent(current_data_item);;}
	| datablock_content TK_WHITESPACE saveframe  { /* cout << "P: data block content of type save frame!" << std::endl;*/
								current_datablock.insertDatacontent(current_saveframe);}
	;

data_items: TK_TAG TK_WHITESPACE TK_VALUE {
					/*cout << "P: data item with tag title " << $1 << " and value " << $3 << std::endl;*/ 
							current_data_item.clear();
							current_data_item.addPair($1, $3);}
	|   loop tag_list TK_WHITESPACE loop_body optional_whitespace TK_STOP {
					/*cout << "P: data item tag list loop body stop: " <<  std::endl;*/
					}
	;

loop: TK_LOOP {
			current_data_item.startLoop();
		}
		;


tag_list: TK_WHITESPACE TK_TAG {
				current_data_item.addTag($2); /*cout << "P:   tag " << $2 << std::endl;*/   }
  | tag_list TK_WHITESPACE TK_TAG {
				current_data_item.addTag($3); /*cout << "P:   tag " << $3  << std::endl;*/  }
	;

loop_body: TK_VALUE { /*cout << "P:   value " << $1 << std::endl;*/
				 				current_data_item.addValue($1);  } 
	| loop_body TK_WHITESPACE TK_VALUE { /* cout << "P:   value " << $3 << std::endl;*/ 
								current_data_item.addValue($3); }
	;  

saveframe: TK_SAVE save_heading TK_WHITESPACE save_frame_content TK_WHITESPACE TK_SAVE 
					{ /*cout << "P: finished save frame : heading" << $2 << std::endl;*/
					current_saveframe.setCategory(current_saveframe.getDataItem("_Saveframe_category").entry.second);
					}
	;

save_heading: TK_SAVE_HEADING {
		current_saveframe.start($1);
}

save_frame_content: data_items {
									/*cout << "P: save_frame_content: first content " <<  std::endl;*/
									current_saveframe.addDataItem(current_data_item);
									}
	| save_frame_content TK_WHITESPACE data_items {
									/*cout << "P: save_frame_content: add content " << std::endl;*/
									current_saveframe.addDataItem(current_data_item);
									}
	;


optional_whitespace: /* empty */ { /*strcpy($$, "");*/ }
	| TK_WHITESPACE { /*strcpy($$, $1); */}
	;

%%
/*        additional C code            */

void yyerror(char* s)
{
	printf("Parse Error!\n");
}

/*
int main(int argc, char **argv)
{
	yydebug = 1;
	return yyparse();
}

*/
