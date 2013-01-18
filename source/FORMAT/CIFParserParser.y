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
bool is_textfield;
string value_helper;
string textfield_helper;

#undef BALL_CIFPARSERPARSER_DEBUG
//#define BALL_CIFPARSERPARSER_DEBUG

#ifdef BALL_CIFPARSERPARSER_DEBUG
#define CIF_DEBUG(a) Log.info() << a << std::endl;
#define YYDEBUG 1
#else
#define CIF_DEBUG(a)
#endif
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
%token <text>	  				TK_VALUE
%token 									TK_LOOP
%token 									TK_STOP
%token 									TK_SINGLE_QUOTE
%token 									TK_OPEN_SINGLE_QUOTE
%token 									TK_CLOSE_SINGLE_QUOTE
%token 									TK_DOUBLE_QUOTE
%token 									TK_OPEN_DOUBLE_QUOTE
%token 									TK_CLOSE_DOUBLE_QUOTE
%token 									TK_UNDERSCORE
%token 									TK_TEXTFIELD
%token <text>						TK_TEXTFIELD_LINE
%type  <text>		 				optional_whitespace
%type  <text>		 				single_quoted_string
%type  <text>		 				double_quoted_string
%type  <text>		 				single_quote_helper
%type  <text>		 				tag
%type  <text>		 				value
%type  <text>		 				value_helper
%type  <text>		 				value_helper_start
%type  <text>		 				textfield_line
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

data_heading: TK_DATA_HEADING { CIF_DEBUG("Started data heading")
						current_datablock.clear();
						current_datablock.start($1);
	}
	;

datablock_content: data_items { CIF_DEBUG("P: content: data items ")
								current_datablock.insertDatacontent(current_data_item); }
	| saveframe { CIF_DEBUG("P: content saveframe ")
								current_datablock.insertDatacontent(current_saveframe); }
	| datablock_content TK_WHITESPACE data_items { CIF_DEBUG("P: data block content of type data items!")
								current_datablock.insertDatacontent(current_data_item);;}
	| datablock_content TK_WHITESPACE saveframe  { CIF_DEBUG("P: data block content of type save frame!")
								current_datablock.insertDatacontent(current_saveframe);}
	;

data_items: tag optional_whitespace value {
					CIF_DEBUG("P: data item with tag title " << $1 << " and value " << value_helper);
							current_data_item.clear();
							current_data_item.addPair($1, value_helper);
						  value_helper = "";
					}
	|		tag TK_WHITESPACE {
				CIF_DEBUG("Found data item with title " << $1);
							current_data_item.clear();
							current_data_item.addPair($1, "");
				}
	|   loop tag_list optional_whitespace loop_body optional_whitespace TK_STOP {
					CIF_DEBUG("P: data item tag list loop body stop: ");
					}
	;

loop: TK_LOOP {
			current_data_item.startLoop();
		}
		;


tag_list: TK_WHITESPACE tag {
				current_data_item.addTag($2); CIF_DEBUG("P:   tag " << $2);   }
  | tag_list optional_whitespace tag {
				current_data_item.addTag($3); CIF_DEBUG("P:   tag " << $3);  }
	;

loop_body: value { 
						 		CIF_DEBUG("P:   value " << value_helper) 
				 				current_data_item.addValue(value_helper);  
					 			value_helper = "";
					 } 
	| loop_body optional_whitespace value { 
								CIF_DEBUG("P:   value " << value_helper ) 
								current_data_item.addValue(value_helper); 
								value_helper = "";
					 }
	;  

saveframe: TK_SAVE save_heading TK_WHITESPACE save_frame_content optional_whitespace TK_SAVE { 
					CIF_DEBUG("P: finished save frame : heading") 
					current_saveframe.setCategory(current_saveframe.getDataItem("_Saveframe_category").entry.second); }
	;

save_heading: TK_SAVE_HEADING {
		current_saveframe.start($1);
}

save_frame_content: data_items {
									CIF_DEBUG("P: save_frame_content: first content ") 
									current_saveframe.addDataItem(current_data_item);
									}
	| save_frame_content TK_WHITESPACE data_items {
									CIF_DEBUG("P: save_frame_content: add content ") 
									current_saveframe.addDataItem(current_data_item);
									}
	| save_frame_content data_items {
									CIF_DEBUG("P: save_frame_content: add content ") 
									current_saveframe.addDataItem(current_data_item);
									}
	;


optional_whitespace: /* empty */ { /*strcpy($$, "");*/ }
	| TK_WHITESPACE optional_whitespace { strcpy($$, $1); }
	;

tag: TK_UNDERSCORE value { 
	 		CIF_DEBUG("Found value " << value_helper);
			sprintf($$, "_%s", value_helper.c_str()); 
			value_helper = "";
		}
	 ;

value: value_helper_start { 
				  if (is_textfield) 
					{
						value_helper = textfield_helper;
						textfield_helper = "";
					}
					else
					{
						value_helper = string($1);
						textfield_helper = "";	
					}
				}
	;

value_helper_start: TK_VALUE                            { is_textfield = false; strcpy($$, $1); }
						| TK_VALUE single_quote_helper value_helper { is_textfield = false; sprintf($$, "%s%s%s", $1, $2, $3); }
						| TK_VALUE single_quote_helper              { is_textfield = false; sprintf($$, "%s%s", $1, $2); }
						| TK_VALUE TK_UNDERSCORE value_helper				{ is_textfield = false; sprintf($$, "%s_%s", $1, $3); }
						| TK_OPEN_SINGLE_QUOTE single_quoted_string { is_textfield = false; sprintf($$, "\'%s", $2); }
						| TK_OPEN_DOUBLE_QUOTE double_quoted_string { is_textfield = false; sprintf($$, "\"%s", $2); }
						| TK_TEXTFIELD textfield_line TK_TEXTFIELD  { is_textfield = true; }
	;

value_helper: TK_VALUE                                  { is_textfield = false; strcpy($$, $1); }
						| TK_VALUE single_quote_helper value_helper { is_textfield = false; sprintf($$, "%s%s%s", $1, $2, $3); }
						| TK_VALUE single_quote_helper              { is_textfield = false; sprintf($$, "%s%s", $1, $2); }
						| TK_VALUE TK_UNDERSCORE value_helper				{ is_textfield = false; sprintf($$, "%s_%s", $1, $3); }
	;

single_quote_helper: TK_SINGLE_QUOTE { strcpy($$, "\'"); }
						| TK_SINGLE_QUOTE single_quote_helper { sprintf($$, "\'%s", $2); }
	;

single_quoted_string: TK_CLOSE_SINGLE_QUOTE { strcpy($$, "\'"); }
			|	 TK_VALUE        single_quoted_string { sprintf($$, "%s%s", $1, $2); }
			|	 TK_WHITESPACE   single_quoted_string { sprintf($$, "%s%s", $1, $2); }
			|	 TK_VALUE TK_UNDERSCORE single_quoted_string { 
										CIF_DEBUG("single_quoted_string: %s_%s" << $1 << $3)
										sprintf($$, "%s_%s", $1, $3); }
      |  TK_SINGLE_QUOTE single_quoted_string { sprintf($$, "\'%s", $2); }
			|  TK_DOUBLE_QUOTE single_quoted_string { sprintf($$, "\"%s", $2); }
	;

double_quoted_string: TK_CLOSE_DOUBLE_QUOTE { 
										CIF_DEBUG("end double quoted string")
										strcpy($$, "\""); }
			|	 TK_VALUE        double_quoted_string { 
										CIF_DEBUG("double_quoted_string: value " << $1)
										sprintf($$, "%s%s", $1, $2); }
			|	 TK_VALUE TK_UNDERSCORE double_quoted_string { 
										CIF_DEBUG("double_quoted_string: %s_%s" << $1 << $3)
										sprintf($$, "%s_%s", $1, $3); }
			|	 TK_WHITESPACE   double_quoted_string { 
										CIF_DEBUG("double_quoted_string: whitespace" << $1)
										sprintf($$, "%s%s", $1, $2); }
      |  TK_DOUBLE_QUOTE double_quoted_string { 
										CIF_DEBUG("double_quoted_string: double quote and value" << $2)
										sprintf($$, "\"%s", $2); }
			|  TK_SINGLE_QUOTE double_quoted_string { 
										CIF_DEBUG("double_quoted_string: single quote")
										sprintf($$, "\'%s", $2); }
	;

textfield_line: /* empty */ {}
							| TK_TEXTFIELD_LINE textfield_line { 
								textfield_helper = string($1) + textfield_helper;
								CIF_DEBUG("TextField line: " << $1 << " " << $2); } ;

%%
/*        additional C code            */

void yyerror(char* s)
{
	printf("CIFParserParser(): Parse Error! %s \n",s);
}

/*
int main(int argc, char **argv)
{
	yydebug = 1;
	return yyparse();
}

*/
